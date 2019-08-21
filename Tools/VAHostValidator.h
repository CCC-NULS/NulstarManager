#ifndef VAHOSTVALIDATOR_H
#define VAHOSTVALIDATOR_H

#include <QObject>
#include <QHostAddress>
#include <QValidator>

#include "Tools.h"

class QRegExpValidator;

class VAHostValidator : public QValidator
{
  Q_OBJECT

  public:
    VAHostValidator(QObject *parent = 0);

    State validate(QString& input, int& pos) const;

  private:
    QRegExpValidator* _regExp;
    mutable QHostAddress _address;
};

#endif // VAHOSTVALIDATOR_H
