#ifndef WCMESSAGEPOOL_H
#define WCMESSAGEPOOL_H

#include <QMap>
#include <QString>

#include "Definitions.h"
#include "WCMessage.h"

class WCMessagePool
{
  public:
    explicit WCMessagePool();
    const WCMessage message(int code) const { return messageMap.value(code); }
    int messageType(int code) const { return messageMap.value(code).type(); }
    QString messageText(int code) const { return messageMap.value(code).text(); }
    QString messageHelpText(int code) const { return messageMap.value(code).helpText(); }

  private:
    QMap<int, WCMessage> messageMap; // Code, Message
};

#endif // WCMESSAGEPOOL_H
