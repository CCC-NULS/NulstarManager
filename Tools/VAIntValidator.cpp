#include "VAIntValidator.h"

VAIntValidator::VAIntValidator(qint64 min, qint64 max, QObject *parent)
                 : QValidator(parent), _min(min), _max(max)
{
}

QValidator::State VAIntValidator::validate(QString& input, int& /*pos*/) const
{
  bool ok = true;
  if(input.simplified().isEmpty())
    return QValidator::Intermediate;
  if(input.startsWith("0"))
    return QValidator::Invalid;

  qint64 number = input.toLongLong(&ok);
  if(!ok || number > _max)
    return QValidator::Invalid;
  if(number < _min)
    return QValidator::Intermediate;
  return QValidator::Acceptable;
}
