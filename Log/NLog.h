#ifndef NLOG_H
#define NLOG_H

#include <NMessage.h>
#include <QFile>
#include <QWidget>

#include "Log.h"
#include "ui_Log.h"

class QStandardItemModel;

class LOGSHARED_EXPORT NLog : public QWidget, private Ui::uiLog
{
  Q_OBJECT

  public:
    explicit NLog(QWidget* parent = nullptr);
    void createModel();

  private:
    QStandardItemModel* _logModel;
    QFile _logFile;

  public slots:
    void appendEntry(int code, const NMessage& message);
};

#endif // NLOG_H
