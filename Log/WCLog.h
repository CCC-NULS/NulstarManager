#ifndef WCLOG_H
#define WCLOG_H

#include <WCMessage.h>
#include <QWidget>

#include "Log.h"
#include "ui_Log.h"

class QStandardItemModel;

class WCLog : public QWidget, private Ui::uiLog
{
  Q_OBJECT

  public:
    explicit WCLog(QWidget* parent = 0);
    void createModel();

  private:
    QStandardItemModel* _logModel;

  public slots:
    void appendEntry(int code, const WCMessage& message);
};

#endif // WCCONNECTOR_H
