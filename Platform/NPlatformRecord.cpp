#include <NConstants.h>
#include <NValidatorCreator.h>
#include <QColor>
#include <QLineEdit>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QValidator>

#include "NPlatformRecord.h"

const int _minIdNameSize = 3;
const int _maxIdNameSize = 50;

NPlatformRecord::NPlatformRecord(QWidget* parent)
               : QDialog(parent)
{
  setupUi(this);
  _addingRecord = true;
  configUi();
  createValidators();
  createConnections();
}

bool NPlatformRecord::checkUIStatus() const
{
  QList<QLineEdit*> params = findChildren<QLineEdit*> ();
  foreach(QLineEdit* param, params)
    if(!status(param))
      return false;
  return true;
}

void NPlatformRecord::configUi()
{
  QColor color = NConstants::lightBlue();
  txtDescription->setProperty("Status", true);
  txtDescription->setStyleSheet(QString("background-color: rgb%1").arg(NConstants::colorToRgbText(color)));
}

void NPlatformRecord::createConnections()
{
  connect(ledCode, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(ledName, SIGNAL(textChanged(QString)), this, SLOT(validateParameter(QString)));
  connect(pbtCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(pbtAccept, SIGNAL(clicked()), this, SLOT(saveRecord()));
}

void NPlatformRecord::createValidators()
{
  NValidatorCreator::createValidator(ledCode, NConstants::IntRegExp, 1, Q_INT64_C(999999999));
  NValidatorCreator::createValidator(ledName, NConstants::IdentifierRegExp, _minIdNameSize, _maxIdNameSize);

  fSetStatus(ledCode, false);
  fSetStatus(ledName, false);
}

void NPlatformRecord::editRecord(const QStringList& values)
{
  ledCode->setEnabled(false);
  ledCode->setText(values.at(0));
  ledName->setText(values.at(1));
  txtDescription->setText(values.at(2));
  _addingRecord = false;
}

void NPlatformRecord::saveRecord()
{
  QStringList lValues;
  lValues << ledCode->text() << ledName->text() << txtDescription->toPlainText();
  if(_addingRecord)
    emit sObjectInserted(lValues);
  else
    emit sObjectEdited(lValues);
  accept();
}

void NPlatformRecord::fSetDefault() {
  ledCode->setEnabled(true);
  ledCode->clear();
  ledName->clear();
  txtDescription->clear();
  _addingRecord = true;
}

void NPlatformRecord::fSetStatus(QLineEdit* lineEdit, bool status)
{
  QColor color;
  if(status)
  {
    color = NConstants::lightBlue();
    lineEdit->setProperty("Status", true);
  }
  else
  {
    color = NConstants::lightRed();
    lineEdit->setProperty("Status", false);
  }
  lineEdit->setStyleSheet(QString("background-color: rgb%1").arg(NConstants::colorToRgbText(color)));
  pbtAccept->setEnabled(checkUIStatus());
}

void NPlatformRecord::validateParameter(const QString& text)
{
  QString currentValue = text;
  QLineEdit* param = qobject_cast<QLineEdit*> (sender());
  Q_ASSERT(param);

  int pos = 0;
  QValidator::State state = param->validator()->validate(currentValue, pos);
  if( state == QValidator::Acceptable)
    fSetStatus(param, true);
  else
    fSetStatus(param, false);
}
