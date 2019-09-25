#ifndef VAVERSIONVALIDATOR_H
#define VAVERSIONVALIDATOR_H

#include <QObject>
#include <QString>
#include <QValidator>

#include "Tools.h"

class QRegExpValidator;

class TOOLSSHARED_EXPORT VAVersionValidator : public QValidator
{
  Q_OBJECT

  public:
    VAVersionValidator(QObject *parent = nullptr);

    State validate(QString& lInput, int& lPos) const;

  private:
    QString mVersionNumber;
};

#endif // VAVERSIONVALIDATOR_H
