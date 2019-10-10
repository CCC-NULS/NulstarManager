#include <QApplication>
#include <QCryptographicHash>
#include <QDesktopWidget>
#include <QDate>
#include <QDialog>
#include <QFileDialog>
#include <QFont>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QModelIndexList>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <QVersionNumber>
#include <JlCompress.h>
#include <NConstants.h>
#include <NValidatorCreator.h>

#include "NCreatePackage.h"

const QString cLibrariesDirectory("Libraries");
const QString cModulesDirectory("Modules");
const QString cExec("Exec");
const QString cTempDir("Temp");
const QString cHashAlgorithm("Sha256");

NCreatePackage::NCreatePackage(QWidget* parent)
         : QWidget(parent) {
  setupUi(this);
  fCreateModel();  
  fCreateValidators();
}


void NCreatePackage::fCreateModel() {
  QStringList lHeaders;
  lHeaders << tr("Relative Directory") << tr("File Name") << tr("Size") << tr("Hash - Sha256");
  pFileModel = new QStandardItemModel(0, 4, this);
  pFileModel->setHorizontalHeaderLabels(lHeaders);

  lHeaders.clear();
  lHeaders << tr("Component") << tr("Type") << tr("Log");
  pLogModel = new QStandardItemModel(0, 3, this);
  pLogModel->setHorizontalHeaderLabels(lHeaders);

  pProxyModelFiles = new QSortFilterProxyModel(this);
  pProxyModelFiles->setSortRole(Qt::DisplayRole);
  pProxyModelFiles->setDynamicSortFilter(true);
  pProxyModelFiles->setSourceModel(pFileModel);

  pProxyModelLog = new QSortFilterProxyModel(this);
  pProxyModelLog->setSortRole(Qt::DisplayRole);
  pProxyModelLog->setDynamicSortFilter(false);
  pProxyModelLog->setSourceModel(pLogModel);

  tvwFiles->setSortingEnabled(true);
  tvwFiles->setModel(pProxyModelFiles);
  tvwFiles->sortByColumn(0, Qt::AscendingOrder);
  tvwFiles->setColumnWidth(0,180);
  tvwFiles->setColumnWidth(1,180);
  tvwFiles->setColumnWidth(2,110);

  tvwLog->setSortingEnabled(false);
  tvwLog->setModel(pProxyModelLog);
  tvwLog->setColumnWidth(0,110);
  tvwLog->setColumnWidth(1,110);

  tvwUpgradeCommand->hide();
  tbtAddCommand->hide();
  tbtDeleteCommand->hide();

  dedDate->setProperty("Status", true);
  txtDescription->setProperty("Status", true);
  txtDescription->setStyleSheet(QString("background-color: rgb%1").arg(NConstants::colorToRgbText(NConstants::lightBlue())));
  pFileModel->setProperty("Status", QString("0"));

  connect(tbtAddLog, SIGNAL(clicked()), this, SLOT(fAddLog()));
  connect(tbtDeleteLog, SIGNAL(clicked()), this, SLOT(fDeleteLog()));
  connect(pbtLoadFiles, SIGNAL(clicked()), this, SLOT(fLoadFiles()));
}

void NCreatePackage::fCreateValidators() {
  NValidatorCreator::createValidator(ledName, NConstants::NameRegExp, cMinNameSize, cMaxNameSize);
  NValidatorCreator::createValidator(ledVersion, NConstants::VersionRegExp, 5, 8);

  fSetStatus(ledName, false);
  fSetStatus(ledVersion, false);

  connect(ledName, SIGNAL(textChanged(QString)), this, SLOT(fValidateParameter(QString)));
  connect(ledVersion, SIGNAL(textChanged(QString)), this, SLOT(fValidateParameter(QString)));
  connect(pbtCancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(pbtCreatePackage, SIGNAL(clicked()), this, SLOT(fCreatePackage()));
  connect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));

}

