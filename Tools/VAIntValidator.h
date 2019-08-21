#ifndef VAINTVALIDATOR_H
#define VAINTVALIDATOR_H

#include <QObject>
#include <QValidator>

#include "Tools.h"

class VAIntValidator : public QValidator
{
  Q_OBJECT

  public:
    VAIntValidator(qint64 min, qint64 max, QObject *parent = 0);

    State validate(QString& input, int& pos) const;

  private:
    qint64 _min;
    qint64 _max;
};

#endif // VAINTVALIDATOR_H
