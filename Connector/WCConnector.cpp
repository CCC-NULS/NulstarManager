#include <WCConstants.h>
#include <WCValidatorCreator.h>
#include <QColor>
#include <QEventLoop>
#include <QIcon>
#include <QLineEdit>
#include <QList>
#include <QSettings>
#include <QSqlDatabase>
#include <QString>
#include <QUrl>
#include <QValidator>

#include "WCConnector.h"
#include <qftp.h>

const int _minIdNameSize = 1;
const int _maxIdNameSize = 25;
const QString _dbDriver = QString("QMYSQL");

WCConnector::WCConnector(QWidget* parent)
           : QWidget(parent), _status(false), _ftpClose(0), _ftpConnectId(0), _ftpLoginId(0)
{
  setupUi(this);
  createValidators();
  createObjects();
  createConnections();

  _mainConnection = WCConstants::mainDBConnection();
}

bool WCConnector::checkUIStatus() const
{
  QList<QLineEdit*> params = findChildren<QLineEdit*> ();
  foreach(QLineEdit* param, params)
    if(!status(param))
      return false;
  return true;
}

void WCConnector::createConnections()
{
  connect(ledDB, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledMainHost, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledMainPort, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledMainUser, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledMainPassword, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledRoot, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledSoftwareHost, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledSoftwarePort, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledSoftwareUser, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledSoftwarePassword, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(pbtConnect, SIGNAL(clicked()), this, SLOT(establishConnection()));
 // connect(_ftp, SIGNAL(stateChanged(int)), this, SLOT(processFtpState(int)));
  connect(_ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(processFtpCommandResult(int,bool)));
/*  connect(ftp, SIGNAL(listInfo(QUrlInfo)),
          this, SLOT(addToList(QUrlInfo)));
  connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
          this, SLOT(updateDataTransferProgress(qint64,qint64)));*/

// Inicialmente no hay una verificacion del valor hasta teclear algo, asi que debemos poner el status en false
  setStatus(ledDB, false);
  setStatus(ledMainHost, false);
  setStatus(ledMainPort, false);
  setStatus(ledMainUser, false);
  setStatus(ledMainPassword, false);
  setStatus(ledRoot, false);
  setStatus(ledSoftwareHost, false);
  setStatus(ledSoftwarePort, false);
  setStatus(ledSoftwareUser, false);
  setStatus(ledSoftwarePassword, false);
}

void WCConnector::createObjects()
{
  _ftp = new QFtp(this);
}

void WCConnector::createValidators()
{
  WCValidatorCreator::createValidator(ledDB, WCConstants::IdentifierRegExp,_minIdNameSize,_maxIdNameSize);
  WCValidatorCreator::createValidator(ledMainHost, WCConstants::HostRegExp);
  WCValidatorCreator::createValidator(ledMainPort, WCConstants::PortRegExp);
  WCValidatorCreator::createValidator(ledMainUser, WCConstants::IdentifierRegExp,_minIdNameSize,_maxIdNameSize);
  WCValidatorCreator::createValidator(ledMainPassword, WCConstants::IdentifierRegExp,_minIdNameSize,_maxIdNameSize);
  WCValidatorCreator::createValidator(ledRoot, WCConstants::ResourceRegExp);
  WCValidatorCreator::createValidator(ledSoftwareHost, WCConstants::HostRegExp);
  WCValidatorCreator::createValidator(ledSoftwarePort, WCConstants::PortRegExp);
  WCValidatorCreator::createValidator(ledSoftwareUser, WCConstants::IdentifierRegExp,_minIdNameSize,_maxIdNameSize);
  WCValidatorCreator::createValidator(ledSoftwarePassword, WCConstants::IdentifierRegExp,_minIdNameSize,_maxIdNameSize);
}

QStringList WCConnector::dbParams()
{
  QStringList params;
  params << ledDB->text() << ledMainHost->text() << ledMainPort->text() << ledMainUser->text() << ledMainPassword->text();
  return params;
}

void WCConnector::establishConnection()
{
  setEnabled(false);

  _ftpClose = _ftp->close();
  _status = false;

  QEventLoop closeFtp;
  connect(this, SIGNAL(disconnected()), &closeFtp, SLOT(quit()));
  closeFtp.exec();

  _ftpConnectId = _ftp->connectToHost(ledSoftwareHost->text(), ledSoftwarePort->text().toInt());
  _ftpLoginId = _ftp->login(ledSoftwareUser->text(), ledSoftwarePassword->text());

  QSqlDatabase db;
  if(QSqlDatabase::contains(_mainConnection))
    db = QSqlDatabase::database(_mainConnection);
  else
    db = QSqlDatabase::addDatabase(_dbDriver,_mainConnection);
  db.setHostName(ledMainHost->text());
  db.setDatabaseName(ledDB->text());
  db.setUserName(ledMainUser->text());
  db.setPassword(ledMainPassword->text());
  db.setPort(ledMainPort->text().toInt());
  db.open();

  QEventLoop eventLoop;
  connect(this, SIGNAL(connected()), &eventLoop, SLOT(quit()));
  connect(this, SIGNAL(disconnected()), &eventLoop, SLOT(quit()));
  eventLoop.exec();
  setEnabled(true);
}

QStringList WCConnector::ftpParams()
{
  QStringList params;
  params << ledRoot->text() << ledSoftwareHost->text() << ledSoftwarePort->text() << ledSoftwareUser->text() << ledSoftwarePassword->text();
  return params;
}

void WCConnector::loadSettings()
{
  QSettings settings(WCConstants::configurationFile(), QSettings::IniFormat);
  settings.beginGroup("DB");
  ledDB->setText(settings.value("DBName", WCConstants::defaultDBName()).toString());
  ledMainHost->setText(settings.value("DBHost", WCConstants::defaultDBHost()).toString());
  ledMainPort->setText(settings.value("DBPort", WCConstants::defaultDBPort()).toString());
  ledMainUser->setText(settings.value("DBUser", WCConstants::defaultDBUser()).toString());
  ledMainPassword->setText(settings.value("DBPassword", WCConstants::defaultDBPassword()).toString());
  settings.endGroup();
  settings.beginGroup("FileServer");
  ledRoot->setText(settings.value("FSDirectory", WCConstants::defaultFSDirectory()).toString());
  ledSoftwareHost->setText(settings.value("FSHost", WCConstants::defaultFSHost()).toString());
  ledSoftwarePort->setText(settings.value("FSPort", WCConstants::defaultFSPort()).toString());
  ledSoftwareUser->setText(settings.value("FSUser", WCConstants::defaultFSUser()).toString());
  ledSoftwarePassword->setText(settings.value("FSPassword", WCConstants::defaultFSPassword()).toString());
  settings.endGroup();

  emit eventGenerated(1100001);
}

void WCConnector::processFtpCommandResult(int id, bool error)
{
  if(id == _ftpClose)
  {
    pbtConnect->setIcon(QIcon(":/Resources/Images/Reconnect.png"));
    emit disconnected();
    _status = false;
    emit eventGenerated(1200001);
    return;
  }
  if(id == _ftpConnectId)
  {
    if(error)
    {
      _status = false;
      emit eventGenerated(1300001);
    }
  }
  if(id == _ftpLoginId)
  {
    if(error)
    {
      _status = false;
      pbtConnect->setIcon(QIcon(":/Resources/Images/Reconnect.png"));
      emit disconnected();
      emit eventGenerated(1300002);
    }
    else
    {
      QSqlDatabase db = QSqlDatabase::database(_mainConnection);
      if(db.isOpen())
      {
        _status = true;
        pbtConnect->setIcon(QIcon(":/Resources/Images/Connected.png"));
        saveSettings();
        emit connected();
        emit eventGenerated(1100002);
      }
      else
      {
        _status = false;
        pbtConnect->setIcon(QIcon(":/Resources/Images/Reconnect.png"));
        emit disconnected();
        emit eventGenerated(1300003);
      }
    }
    return;
  }
}

void WCConnector::saveSettings()
{
  QSettings settings(WCConstants::configurationFile(), QSettings::IniFormat);
  settings.beginGroup("DB");
  settings.setValue("DBName", ledDB->text());
  settings.setValue("DBHost", ledMainHost->text());
  settings.setValue("DBPort", ledMainPort->text());
  settings.setValue("DBUser", ledMainUser->text());
  settings.setValue("DBPassword", ledMainPassword->text());
  settings.endGroup();
  settings.beginGroup("FileServer");
  settings.setValue("FSDirectory", ledRoot->text());
  settings.setValue("FSHost", ledSoftwareHost->text());
  settings.setValue("FSPort", ledSoftwarePort->text());
  settings.setValue("FSUser", ledSoftwareUser->text());
  settings.setValue("FSPassword", ledSoftwarePassword->text());
  settings.endGroup();
}

void WCConnector::setStatus(QLineEdit* lineEdit, bool status)
{
  QColor color;
  if(status)
  {
    color = WCConstants::lightBlue();
    lineEdit->setProperty("Status", true);
  }
  else
  {
    color = WCConstants::lightRed();
    lineEdit->setProperty("Status", false);
  }
  lineEdit->setStyleSheet(QString("background-color: rgb%1").arg(WCConstants::colorToRgbText(color)));
  pbtConnect->setEnabled(checkUIStatus());
}

void WCConnector::validateParameter(const QString& text)
{
  QString currentValue = text;
  QLineEdit* param = qobject_cast<QLineEdit*> (sender());
  Q_ASSERT(param);

  int pos = 0;
  QValidator::State state = param->validator()->validate(currentValue, pos);
  if( state == QValidator::Acceptable)
    setStatus(param, true);
  else
    setStatus(param, false);
}