void NCreatePackage::fSetStatus(QLineEdit* rLineEdit, bool fStatus) {
  QColor lColor;
  if(fStatus){
    lColor = NConstants::lightBlue();
    rLineEdit->setProperty("Status", true);
  }
  else {
    lColor = NConstants::lightRed();
     rLineEdit->setProperty("Status", false);
  }
  rLineEdit->setStyleSheet(QString("background-color: rgb%1").arg(NConstants::colorToRgbText(lColor)));
  pbtCreatePackage->setEnabled(fCheckUIStatus());
}

void NCreatePackage::fLoadFiles() {
  QFileDialog lFileDialog(this);
  lFileDialog.setFileMode(QFileDialog::Directory);
  QString lDirName = lFileDialog.getExistingDirectory(this, tr("Directory"), QString(), QFileDialog::ShowDirsOnly);
  pFileModel->removeRows(0, pFileModel->rowCount());
  lblTotalFiles->setText("0");
  lblTotalSize->setText("0");
  pbtCreatePackage->setEnabled(fCheckUIStatus());
  if(lDirName.isEmpty())
    return;
  bool lSuccess = fProcessDirectories(lDirName);
  if(lSuccess)
    pFileModel->setProperty("Status", QString("1"));
  else
    pFileModel->setProperty("Status", QString("0"));
  pbtCreatePackage->setEnabled(fCheckUIStatus());
  QDir lCreatePackageDir(lDirName);
  lCreatePackageDir.cdUp();
  mCreatePackageDirectory = lCreatePackageDir.path();
}

bool NCreatePackage::fProcessLibraries(const QDir& lDir) {
  QStringList lLanguageEntries = lDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  if(lLanguageEntries.isEmpty()) {
    QString lMessage(tr("'%1' directory is empty.").arg(cLibrariesDirectory));
    emit sEventGenerated(5002005,lMessage, NMessage::ErrorMessage);
    return false;
  }
  for(const QString& lLanguage : lLanguageEntries) {
    QDir lLanguageDir(QString("%1/%2").arg(lDir.path()).arg(lLanguage));
    QStringList lLibraryEntries = lLanguageDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    if(lLibraryEntries.isEmpty()) {
      QString lMessage(tr("No libraries found for language '%1'.").arg(lLanguage));
      emit sEventGenerated(5002006,lMessage, NMessage::ErrorMessage);
      return false;
    }
    for(const QString& lLibrary : lLibraryEntries) {
      QDir lLibraryDir(QString("%1/%2").arg(lLanguageDir.path()).arg(lLibrary));
      QStringList lVersionEntries = lLibraryDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
      if(lVersionEntries.isEmpty()) {
        QString lMessage(tr("No versions found for library '%1'.").arg(lLibrary));
        emit sEventGenerated(5002007,lMessage, NMessage::ErrorMessage);
        return false;
      }
      QVersionNumber lCurrentMaxVersion(0,0,0);
      QString lVersionString;
      for(const QString& lVersion : lVersionEntries) {
        QVersionNumber lCurrentVersion(QVersionNumber::fromString(lVersion));
        if(lCurrentMaxVersion < lCurrentVersion) {
           lCurrentMaxVersion = lCurrentVersion;
           lVersionString = lVersion;
        }
      }
      QDir lVersionDir(QString("%1/%2").arg(lLibraryDir.path()).arg(lVersionString));
      QString lRelativePath(QString("%1/%2/%3/%4").arg(cLibrariesDirectory).arg(lLanguage).arg(lLibrary).arg(lVersionString));
      if(!fProcessFiles(lVersionDir, lRelativePath))
        return false;
    }
  }
  return true;
}

