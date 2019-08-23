#include "NMessage.h"

NMessage::NMessage(int type, const QString& text, const QString& helpText)
         : _type(type), _helpText(helpText), _text(text)
{
}
