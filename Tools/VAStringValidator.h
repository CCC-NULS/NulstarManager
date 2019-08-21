#ifndef VASTRINGVALIDATOR_H
#define VASTRINGVALIDATOR_H

#include <QObject>
#include <QString>
#include <QValidator>

#include "Tools.h"

class QRegExpValidator;

class VAStringValidator : public QValidator
{
  Q_OBJECT

  public:
    VAStringValidator(const QString& regExpString, qint64 min = 0, qint64 max = 0, QObject *parent = 0);

    State validate(QString& input, int& pos) const;

  private:
    qint64 _max;
    qint64 _min;
    QRegExpValidator* _regExp;
    QString _regExpString;
};

#endif // VASTRINGVALIDATOR_H