bool NCreatePackage::fProcessModules(const QDir& lDir) {
  QStringList lDomainEntries = lDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  if(lDomainEntries.isEmpty()) {
    QString lMessage(tr("'%1' directory is empty.").arg(cModulesDirectory));
    emit sEventGenerated(5002008,lMessage, NMessage::ErrorMessage);
    return false;
  }
  for(const QString& lDomain : lDomainEntries) {
    QDir lDomainDir(QString("%1/%2").arg(lDir.path()).arg(lDomain));
    QStringList lModuleEntries = lDomainDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    if(lModuleEntries.isEmpty()) {
      QString lMessage(tr("No modules found for domain '%1'.").arg(lDomain));
      emit sEventGenerated(5002009,lMessage, NMessage::ErrorMessage);
      return false;
    }
    for(const QString& lModule : lModuleEntries) {
      QDir lModuleDir(QString("%1/%2").arg(lDomainDir.path()).arg(lModule));
      QStringList lVersionEntries = lModuleDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
      if(lVersionEntries.isEmpty() && (lModule != "libs")) {  // CHANGE!!!
        QString lMessage(tr("No versions found for module '%1'.").arg(lModule));
        emit sEventGenerated(5002010,lMessage, NMessage::ErrorMessage);
        return false;
      }
      QVersionNumber lCurrentMaxVersion(0,0,0);
      QString lVersionString;
      for(const QString& lVersion : lVersionEntries) {
        QVersionNumber lCurrentVersion(QVersionNumber::fromString(lVersion));
        if(lCurrentMaxVersion < lCurrentVersion) {
           lCurrentMaxVersion = lCurrentVersion;
           lVersionString = lVersion;
        }
      }
      QDir lVersionDir(QString("%1/%2").arg(lModuleDir.path()).arg(lVersionString));
      QString lRelativePath(QString("%1/%2/%3/%4").arg(cModulesDirectory).arg(lDomain).arg(lModule).arg(lVersionString));
      if(lModule == "libs") {   // CHANGE!!
        lVersionDir.setPath(lModuleDir.path());
        lRelativePath = QString("%1/%2/%3").arg(cModulesDirectory).arg(lDomain).arg(lModule);
        QString lMessage(tr("'%1' modules has no versions!").arg(lModule));
        emit sEventGenerated(5002011,lMessage, NMessage::WarningMessage);
      }
      if(!fProcessFiles(lVersionDir, lRelativePath))
        return false;
    }
  }
  return true;
}

bool NCreatePackage::fProcessFiles(const QDir& lDir, const QString& lRelativePath) {
  QStringList lFileEntries = lDir.entryList(QDir::Files | QDir::CaseSensitive | QDir::NoSymLinks);
  QStringList lDirEntries = lDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for(const QString& lFile : lFileEntries) {
    QFile lFilePath(QString("%1/%2").arg(lDir.path()).arg(lFile));
    if(!lFilePath.open(QIODevice::ReadOnly)) {
      QString lMessage(tr("Unable to read file '%1'").arg(lFilePath.fileName()));
      emit sEventGenerated(5002003,lMessage, NMessage::ErrorMessage);
      return false;
    }
    QByteArray lFileContents = lFilePath.readAll();
    QString lHash = QString(QCryptographicHash::hash(lFileContents,QCryptographicHash::Sha256).toHex());
    QStandardItem* lDirectory = new QStandardItem(lRelativePath);
    lDirectory->setData(lDir.path(), Qt::ToolTipRole);
    QStandardItem* lFileName = new QStandardItem(lFile);
    QStandardItem* lSize = new QStandardItem(QString::number(lFilePath.size()));
    QStandardItem* lFileHash = new QStandardItem(lHash);
    QList<QStandardItem* > lItemList({lDirectory, lFileName, lSize, lFileHash});
    pFileModel->appendRow(lItemList);
    qApp->processEvents();
  }
  bool lSuccess(true);
  for(const QString& lDirName : lDirEntries) {
    QString lNewPath(QString("%1/%2").arg(lRelativePath).arg(lDirName));
    lSuccess = lSuccess && fProcessFiles(QDir(QString("%1/%2").arg(lDir.path()).arg(lDirName)), lNewPath);
  }
  return lSuccess;
}

