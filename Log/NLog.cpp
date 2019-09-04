#include <NConstants.h>
#include <QDate>
#include <QFile>
#include <QFont>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QTime>

#include "NLog.h"

NLog::NLog(QWidget* parent)
    : QWidget(parent) {
  setupUi(this);
  createModel();
  tvwLog->setColumnWidth(0,110);
  tvwLog->setColumnWidth(1,110);
  tvwLog->setColumnWidth(2,110);

  _logFile.setFileName("NulstarManager.log");
  if(!_logFile.open(QFile::WriteOnly | QFile::Append)) {
    NMessage message(NMessage::ErrorMessage, tr("Log file NulstarManager.log could not be opened, logs won't be saved."));
    fAppendEntry(1000001, message);
  }
}

void NLog::fAppendEntry(int code, const NMessage& message) {
  QFont curFont = font();
  curFont.setItalic(true);
  int type = message.type();

  QStandardItem* codItem = new QStandardItem(QString::number(code));
  QStandardItem* date = new QStandardItem(QDate::currentDate().toString(Qt::ISODate));
  QStandardItem* time = new QStandardItem(QTime::currentTime().toString("hh:mm:ss"));
  QStandardItem* mesItem = new QStandardItem(message.text());
  QString lMessageType;
  codItem->setData(NConstants::orange(), Qt::ForegroundRole);
  if(type == NMessage::BugMessage) {
    codItem->setData(QIcon(":/Resources/Images/Bug.png"), Qt::DecorationRole);
    lMessageType = tr("Bug");
  }
  if(type == NMessage::SuccessMessage) {
    codItem->setData(QIcon(":/Resources/Images/Tick.png"), Qt::DecorationRole);
    lMessageType = tr("Success");
  }
  if(type == NMessage::ErrorMessage) {
    codItem->setData(QIcon(":/Resources/Images/Error.png"), Qt::DecorationRole);
    lMessageType = tr("Error");
  }
  if(type == NMessage::WarningMessage) {
    codItem->setData(QIcon(":/Resources/Images/Warning.png"), Qt::DecorationRole);
    lMessageType = tr("Warning");
  }
  if(type == NMessage::InfoMessage) {
    codItem->setData(QIcon(":/Resources/Images/Info.png"), Qt::DecorationRole);
    lMessageType = tr("Info");
  }
  date->setData(NConstants::green(), Qt::ForegroundRole);
  date->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
  time->setData(NConstants::green(), Qt::ForegroundRole);
  time->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
  mesItem->setData(NConstants::darkBlue(), Qt::ForegroundRole);
  mesItem->setData(curFont, Qt::FontRole);
  QList<QStandardItem*> items;
  items << codItem << date << time << mesItem;
  _logModel->appendRow(items);
  tvwLog->scrollToBottom();

  if(_logFile.isOpen()) {
    QTextStream lOutStream(&_logFile);
    lOutStream << date->data(Qt::EditRole).toString() << qSetFieldWidth(12) << left << "   " << lMessageType << qSetFieldWidth(12)  << QString::number(code) << qSetFieldWidth(12)  << message.text() << endl;
  }
}

void NLog::createModel() {
  QStringList headers;
  headers << tr("Code") << tr("Date") << tr("Hour") << tr("Message");

  _logModel = new QStandardItemModel(0, 4, this);
  _logModel->setHorizontalHeaderLabels(headers);
  tvwLog->setModel(_logModel);
}
