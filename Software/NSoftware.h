#ifndef NSOFTWARE_H
#define NSOFTWARE_H

#include <QFile>
#include <QWidget>
#include <NMessage.h>

#include "Software.h"
#include "NSoftwareRecord.h"
#include "ui_NSoftware.h"

class QSortFilterProxyModel;
class QStandardItemModel;

class SOFTWARESHARED_EXPORT NSoftware : public QWidget, private Ui::uiNSoftware
{
  Q_OBJECT

  public:
    explicit NSoftware(QWidget* parent = nullptr);
    void fCreateModel();
    void fMoveToCenter(QWidget* window);

  private:
    NSoftwareRecord* pRecord;
    QStandardItemModel* pSoftwareModel;
    QSortFilterProxyModel* pProxyModel;
    QFile mSoftwareFile;

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
    void sDataChanged();
};

#endif // NSOFTWARE_H
