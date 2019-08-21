#include <QChar>

#include "VAHostValidator.h"

const int _min = 1;
const int _max = 50;

VAHostValidator::VAHostValidator(QObject* parent)
                   : QValidator(parent)
{
  QRegExp regExpr("^[a-z0-9.-]+");
  _regExp = new QRegExpValidator(regExpr, this);
}

QValidator::State VAHostValidator::validate(QString& input, int& pos) const
{
  if(_regExp->validate(input, pos) == QValidator::Invalid)
    return QValidator::Invalid;
  if(input.size() && input.at(0).isDigit())
  {
    if((input.size() < _min) || (_regExp->validate(input, pos) == QValidator::Intermediate) || !_address.setAddress(input))
      return QValidator::Intermediate;
  }
  else
  {
    if((input.size() < _min) || (_regExp->validate(input, pos) == QValidator::Intermediate) )
      return QValidator::Intermediate;
  }
  if((input.size() > _max))
    return QValidator::Invalid;

  return QValidator::Acceptable;
}
