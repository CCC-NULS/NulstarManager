#include "VAResourceValidator.h"

const int _min = 1;
const int _max = 50;

VAResourceValidator::VAResourceValidator(QObject* parent)
                   : QValidator(parent)
{
  QRegExp regExpr("^[a-zA-Z][a-zA-Z0-9_.@/\\-:]*");
  _regExp = new QRegExpValidator(regExpr, this);
}

QValidator::State VAResourceValidator::validate(QString& input, int& pos) const
{
  if(_regExp->validate(input, pos) == QValidator::Invalid)
    return QValidator::Invalid;
  _url.setUrl(input);
  if((input.size() < _min) || (_regExp->validate(input, pos) == QValidator::Intermediate) || !_url.isValid())
    return QValidator::Intermediate;
  if(input.size() > _max)
    return QValidator::Invalid;

  return QValidator::Acceptable;
}