bool NCreatePackage::fProcessDirectories(const QDir& lDir) {
  QStringList lFileEntries = lDir.entryList(QDir::Files | QDir::CaseSensitive | QDir::NoSymLinks);
  //QStringList lDirEntries = lDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for(const QString& lFile : lFileEntries) {
    QFile lFilePath(QString("%1/%2").arg(lDir.path()).arg(lFile));
    if(!lFilePath.open(QIODevice::ReadOnly)) {
      QString lMessage(tr("Unable to read file '%1'").arg(lFilePath.fileName()));
      emit sEventGenerated(5002003,lMessage, NMessage::ErrorMessage);
      return false;
    }
    QByteArray lFileContents = lFilePath.readAll();
    QString lHash = QString(QCryptographicHash::hash(lFileContents,QCryptographicHash::Sha256).toHex());
    QStandardItem* lDirectory = new QStandardItem(".");
    lDirectory->setData(lDir.path(), Qt::ToolTipRole);
    QStandardItem* lFileName = new QStandardItem(lFile);
    QStandardItem* lSize = new QStandardItem(QString::number(lFilePath.size()));
    QStandardItem* lFileHash = new QStandardItem(lHash);
    QList<QStandardItem* > lItemList({lDirectory, lFileName, lSize, lFileHash});
    pFileModel->appendRow(lItemList);
  }
  bool lSuccess = false;
  QDir lLibrariesDir(QString("%1/%2").arg(lDir.path()).arg(cLibrariesDirectory));
  if(lLibrariesDir.exists()) {
    lSuccess = fProcessLibraries(lLibrariesDir);
  }
  QDir lModulesDir(QString("%1/%2").arg(lDir.path()).arg(cModulesDirectory));
  if(!lModulesDir.exists()) {
    QString lMessage(tr("Directory '%1' doesn't exist!").arg(cLibrariesDirectory));
    emit sEventGenerated(5002004,lMessage, NMessage::ErrorMessage);
    lSuccess = false;
  }
  lSuccess = lSuccess && fProcessModules(lModulesDir);
  lblTotalFiles->setText(QString::number(pFileModel->rowCount()));
  quint64 lTotalSize = 0;
  for(int i = 0; i < pFileModel->rowCount(); i++) {
    lTotalSize += pFileModel->data(pFileModel->index(i,2), Qt::DisplayRole).toULongLong();
  }
  lTotalSize = lTotalSize / (1024 * 1024);
  lblTotalSize->setText(QString::number(lTotalSize));
  return lSuccess;
}

void NCreatePackage::fVerifyLogModel(QStandardItem* rItem) {
  disconnect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
  if(!rItem || ((rItem->column() < 0) || (rItem->column() > 2)))
      return;
  if(rItem->data(Qt::DisplayRole).toString().simplified().isEmpty()) {
    rItem->setData("0", NConstants::StatusRole);
    rItem->setData(QBrush(NConstants::lightRed()), Qt::BackgroundRole);
  }
  else {
    rItem->setData("1", NConstants::StatusRole);
    rItem->setData(QBrush(NConstants::lightBlue()), Qt::BackgroundRole);
  }
  rItem->setData(rItem->data(Qt::DisplayRole).toString(), Qt::UserRole);
  pbtCreatePackage->setEnabled(fCheckUIStatus());
  connect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
}

bool NCreatePackage::fCheckUIStatus() const
{
  if(!fStatus(ledVersion))
    return false;
  if(!fStatus(ledName))
    return false;

  if(pLogModel->rowCount() == 0)
    return false;
  if(pFileModel->rowCount() == 0)
    return false;

  for(int i = 0; i < pLogModel->rowCount(); i++)
  {
    if(pLogModel->item(i,0)->data(NConstants::StatusRole).toString() == "0")
      return false;
    if(pLogModel->item(i,1)->data(NConstants::StatusRole).toString() == "0")
      return false;
    if(pLogModel->item(i,2)->data(NConstants::StatusRole).toString() == "0")
      return false;
  }
  if(pFileModel->property("Status") != QString("1"))
    return false;
  return true;
}

void NCreatePackage::fWritePackageSummary(const QString& lManifestFile) {
  QSettings lManifest(lManifestFile, QSettings::IniFormat);
  lManifest.beginGroup(QString("PackageSummary"));
  lManifest.setValue("Name", cbxSoftware->currentText());
  lManifest.setValue("ReleaseDate", dedDate->date().toString("yyyy-MM-dd"));
  lManifest.setValue("Priority", cbxPriority->currentText());
  lManifest.setValue("VersionName", ledName->text());
  lManifest.setValue("VersionNumber", ledVersion->text());
  lManifest.setValue("UpgradeNotes", txtDescription->toPlainText());
  lManifest.setValue("Platform", cbxPlatform->currentText());
  lManifest.endGroup();
}

