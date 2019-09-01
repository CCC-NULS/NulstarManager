#ifndef NVALIDATORCREATOR_H
#define NVALIDATORCREATOR_H

#include <NConstants.h>
#include <QString>

#include "Tools.h"

class QLineEdit;
class QObject;
class QValidator;

class TOOLSSHARED_EXPORT NValidatorCreator
{
  public:
    explicit NValidatorCreator();

    static QValidator* createValidator(QLineEdit* parent, NConstants::CustomRegExp customRegExp, quint64 min = 0, quint64 max = 0);
};

#endif // NVALIDATORCREATOR_H
