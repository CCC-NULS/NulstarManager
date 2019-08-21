#include <WCConstants.h>
#include <QDate>
#include <QFont>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QTime>

#include "WCLog.h"

WCLog::WCLog(QWidget* parent)
     : QWidget(parent)
{
  setupUi(this);
  createModel();
  tvwLog->setColumnWidth(0,90);
}

void WCLog::appendEntry(int code, const WCMessage& message)
{
  QFont curFont = font();
  curFont.setItalic(true);
  int type = message.type();

  QStandardItem* codItem = new QStandardItem(QString::number(code));
  QStandardItem* date = new QStandardItem(QDate::currentDate().toString(Qt::ISODate));
  QStandardItem* time = new QStandardItem(QTime::currentTime().toString("hh:mm:ss"));
  QStandardItem* mesItem = new QStandardItem(message.text());
  codItem->setData(WCConstants::orange(), Qt::ForegroundRole);
  if(type == WCMessage::BugMessage)
    codItem->setData(QIcon(":/Resources/Images/Bug.png"), Qt::DecorationRole);
  if(type == WCMessage::SuccessMessage)
    codItem->setData(QIcon(":/Resources/Images/Tick.png"), Qt::DecorationRole);
  if(type == WCMessage::ErrorMessage)
    codItem->setData(QIcon(":/Resources/Images/Error.png"), Qt::DecorationRole);
  if(type == WCMessage::WarningMessage)
    codItem->setData(QIcon(":/Resources/Images/Warning.png"), Qt::DecorationRole);
  if(type == WCMessage::InfoMessage)
    codItem->setData(QIcon(":/Resources/Images/Info.png"), Qt::DecorationRole);
  date->setData(WCConstants::green(), Qt::ForegroundRole);
  date->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
  time->setData(WCConstants::green(), Qt::ForegroundRole);
  time->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
  mesItem->setData(WCConstants::darkBlue(), Qt::ForegroundRole);
  mesItem->setData(curFont, Qt::FontRole);
  QList<QStandardItem*> items;
  items << codItem << date << time << mesItem;
  _logModel->appendRow(items);
  tvwLog->scrollToBottom();
}

void WCLog::createModel()
{
  QStringList headers;
  headers << tr("Code") << tr("Date") << tr("Hour") << tr("Message");

  _logModel = new QStandardItemModel(0, 4, this);
  _logModel->setHorizontalHeaderLabels(headers);
  tvwLog->setModel(_logModel);
}