void NCreatePackage::fWriteUpgradeLogs(const QString &lManifestFile) {
   QSettings lManifest(lManifestFile, QSettings::IniFormat);
   lManifest.beginGroup(QString("UpgradeLogs"));
   for(int i = 0; i < pLogModel->rowCount(); i++) {
     QString lNumber(QString("%1").arg(i, 3, 10, QChar('0')));
     QString lComponent(pLogModel->data(pLogModel->index(i,0), Qt::DisplayRole).toString());
     QString lType(pLogModel->data(pLogModel->index(i,1), Qt::DisplayRole).toString());
     QString lLog(pLogModel->data(pLogModel->index(i,2), Qt::DisplayRole).toString());
     QString lLogEntry(QString("[%1][%2] - %3").arg(lType).arg(lComponent).arg(lLog));
     lManifest.setValue(lNumber, lLogEntry);
   }
   lManifest.endGroup();
}

void NCreatePackage::fCreatePackage() {
  QString lPackageName(QString("%1_%2_%3").arg(cbxSoftware->currentText()).arg(cbxPlatform->currentText()).arg(ledVersion->text()));
  QString lCreatePackageDirectory = QString("%1/%2").arg(mCreatePackageDirectory).arg(lPackageName);
  if(fCreatePackageDirectory(lCreatePackageDirectory)) {
    qApp->processEvents();
    QDialog* rCompressingDialog = new QDialog(nullptr, Qt::Popup);
    QString lProcessingText(tr("Creating packages, this can take a few seconds ...\n"));
    QLabel* rText = new QLabel(lProcessingText, nullptr);
    QHBoxLayout* rLayout = new QHBoxLayout();
    rLayout->addWidget(rText);
    rCompressingDialog->setLayout(rLayout);
    rCompressingDialog->show();
    qApp->processEvents();

    QDir lCurrentDir;
    if(!(lCurrentDir.mkpath(QString("%1/%2").arg(lCreatePackageDirectory).arg(cExec)) && lCurrentDir.mkpath(QString("%1/%2").arg(lCreatePackageDirectory).arg(cLibrariesDirectory)) && lCurrentDir.mkpath(QString("%1/%2").arg(lCreatePackageDirectory).arg(cModulesDirectory)))) {
      QString lMessage(tr("Package main directories couldn't be created."));
      emit sEventGenerated(5002040,lMessage, NMessage::ErrorMessage);
      return;
    }
    QStringList lExecFiles;
    QStringList lLibDirectories;
    QStringList lModuleDirectories;
    rText->setText(QString("%1%2").arg(lProcessingText).arg(tr("Scanning files and creating temporal directory ... Task (1/6)")));

    QString lManifestFileName(QString("%1/%2.manifest").arg(lCreatePackageDirectory).arg(lPackageName));
    fWritePackageSummary(lManifestFileName);
    fWriteUpgradeLogs(lManifestFileName);
    QSettings lManifest(lManifestFileName, QSettings::IniFormat);

    for(int i = 0; i < pFileModel->rowCount(); i++) {
      QString lRelativeFileDirectory(pFileModel->data(pFileModel->index(i,0), Qt::DisplayRole).toString());
      if(lRelativeFileDirectory.section('/',0,0) == cLibrariesDirectory) {
        QString lCurrentRelativeDir;
        if(lRelativeFileDirectory.split('/').size() >= 4)
          lCurrentRelativeDir = lRelativeFileDirectory.section('/',0,3);
        else
          lCurrentRelativeDir = lRelativeFileDirectory.section('/',0, lRelativeFileDirectory.split('/').size());

        if(!lLibDirectories.contains(lCurrentRelativeDir))
          lLibDirectories << lCurrentRelativeDir;
      }
      if(lRelativeFileDirectory.section('/',0,0) == cModulesDirectory) {
        QString lCurrentRelativeDir;
        if(lRelativeFileDirectory.split('/').size() >= 4)
           lCurrentRelativeDir = lRelativeFileDirectory.section('/',0,3);
        else
           lCurrentRelativeDir = lRelativeFileDirectory.section('/',0, lRelativeFileDirectory.split('/').size());

        if(!lModuleDirectories.contains(lCurrentRelativeDir))
          lModuleDirectories << lCurrentRelativeDir;
      }
      QString lFile(QString("%1/%2").arg(pFileModel->data(pFileModel->index(i,0), Qt::ToolTipRole).toString()).arg(pFileModel->data(pFileModel->index(i,1), Qt::DisplayRole).toString()));
      if(lRelativeFileDirectory.simplified() == ".")
        lExecFiles << lFile;
      QString lTempDirName(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cTempDir).arg(lRelativeFileDirectory));
      QString lTargetFileName(QString("%1/%2").arg(lTempDirName).arg(pFileModel->data(pFileModel->index(i,1), Qt::DisplayRole).toString()));
      qApp->processEvents();
      if(lCurrentDir.mkpath(lTempDirName)) {
        if(!QFile::copy(lFile, lTargetFileName)) {
          QString lMessage(tr("File '%1' couldn't be copied to temporary directory.").arg(lFile));
          emit sEventGenerated(5002039,lMessage, NMessage::ErrorMessage);
          return;
        }
      }
      else {
        QString lMessage(tr("Directory '%1' couldn't be created.").arg(lTempDirName));
        emit sEventGenerated(5002038,lMessage, NMessage::ErrorMessage);
        return;
      }
    }
    rText->setText(QString("%1%2").arg(lProcessingText).arg(tr("Creating package for executables ... Task (2/6)")));
    qApp->processEvents();
    QString lExecFileName(QString("%1_%2_%3_%4.zip").arg(cbxSoftware->currentText()).arg(cExec).arg(cbxPlatform->currentText()).arg(ledVersion->text()));
    JlCompress::compressFiles(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cExec).arg(lExecFileName), lExecFiles);
    lManifest.beginGroup(QString("%1_%2").arg(cHashAlgorithm).arg("ExecHash"));
    lManifest.setValue(lExecFileName, QString(fFileChecksum(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cExec).arg(lExecFileName)).toHex()));
    lManifest.endGroup();

    rText->setText(QString("%1%2").arg(lProcessingText).arg(tr("Creating package for manual download ... Task (3/6)")));
    qApp->processEvents();
    JlCompress::compressDir(QString("%1/%2.zip").arg(lCreatePackageDirectory).arg(lPackageName), QString("%1/%2").arg(lCreatePackageDirectory).arg(cTempDir));
    lManifest.beginGroup(QString("%1_%2").arg(cHashAlgorithm).arg("PackageHash"));
    lManifest.setValue(lPackageName, QString(fFileChecksum(QString("%1/%2.zip").arg(lCreatePackageDirectory).arg(lPackageName)).toHex()));
    lManifest.endGroup();

    rText->setText(QString("%1%2").arg(lProcessingText).arg(tr("Creating library packages ... Task (4/6)")));
    qApp->processEvents();
    lManifest.beginGroup(QString("%1_%2").arg(cHashAlgorithm).arg("LibraryHashes"));
    for(const QString& lLibrary : lLibDirectories) {
      QStringList lLibraryList(lLibrary.split('/'));
      QString lLibraryName(QString("Library_%1_%2_%3.zip").arg(lLibraryList.at(1)).arg(lLibraryList.at(2)).arg(lLibraryList.size() >= 4 ? lLibraryList.at(3) : ledVersion->text()));
      JlCompress::compressDir(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cLibrariesDirectory).arg(lLibraryName), QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cTempDir).arg(lLibrary));
      lManifest.setValue(lLibraryName, QString(fFileChecksum(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cLibrariesDirectory).arg(lLibraryName)).toHex()));
    }
    lManifest.endGroup();

    rText->setText(QString("%1%2").arg(lProcessingText).arg(tr("Creating module packages ... Task (5/6)")));
    qApp->processEvents();
    lManifest.beginGroup(QString("%1_%2").arg(cHashAlgorithm).arg("ModuleHashes"));
    for(const QString& lModule : lModuleDirectories) {
      QStringList lModuleList(lModule.split('/'));
      QString lModuleName(QString("Module_%1_%2_%3.zip").arg(lModuleList.at(1)).arg(lModuleList.at(2)).arg(lModuleList.size() >= 4 ? lModuleList.at(3) : ledVersion->text()));
      JlCompress::compressDir(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cModulesDirectory).arg(lModuleName), QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cTempDir).arg(lModule));
      lManifest.setValue(lModuleName, QString(fFileChecksum(QString("%1/%2/%3").arg(lCreatePackageDirectory).arg(cModulesDirectory).arg(lModuleName)).toHex()));
    }

    rText->setText(QString("%1%2").arg(lProcessingText).arg(tr("Cleaning up ... Task (6/6)")));
    qApp->processEvents();
    lCurrentDir.setPath(QString("%1/%2").arg(lCreatePackageDirectory).arg(cTempDir));
    if(!lCurrentDir.removeRecursively()) {
      QString lMessage(tr("Directory '%1' couldn't be removed.").arg(cTempDir));
      emit sEventGenerated(5002041,lMessage, NMessage::ErrorMessage);
      return;
    }

    QString lMessage(tr("Package '%1' has been created successfully.").arg(lCreatePackageDirectory));
    emit sEventGenerated(5002035,lMessage, NMessage::SuccessMessage);
    rCompressingDialog->deleteLater();
  }
}

