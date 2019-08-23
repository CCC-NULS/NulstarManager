#ifndef WCVERSION_H
#define WCVERSION_H

#include <QObject>
#include <QString>

#include "Definitions.h"

class WCVersion : public QObject
{
  Q_OBJECT

  public:
    explicit WCVersion(QObject* parent = nullptr);

    static QString fileRegion(const QString& fileName);
    static QString fileToObject(int os, const QString& fileName);
    static QString normalizeFile(const QString& fileName); // Sacar la region del nombre por ejm: Pulzar__2__.exe -> Pulzar.exe
    static QString worldcoinManagerAlias();
    static QString worldcoinManagerVersion();
    static QString worldcoinManagerVersionCompressed(const QString& sversion);
    static QString worldcoinManagerVersionExtended();
    static QString worldcoinManagerVersionExtended(const QString& sversion);
};

#endif // AXVERSION_H

