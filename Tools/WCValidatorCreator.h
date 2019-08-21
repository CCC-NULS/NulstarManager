#ifndef WCVALIDATORCREATOR_H
#define WCVALIDATORCREATOR_H

#include <WCConstants.h>
#include <QString>

#include "Tools.h"

class QLineEdit;
class QObject;
class QValidator;

class WCValidatorCreator
{
  public:
    explicit WCValidatorCreator();

    static QValidator* createValidator(QLineEdit* parent, WCConstants::CustomRegExp customRegExp, quint64 min = 0, quint64 max = 0);
};

#endif // WCVALIDATORCREATOR_H