QByteArray NCreatePackage::fFileChecksum(const QString& lFileName) {
  QFile lFile(lFileName);
  if(lFile.open(QFile::ReadOnly)) {
    QCryptographicHash lHash(QCryptographicHash::Sha256);
    if(lHash.addData(&lFile)) {
      return lHash.result();
    }
    else {
      QString lMessage(tr("Hash of file '%1' couldn't be calculated.").arg(lFileName));
      emit sEventGenerated(5002042,lMessage, NMessage::ErrorMessage);
    }
  }
  else {
    QString lMessage(tr("File '%1' couldn't be opened.").arg(lFileName));
    emit sEventGenerated(5002043,lMessage, NMessage::ErrorMessage);
  }
  return QByteArray();
}

bool NCreatePackage::fCreatePackageDirectory(const QString& lPath) {
  QDir lCreatePackageDirectory(lPath);
  if(lCreatePackageDirectory.exists()) {
    QMessageBox lMsgBox;
    lMsgBox.setText(tr("Directory '%1' already exists!").arg(lPath));
    lMsgBox.setInformativeText("Do you want to replace it?");
    lMsgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    lMsgBox.setDefaultButton(QMessageBox::Ok);
    int lReturn = lMsgBox.exec();
    if(lReturn == QMessageBox::Ok) {
      if(lCreatePackageDirectory.removeRecursively()) {
        QString lMessage(tr("Directory '%1' has been removed successfully.").arg(lPath));
        emit sEventGenerated(5002030,lMessage, NMessage::SuccessMessage);
      }
      else {
        QString lMessage(tr("Directory '%1' couldn't be removed!").arg(lPath));
        emit sEventGenerated(5002031,lMessage, NMessage::ErrorMessage);
        return false;
      }
    }
    else {
      QString lMessage(tr("Package creation canceled."));
      emit sEventGenerated(5002032,lMessage, NMessage::InfoMessage);
      return false;
    }
  }
  if(lCreatePackageDirectory.mkpath(lPath)) {
    if(lCreatePackageDirectory.mkpath(QString("%1/%2").arg(lPath).arg(cTempDir))) {
      QString lMessage(tr("Directory '%1' and '%2' have been created successfully.").arg(lPath).arg(cTempDir));
      emit sEventGenerated(5002036,lMessage, NMessage::SuccessMessage);
      return true;
    }
    else {
      QString lMessage(tr("Directory '%1' couldn't be created.").arg(cTempDir));
      emit sEventGenerated(5002037,lMessage, NMessage::ErrorMessage);
      return false;
    }
  }
  QString lMessage(tr("Directory '%1' couldn't be created.").arg(lPath));
  emit sEventGenerated(5002034,lMessage, NMessage::ErrorMessage);
  return false;
}

