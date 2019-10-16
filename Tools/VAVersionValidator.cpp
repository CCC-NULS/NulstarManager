#include <QVersionNumber>

#include "VAVersionValidator.h"

VAVersionValidator::VAVersionValidator(QObject* rParent)
                  : QValidator(rParent) {

}

QValidator::State VAVersionValidator::validate(QString& lInput, int& /*lPos*/) const {
  if(lInput.simplified().isEmpty())
    return QValidator::Intermediate;
  if(lInput.contains(" "))
    return QValidator::Invalid;
  QVersionNumber lVersion(QVersionNumber::fromString(lInput));
  if(lVersion.isNull())
    return QValidator::Invalid;
  if(lVersion.segmentCount() < 3)
    return QValidator::Intermediate;
  if(lVersion.segmentCount() > 3)
    return QValidator::Invalid;
  if(lVersion.segmentCount() == 3)
    return QValidator::Acceptable;
  return QValidator::Invalid;
}
