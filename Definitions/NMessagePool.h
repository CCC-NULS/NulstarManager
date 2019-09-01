#ifndef NMESSAGEPOOL_H
#define NMESSAGEPOOL_H

#include <QMap>
#include <QString>

#include "Definitions.h"
#include "NMessage.h"

class DEFINITIONSSHARED_EXPORT NMessagePool
{
  public:
    explicit NMessagePool();
    const NMessage message(int code) const { return messageMap.value(code); }
    int messageType(int code) const { return messageMap.value(code).type(); }
    QString messageText(int code) const { return messageMap.value(code).text(); }
    QString messageHelpText(int code) const { return messageMap.value(code).helpText(); }

  private:
    QMap<int, NMessage> messageMap; // Code, Message
};

#endif // NMESSAGEPOOL_H
