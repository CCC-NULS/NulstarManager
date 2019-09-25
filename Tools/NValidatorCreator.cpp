#include <QLineEdit>
#include <QObject>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>

#include "NValidatorCreator.h"
#include "VAHostValidator.h"
#include "VAIntValidator.h"
#include "VAResourceValidator.h"
#include "VAStringValidator.h"
#include "VAVersionValidator.h"

const int _minPort = 1;
const int _maxPort = 32768;

NValidatorCreator::NValidatorCreator()
{
}

QValidator* NValidatorCreator::createValidator(QLineEdit* parent, NConstants::CustomRegExp customRegExp, quint64 min, quint64 max)
{
  QValidator* validator = 0;
  if(customRegExp == NConstants::HostRegExp)
  {
    validator = new VAHostValidator(parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::IdentifierRegExp)
  {
    QString regExpr("^[a-zA-Z0-9][a-zA-Z0-9_.\\-]*");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::IntRegExp)
  {
    validator = new VAIntValidator(min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::MacExp)
  {
    QString regExpr("[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::NameRegExp)
  {
    QString regExpr("^[a-zA-Z0-9áéíóúÑñ\\s_.,\\-]+$");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::PortRegExp)
  {
    validator = new VAIntValidator(_minPort, _maxPort, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::ResourceRegExp)
  {
    validator = new VAResourceValidator(parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::VersionRegExp)
  {
  //  validator = new VAVersionValidator(parent);
    QString regExpr("^(\\d+)\\.(\\d+)\\.(\\d+)");
    validator = new VAStringValidator(regExpr, min, max, parent);

    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == NConstants::SerialNumberExp)
  {
    QString regExpr("[0-9A-Z]{5}\\-[0-9A-Z]{5}\\-[0-9A-Z]{5}\\-[0-9A-Z]{5}\\-[0-9A-Z]{5}");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  return validator;
}