void NCreatePackage::fLoadTables() {
  dedDate->setDate(QDate::currentDate());
  cbxPriority->clear();
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleGreen.png"), tr("Low Priority"),QString("1"));
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleYellow.png"), tr("Medium Priority"),QString("2"));
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleOrange.png"), tr("High Priority"),QString("3"));
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleRed.png"), tr("Critical Priority"),QString("4"));
  fLoadPlatform();
  fLoadSoftware();
}

void NCreatePackage::fLoadSoftware() {
  QFile lSoftwareFile;
  lSoftwareFile.setFileName("Software.ndd");
  if(lSoftwareFile.open(QFile::ReadOnly)) {
    cbxSoftware->clear();
    QTextStream lStream(&lSoftwareFile);
    while(!lStream.atEnd()) {
      QString lLine = lStream.readLine();
      QStringList lRecord = lLine.split("|");
      if(lRecord.size() == 3) {
        cbxSoftware->addItem(lRecord.at(1),lRecord.at(0));
      }
      else {
        QString lMessage(tr("Software.ndd has incorrect number of fields."));
        emit sEventGenerated(5002020, lMessage, NMessage::ErrorMessage);
      }
    }
    lSoftwareFile.close();
  }
  else {
    QString lMessage(tr("Software.ndd file could not be opened! Impossible to create package!"));
    emit sEventGenerated(5002021,lMessage, NMessage::ErrorMessage);
  }
}

