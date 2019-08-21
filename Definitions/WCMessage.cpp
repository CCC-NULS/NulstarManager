#include "WCMessage.h"

WCMessage::WCMessage(int type, const QString& text, const QString& helpText)
         : _type(type), _helpText(helpText), _text(text)
{
}
