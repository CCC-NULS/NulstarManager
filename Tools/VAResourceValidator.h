#ifndef VARESOURCEVALIDATOR_H
#define VARESOURCEVALIDATOR_H

#include <QObject>
#include <QUrl>
#include <QValidator>

#include "Tools.h"

class QRegExpValidator;

class TOOLSSHARED_EXPORT VAResourceValidator : public QValidator
{
  Q_OBJECT

  public:
    VAResourceValidator(QObject *parent = 0);

    State validate(QString& input, int& pos) const;

  private:
    QRegExpValidator* _regExp;
    mutable QUrl _url;
};

#endif // VARESOURCEVALIDATOR_H