void NCreatePackage::fLoadPlatform() {
  QFile lPlatformFile;
  lPlatformFile.setFileName("Platform.ndd");
  if(lPlatformFile.open(QFile::ReadOnly)) {
    cbxPlatform->clear();
    QTextStream lStream(&lPlatformFile);
    while(!lStream.atEnd()) {
      QString lLine = lStream.readLine();
      QStringList lRecord = lLine.split("|");
      if(lRecord.size() == 3) {
        cbxPlatform->addItem(lRecord.at(1),lRecord.at(0));
      }
      else {
        QString lMessage(tr("Platform.ndd has incorrect number of fields."));
        emit sEventGenerated(5002001, lMessage, NMessage::ErrorMessage);
      }
    }
    lPlatformFile.close();
  }
  else {
    QString lMessage(tr("Platform.ndd file could not be opened! Impossible to create package!"));
    emit sEventGenerated(5002002,lMessage, NMessage::ErrorMessage);
  }
}

void NCreatePackage::fAddLog() {
  disconnect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
  QStandardItem* rComponent = new QStandardItem();
  //rComponent->setData("NULL", Qt::UserRole);
  QStandardItem* rType = new QStandardItem();
 // rType->setData(ledCode->text(), Qt::UserRole);
  QStandardItem* rLog = new QStandardItem();

  QList<QStandardItem* > lList;
  lList << rComponent << rType << rLog;
  pLogModel->appendRow(lList);
  fVerifyLogModel(rComponent);
  fVerifyLogModel(rType);
  fVerifyLogModel(rLog);
  connect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
}

void NCreatePackage::fDeleteLog() {
  QModelIndexList lSelection = tvwLog->selectionModel()->selectedIndexes();
  if(!lSelection.count())
    return;
  pLogModel->removeRow(lSelection.at(0).row());
  pbtCreatePackage->setEnabled(fCheckUIStatus());
}

void NCreatePackage::fValidateParameter(const QString& lParameter) {
  QString lCurrentValue = lParameter;
  QLineEdit* lParam = qobject_cast<QLineEdit*> (sender());
  Q_ASSERT(lParam);

  int lPos = 0;
  QValidator::State lState = lParam->validator()->validate(lCurrentValue, lPos);
  if(lState == QValidator::Acceptable)
    fSetStatus(lParam, true);
  else
    fSetStatus(lParam, false);
}
