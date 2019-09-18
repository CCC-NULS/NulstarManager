#ifndef NCREATEPACKAGE_H
#define NCREATEPACKAGE_H

#include <QFile>
#include <QWidget>
#include <NMessage.h>

#include "CreatePackage.h"
#include "ui_NCreatePackage.h"

class QSortFilterProxyModel;
class QStandardItemModel;

class CREATEPACKAGESHARED_EXPORT NCreatePackage : public QWidget, private Ui::uiCreatePackage
{
  Q_OBJECT

  public:
    explicit NCreatePackage(QWidget* parent = nullptr);
    void fCreateModel();
    void fCreateValidators();
    bool fCheckUIStatus() const;
    bool fStatus(const QLineEdit* rLineEdit) const { return rLineEdit->property("Status").toBool(); }

  protected:
    void fLoadPlatform();

  private:
    const quint64 cMinNameSize = 3;
    const quint64 cMaxNameSize = 50;

    QStandardItemModel* pFileModel;
    QStandardItemModel* pLogModel;
    QSortFilterProxyModel* pProxyModelFiles;
    QSortFilterProxyModel* pProxyModelLog;
    QFile mCreatePackageFile;
    void fSetStatus(QLineEdit* rLineEdit, bool fStatus);

  public slots:
    void fCreatePackage();
    void fAddLog();
    void fDeleteLog();
    void fLoadFiles();
    void fVerifyLogModel();
    void fLoadTables();
    void fValidateParameter(const QString& lParameter);

  signals:
    void sEventGenerated(int lCode,QString lMessage,int lType);
};

#endif // NCREATEPACKAGE_H
