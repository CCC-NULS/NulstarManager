#include <QLineEdit>
#include <QObject>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>

#include "WCValidatorCreator.h"
#include "VAHostValidator.h"
#include "VAIntValidator.h"
#include "VAResourceValidator.h"
#include "VAStringValidator.h"

const int _minPort = 1;
const int _maxPort = 32768;

WCValidatorCreator::WCValidatorCreator()
{
}

QValidator* WCValidatorCreator::createValidator(QLineEdit* parent, WCConstants::CustomRegExp customRegExp, quint64 min, quint64 max)
{
  QValidator* validator = 0;
  if(customRegExp == WCConstants::HostRegExp)
  {
    validator = new VAHostValidator(parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::IdentifierRegExp)
  {
    QString regExpr("^[a-zA-Z0-9][a-zA-Z0-9_.\\-]*");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::IntRegExp)
  {
    validator = new VAIntValidator(min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::MacExp)
  {
    QString regExpr("[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}\\:[0-9A-Z]{2}");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::NameRegExp)
  {
    QString regExpr("^[a-zA-Z0-9áéíóúÑñ\\s_.,\\-]+$");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::PortRegExp)
  {
    validator = new VAIntValidator(_minPort, _maxPort, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::ResourceRegExp)
  {
    validator = new VAResourceValidator(parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::VersionRegExp)
  {
    QString regExpr("[0-9]{2}\\.[0-9]{2}\\.[0-9]{2}");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  if(customRegExp == WCConstants::SerialNumberExp)
  {
    QString regExpr("[0-9A-Z]{5}\\-[0-9A-Z]{5}\\-[0-9A-Z]{5}\\-[0-9A-Z]{5}\\-[0-9A-Z]{5}");
    validator = new VAStringValidator(regExpr, min, max, parent);
    parent->setValidator(validator);
    return validator;
  }
  return validator;
}
