#ifndef NMESSAGE_H
#define NMESSAGE_H

#include <QString>

#include "Definitions.h"

class DEFINITIONSSHARED_EXPORT NMessage
{
  public:            
    NMessage(int type = 0, const QString& text = QString(), const QString& helpText = QString());
    enum MessageType { InfoMessage, SuccessMessage, ErrorMessage, WarningMessage, BugMessage };
    int type() const { return _type; }
    QString text() const { return _text; }
    QString helpText() const { return _helpText; }

  private:
    int _type;
    QString _helpText;
    QString _text;
};

#endif // NMESSAGE_H
