#ifndef NCREATEPACKAGE_H
#define NCREATEPACKAGE_H

#include <QDir>
#include <QFile>
#include <QMap>
#include <QWidget>
#include <NMessage.h>
#include <secp256k1.h>

#include "CreatePackage.h"
#include "ui_NCreatePackage.h"

class QSortFilterProxyModel;
class QStandardItemModel;
class QStandardItem;

class CREATEPACKAGESHARED_EXPORT NCreatePackage : public QWidget, private Ui::uiCreatePackage
{
  Q_OBJECT

  public:
    explicit NCreatePackage(QWidget* parent = nullptr);
    ~NCreatePackage();
    void fCreateModel();
    void fCreateValidators();
    bool fCheckUIStatus() const;
    bool fStatus(const QLineEdit* rLineEdit) const { return rLineEdit->property("Status").toBool(); }
    QByteArray fFileChecksum(const QString& lFileName);

  protected:
    void fLoadPlatform();
    void fLoadSoftware();

  private:
    const quint64 cMinNameSize = 3;
    const quint64 cMaxNameSize = 50;
    QStandardItemModel* pFileModel;
    QStandardItemModel* pLogModel;
    QSortFilterProxyModel* pProxyModelFiles;
    QSortFilterProxyModel* pProxyModelLog;
    QString mCreatePackageDirectory;
    secp256k1_context* rContextSign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    secp256k1_context* rContextVerify = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
    QString fGetStringFromUnsignedChar(unsigned char* rStr);
    QString lContentsHeaderToSign;
    QMap<QString,QString> lContentsToSign;
    void fSetStatus(QLineEdit* rLineEdit, bool fStatus);
    void fWritePackageSummary(const QString &lManifestFile);
    void fWriteUpgradeLogs(const QString &lManifestFile);
    void fCreateSignature(const QString &lManifestFile);

  public slots:
    void fCreatePackage();
    void fAddLog();
    void fDeleteLog();
    void fLoadFiles();
    bool fProcessDirectories(const QDir& lDir);
    bool fProcessFiles(const QDir& lDir, const QString& lRelativePath);
    bool fProcessLibraries(const QDir& lDir);
    bool fProcessModules(const QDir& lDir);
    void fVerifyLogModel(QStandardItem* rItem);
    void fLoadTables();
    void fValidateParameter(const QString& lParameter);
    bool fCreatePackageDirectory(const QString& lPath);

  signals:
    void sEventGenerated(int lCode,QString lMessage,int lType);
};

#endif // NCREATEPACKAGE_H
