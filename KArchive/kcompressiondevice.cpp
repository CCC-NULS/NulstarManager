/* This file is part of the KDE libraries
   Copyright (C) 2000 David Faure <faure@kde.org>
   Copyright (C) 2011 Mario Bensi <mbensi@ipsquad.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kcompressiondevice.h"
#include "kcompressiondevice_p.h"
#include "loggingcategory.h"
#include <config-compression.h>
#include "kfilterbase.h"
#include <QFile>
#include <stdio.h> // for EOF
#include <stdlib.h>
#include <assert.h>

#include "kgzipfilter.h"
#include "knonefilter.h"

#if HAVE_BZIP2_SUPPORT
#include "kbzip2filter.h"
#endif
#if HAVE_XZ_SUPPORT
#include "kxzfilter.h"
#endif

#include <QDebug>

class KCompressionDevicePrivate
{
public:
    KCompressionDevicePrivate(KCompressionDevice *q)
        : bNeedHeader(true)
        , bSkipHeaders(false)
        , bOpenedUnderlyingDevice(false)
        , type(KCompressionDevice::None)
        , errorCode(QFileDevice::NoError)
        , deviceReadPos(0)
        , q(q)
    {
    }

    void propagateErrorCode();

    bool bNeedHeader;
    bool bSkipHeaders;
    bool bOpenedUnderlyingDevice;
    QByteArray buffer; // Used as 'input buffer' when reading, as 'output buffer' when writing
    QByteArray origFileName;
    KFilterBase::Result result;
    KFilterBase *filter;
    KCompressionDevice::CompressionType type;
    QFileDevice::FileError errorCode;
    qint64 deviceReadPos;
    KCompressionDevice *q;
};

void KCompressionDevicePrivate::propagateErrorCode()
{
    QIODevice *dev = filter->device();
    if (QFileDevice *fileDev = qobject_cast<QFileDevice *>(dev)) {
        if (fileDev->error() != QFileDevice::NoError) {
            errorCode = fileDev->error();
            q->setErrorString(dev->errorString());
        }
    }
    // ... we have no generic way to propagate errors from other kinds of iodevices. Sucks, heh? :(
}

KFilterBase *KCompressionDevice::filterForCompressionType(KCompressionDevice::CompressionType type)
{
    switch (type) {
    case KCompressionDevice::GZip:
        return new KGzipFilter;
    case KCompressionDevice::BZip2:
#if HAVE_BZIP2_SUPPORT
        return new KBzip2Filter;
#else
        return nullptr;
#endif
    case KCompressionDevice::Xz:
#if HAVE_XZ_SUPPORT
        return new KXzFilter;
#else
        return nullptr;
#endif
    case KCompressionDevice::None:
        return new KNoneFilter;
    }
    return nullptr;
}

KCompressionDevice::KCompressionDevice(QIODevice *inputDevice, bool autoDeleteInputDevice, CompressionType type)
    : d(new KCompressionDevicePrivate(this))
{
    assert(inputDevice);
    d->filter = filterForCompressionType(type);
    if (d->filter) {
        d->type = type;
        d->filter->setDevice(inputDevice, autoDeleteInputDevice);
    }
}

KCompressionDevice::KCompressionDevice(const QString &fileName, CompressionType type)
    : d(new KCompressionDevicePrivate(this))
{
    QFile *f = new QFile(fileName);
    d->filter = filterForCompressionType(type);
    if (d->filter) {
        d->type = type;
        d->filter->setDevice(f, true);
    } else {
        delete f;
    }
}

KCompressionDevice::~KCompressionDevice()
{
    if (isOpen()) {
        close();
    }
    delete d->filter;
    delete d;
}

KCompressionDevice::CompressionType KCompressionDevice::compressionType() const
{
    return d->type;
}

bool KCompressionDevice::open(QIODevice::OpenMode mode)
{
    if (isOpen()) {
        //qCWarning(KArchiveLog) << "KCompressionDevice::open: device is already open";
        return true; // QFile returns false, but well, the device -is- open...
    }
    if (!d->filter) {
        return false;
    }
    d->bOpenedUnderlyingDevice = false;
    //qCDebug(KArchiveLog) << mode;
    if (mode == QIODevice::ReadOnly) {
        d->buffer.resize(0);
    } else {
        d->buffer.resize(BUFFER_SIZE);
        d->filter->setOutBuffer(d->buffer.data(), d->buffer.size());
    }
    if (!d->filter->device()->isOpen()) {
        if (!d->filter->device()->open(mode)) {
            //qCWarning(KArchiveLog) << "KCompressionDevice::open: Couldn't open underlying device";
            d->propagateErrorCode();
            return false;
        }
        d->bOpenedUnderlyingDevice = true;
    }
    d->bNeedHeader = !d->bSkipHeaders;
    d->filter->setFilterFlags(d->bSkipHeaders ? KFilterBase::NoHeaders : KFilterBase::WithHeaders);
    if (!d->filter->init(mode)) {
        return false;
    }
    d->result = KFilterBase::Ok;
    setOpenMode(mode);
    return true;
}

void KCompressionDevice::close()
{
    if (!isOpen()) {
        return;
    }
    if (d->filter->mode() == QIODevice::WriteOnly && d->errorCode == QFileDevice::NoError) {
        write(nullptr, 0);    // finish writing
    }
    //qCDebug(KArchiveLog) << "Calling terminate().";

    if (!d->filter->terminate()) {
        //qCWarning(KArchiveLog) << "KCompressionDevice::close: terminate returned an error";
        d->errorCode = QFileDevice::UnspecifiedError;
    }
    if (d->bOpenedUnderlyingDevice) {
        QIODevice *dev = d->filter->device();
        dev->close();
        d->propagateErrorCode();
    }
    setOpenMode(QIODevice::NotOpen);
}

QFileDevice::FileError KCompressionDevice::error() const
{
    return d->errorCode;
}

bool KCompressionDevice::seek(qint64 pos)
{
    if (d->deviceReadPos == pos) {
        return QIODevice::seek(pos);
    }

    //qCDebug(KArchiveLog) << "seek(" << pos << ") called, current pos=" << QIODevice::pos();

    Q_ASSERT(d->filter->mode() == QIODevice::ReadOnly);

    if (pos == 0) {
        if (!QIODevice::seek(pos))
            return false;

        // We can forget about the cached data
        d->bNeedHeader = !d->bSkipHeaders;
        d->result = KFilterBase::Ok;
        d->filter->setInBuffer(nullptr, 0);
        d->filter->reset();
        d->deviceReadPos = 0;
        return d->filter->device()->reset();
    }

    qint64 bytesToRead;
    if (d->deviceReadPos < pos) { // we can start from here
        bytesToRead = pos - d->deviceReadPos;
        // Since we're going to do a read() below
        // we need to reset the internal QIODevice pos to the real position we are
        // so that after read() we are indeed pointing to the pos seek
        // asked us to be in
        if (!QIODevice::seek(d->deviceReadPos)) {
            return false;
        }
    } else {
        // we have to start from 0 ! Ugly and slow, but better than the previous
        // solution (KTarGz was allocating everything into memory)
        if (!seek(0)) { // recursive
            return false;
        }
        bytesToRead = pos;
    }

    //qCDebug(KArchiveLog) << "reading " << bytesToRead << " dummy bytes";
    QByteArray dummy(qMin(bytesToRead, qint64(SEEK_BUFFER_SIZE)), 0);
    while (bytesToRead > 0) {
        const qint64 bytesToReadThisTime = qMin(bytesToRead, qint64(dummy.size()));
        const bool result = (read(dummy.data(), bytesToReadThisTime) == bytesToReadThisTime);
        if (!result) {
            return false;
        }
        bytesToRead -= bytesToReadThisTime;
    }
    return true;
}

bool KCompressionDevice::atEnd() const
{
    return (d->type == KCompressionDevice::None || d->result == KFilterBase::End)
           && QIODevice::atEnd() // take QIODevice's internal buffer into account
           && d->filter->device()->atEnd();
}

qint64 KCompressionDevice::readData(char *data, qint64 maxlen)
{
    Q_ASSERT(d->filter->mode() == QIODevice::ReadOnly);
    //qCDebug(KArchiveLog) << "maxlen=" << maxlen;
    KFilterBase *filter = d->filter;

    uint dataReceived = 0;

    // We came to the end of the stream
    if (d->result == KFilterBase::End) {
        return dataReceived;
    }

    // If we had an error, return -1.
    if (d->result != KFilterBase::Ok) {
        return -1;
    }

    qint64 availOut = maxlen;
    filter->setOutBuffer(data, maxlen);

    while (dataReceived < maxlen) {
        if (filter->inBufferEmpty()) {
            // Not sure about the best size to set there.
            // For sure, it should be bigger than the header size (see comment in readHeader)
            d->buffer.resize(BUFFER_SIZE);
            // Request data from underlying device
            int size = filter->device()->read(d->buffer.data(),
                                              d->buffer.size());
            //qCDebug(KArchiveLog) << "got" << size << "bytes from device";
            if (size) {
                filter->setInBuffer(d->buffer.data(), size);
            } else {
                // Not enough data available in underlying device for now
                break;
            }
        }
        if (d->bNeedHeader) {
            (void) filter->readHeader();
            d->bNeedHeader = false;
        }

        d->result = filter->uncompress();

        if (d->result == KFilterBase::Error) {
            //qCWarning(KArchiveLog) << "KCompressionDevice: Error when uncompressing data";
            break;
        }

        // We got that much data since the last time we went here
        uint outReceived = availOut - filter->outBufferAvailable();
        //qCDebug(KArchiveLog) << "avail_out = " << filter->outBufferAvailable() << " result=" << d->result << " outReceived=" << outReceived;
        if (availOut < uint(filter->outBufferAvailable())) {
            //qCWarning(KArchiveLog) << " last availOut " << availOut << " smaller than new avail_out=" << filter->outBufferAvailable() << " !";
        }

        dataReceived += outReceived;
        data += outReceived;
        availOut = maxlen - dataReceived;
        if (d->result == KFilterBase::End) {
            // We're actually at the end, no more data to check
            if (filter->device()->atEnd()) {
                break;
            }

            // Still not done, re-init and try again
            filter->init(filter->mode());
        }
        filter->setOutBuffer(data, availOut);
    }

    d->deviceReadPos += dataReceived;
    return dataReceived;
}

qint64 KCompressionDevice::writeData(const char *data /*0 to finish*/, qint64 len)
{
    KFilterBase *filter = d->filter;
    Q_ASSERT(filter->mode() == QIODevice::WriteOnly);
    // If we had an error, return 0.
    if (d->result != KFilterBase::Ok) {
        return 0;
    }

    bool finish = (data == nullptr);
    if (!finish) {
        filter->setInBuffer(data, len);
        if (d->bNeedHeader) {
            (void)filter->writeHeader(d->origFileName);
            d->bNeedHeader = false;
        }
    }

    uint dataWritten = 0;
    uint availIn = len;
    while (dataWritten < len || finish) {

        d->result = filter->compress(finish);

        if (d->result == KFilterBase::Error) {
            //qCWarning(KArchiveLog) << "KCompressionDevice: Error when compressing data";
            // What to do ?
            break;
        }

        // Wrote everything ?
        if (filter->inBufferEmpty() || (d->result == KFilterBase::End)) {
            // We got that much data since the last time we went here
            uint wrote = availIn - filter->inBufferAvailable();

            //qCDebug(KArchiveLog) << " Wrote everything for now. avail_in=" << filter->inBufferAvailable() << "result=" << d->result << "wrote=" << wrote;

            // Move on in the input buffer
            data += wrote;
            dataWritten += wrote;

            availIn = len - dataWritten;
            //qCDebug(KArchiveLog) << " availIn=" << availIn << "dataWritten=" << dataWritten << "pos=" << pos();
            if (availIn > 0) {
                filter->setInBuffer(data, availIn);
            }
        }

        if (filter->outBufferFull() || (d->result == KFilterBase::End) || finish) {
            //qCDebug(KArchiveLog) << " writing to underlying. avail_out=" << filter->outBufferAvailable();
            int towrite = d->buffer.size() - filter->outBufferAvailable();
            if (towrite > 0) {
                // Write compressed data to underlying device
                int size = filter->device()->write(d->buffer.data(), towrite);
                if (size != towrite) {
                    //qCWarning(KArchiveLog) << "KCompressionDevice::write. Could only write " << size << " out of " << towrite << " bytes";
                    d->errorCode = QFileDevice::WriteError;
                    setErrorString(tr("Could not write. Partition full?"));
                    return 0; // indicate an error
                }
                //qCDebug(KArchiveLog) << " wrote " << size << " bytes";
            }
            if (d->result == KFilterBase::End) {
                Q_ASSERT(finish); // hopefully we don't get end before finishing
                break;
            }
            d->buffer.resize(BUFFER_SIZE);
            filter->setOutBuffer(d->buffer.data(), d->buffer.size());
        }
    }

    return dataWritten;
}

void KCompressionDevice::setOrigFileName(const QByteArray &fileName)
{
    d->origFileName = fileName;
}

void KCompressionDevice::setSkipHeaders()
{
    d->bSkipHeaders = true;
}

KFilterBase *KCompressionDevice::filterBase()
{
    return d->filter;
}
