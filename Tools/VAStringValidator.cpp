#include "VAStringValidator.h"

VAStringValidator::VAStringValidator(const QString& regExpString, qint64 min, qint64 max, QObject *parent)
                 : QValidator(parent), _max(max), _min(min), _regExpString(regExpString)
{
  QRegExp regExpr(regExpString);
  _regExp = new QRegExpValidator(regExpr, this);
}

QValidator::State VAStringValidator::validate(QString& input, int& pos) const
{
  if(_regExp->validate(input, pos) == QValidator::Invalid)
    return QValidator::Invalid;
  if((input.size() < _min) || (_regExp->validate(input, pos) == QValidator::Intermediate))
    return QValidator::Intermediate;
  if((input.size() > _max))
    return QValidator::Invalid;

  return QValidator::Acceptable;
}
