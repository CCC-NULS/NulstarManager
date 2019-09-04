#ifndef NPLATFORM_H
#define NPLATFORM_H

#include <QFile>
#include <QWidget>
#include <NMessage.h>

#include "Platform.h"
#include "NPlatformRecord.h"
#include "ui_NPlatform.h"

class QSortFilterProxyModel;
class QStandardItemModel;

class PLATFORMSHARED_EXPORT NPlatform : public QWidget, private Ui::uiNPlatform
{
  Q_OBJECT

  public:
    explicit NPlatform(QWidget* parent = nullptr);
    void fCreateModel();
    void fMoveToCenter(QWidget* window);

  private:
    NPlatformRecord* pRecord;
    QStandardItemModel* pPlatformModel;
    QSortFilterProxyModel* pProxyModel;
    QFile mPlatformFile;

  public slots:
    void fAddRecord();
    void fSaveAddRecord(const QStringList& lValues);
    void fDeleteRecord();
    void fEditRecord();
    void fSaveEditRecord(const QStringList& lValues);
    void fEditRecordOnTable(int lRow, const QStringList& lValues);
    void fInsertRecordOnTable(const QStringList& values);
    void fLoadModel();
    bool fSaveModel();

  signals:
    void sEventGenerated(int lCode,QString lMessage,int lType);
};

#endif // NPLATFORM_H
