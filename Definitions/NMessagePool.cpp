#include <QObject>

#include "NMessagePool.h"

NMessagePool::NMessagePool()
{
  messageMap[1100001] = NMessage(NMessage::SuccessMessage, QObject::tr("Connection parameters loaded"));
  messageMap[1100002] = NMessage(NMessage::SuccessMessage, QObject::tr("Connection with servers established"));
  messageMap[1100003] = NMessage(NMessage::SuccessMessage, QObject::tr("System objects loaded"));
  messageMap[1100004] = NMessage(NMessage::SuccessMessage, QObject::tr("System objects parents loaded"));
  messageMap[1100005] = NMessage(NMessage::SuccessMessage, QObject::tr("System object types loaded"));
  messageMap[1100006] = NMessage(NMessage::SuccessMessage, QObject::tr("Object inserted"));
  messageMap[1100007] = NMessage(NMessage::SuccessMessage, QObject::tr("Object eliminated"));
  messageMap[1100008] = NMessage(NMessage::SuccessMessage, QObject::tr("Object edited"));
  messageMap[1100009] = NMessage(NMessage::SuccessMessage, QObject::tr("Software eliminated"));
  messageMap[1100010] = NMessage(NMessage::SuccessMessage, QObject::tr("Software list loaded"));
  messageMap[1100011] = NMessage(NMessage::SuccessMessage, QObject::tr("Software types loaded"));
  messageMap[1100012] = NMessage(NMessage::SuccessMessage, QObject::tr("Version types loaded"));
  messageMap[1100013] = NMessage(NMessage::SuccessMessage, QObject::tr("Software register inserted"));
  messageMap[1100014] = NMessage(NMessage::SuccessMessage, QObject::tr("Software register edited"));
  messageMap[1100015] = NMessage(NMessage::SuccessMessage, QObject::tr("Operating system types loaded"));
  messageMap[1100016] = NMessage(NMessage::SuccessMessage, QObject::tr("Regions loaded"));
  messageMap[1100017] = NMessage(NMessage::SuccessMessage, QObject::tr("Software files loaded"));
  messageMap[1100018] = NMessage(NMessage::SuccessMessage, QObject::tr("Files eliminated"));
  messageMap[1100019] = NMessage(NMessage::SuccessMessage, QObject::tr("Client list loaded"));
  messageMap[1100020] = NMessage(NMessage::SuccessMessage, QObject::tr("Client categories loaded"));
  messageMap[1100021] = NMessage(NMessage::SuccessMessage, QObject::tr("Regions loaded"));
  messageMap[1100022] = NMessage(NMessage::SuccessMessage, QObject::tr("Client states loaded"));
  messageMap[1100023] = NMessage(NMessage::SuccessMessage, QObject::tr("Software registers loaded"));
  messageMap[1100024] = NMessage(NMessage::SuccessMessage, QObject::tr("Client inserted"));
  messageMap[1100025] = NMessage(NMessage::SuccessMessage, QObject::tr("Client eliminated"));
  messageMap[1100026] = NMessage(NMessage::SuccessMessage, QObject::tr("Client edited"));
  messageMap[1100027] = NMessage(NMessage::SuccessMessage, QObject::tr("Software policies loaded"));

  messageMap[1200001] = NMessage(NMessage::InfoMessage, QObject::tr("File server connection closed"));
  messageMap[1200002] = NMessage(NMessage::InfoMessage, QObject::tr("File upload canceled by user"));
  messageMap[1200003] = NMessage(NMessage::InfoMessage, QObject::tr("File elimination starting"));

  messageMap[1300001] = NMessage(NMessage::ErrorMessage, QObject::tr("File server connection error"), QObject::tr("Check 'Host' and 'Port' parameters in the right section of the connection module"));
  messageMap[1300002] = NMessage(NMessage::ErrorMessage, QObject::tr("File server authentication error"), QObject::tr("Check 'User' and 'Password' parameters in the right section of the connection module"));
  messageMap[1300003] = NMessage(NMessage::ErrorMessage, QObject::tr("Database connection error"), QObject::tr("Check the parameters in the left section of the connection module"));
  messageMap[1300004] = NMessage(NMessage::ErrorMessage, QObject::tr("Database connection nonexistent"), QObject::tr("Check the parameters in the left section of the connection module"));

  messageMap[1400001] = NMessage(NMessage::BugMessage, QObject::tr("Object query error"));
  messageMap[1400002] = NMessage(NMessage::BugMessage, QObject::tr("Parent objects query error"));
  messageMap[1400003] = NMessage(NMessage::BugMessage, QObject::tr("Object type query error"));
  messageMap[1400004] = NMessage(NMessage::BugMessage, QObject::tr("Object insertion query error"));
  messageMap[1400005] = NMessage(NMessage::BugMessage, QObject::tr("Object elimination query error"));
  messageMap[1400006] = NMessage(NMessage::BugMessage, QObject::tr("Error. Eliminated item not found in the model"));
  messageMap[1400007] = NMessage(NMessage::BugMessage, QObject::tr("Object editing query error"));
  messageMap[1400008] = NMessage(NMessage::BugMessage, QObject::tr("Error. Edited item not found in the model"));
  messageMap[1400009] = NMessage(NMessage::BugMessage, QObject::tr("Software elimination query error"));
  messageMap[1400010] = NMessage(NMessage::BugMessage, QObject::tr("Software query error"));
  messageMap[1400011] = NMessage(NMessage::BugMessage, QObject::tr("Software type search query error"));
  messageMap[1400012] = NMessage(NMessage::BugMessage, QObject::tr("Version type search query error"));
  messageMap[1400013] = NMessage(NMessage::BugMessage, QObject::tr("Software insertion query error"));
  messageMap[1400014] = NMessage(NMessage::BugMessage, QObject::tr("Software edition query error"));
  messageMap[1400015] = NMessage(NMessage::BugMessage, QObject::tr("Software log insertion query error"));
  messageMap[1400016] = NMessage(NMessage::BugMessage, QObject::tr("Sql insertion query error"));
  messageMap[1400017] = NMessage(NMessage::BugMessage, QObject::tr("Software log elimination query error"));
  messageMap[1400018] = NMessage(NMessage::BugMessage, QObject::tr("Sql elimination query error"));
  messageMap[1400019] = NMessage(NMessage::BugMessage, QObject::tr("Operating system query error"));
  messageMap[1400020] = NMessage(NMessage::BugMessage, QObject::tr("Regions query error"));
  messageMap[1400021] = NMessage(NMessage::BugMessage, QObject::tr("Software files query error"));
  messageMap[1400022] = NMessage(NMessage::BugMessage, QObject::tr("Network table not empty"));
  messageMap[1400023] = NMessage(NMessage::BugMessage, QObject::tr("File insertion query"));
  messageMap[1400024] = NMessage(NMessage::BugMessage, QObject::tr("Client query error"));
  messageMap[1400025] = NMessage(NMessage::BugMessage, QObject::tr("Software file elimination query error"));
  messageMap[1400026] = NMessage(NMessage::BugMessage, QObject::tr("Client category query error"));
  messageMap[1400027] = NMessage(NMessage::BugMessage, QObject::tr("Regions query error"));
  messageMap[1400028] = NMessage(NMessage::BugMessage, QObject::tr("Client state query error"));
  messageMap[1400029] = NMessage(NMessage::BugMessage, QObject::tr("Software registers query error"));
  messageMap[1400030] = NMessage(NMessage::BugMessage, QObject::tr("Client insertion query error"));
  messageMap[1400031] = NMessage(NMessage::BugMessage, QObject::tr("Client platform insertion query error"));
  messageMap[1400032] = NMessage(NMessage::BugMessage, QObject::tr("Access insertion query error"));
  messageMap[1400033] = NMessage(NMessage::BugMessage, QObject::tr("Client elimination query error"));
  messageMap[1400034] = NMessage(NMessage::BugMessage, QObject::tr("Last software version query error"));
  messageMap[1400035] = NMessage(NMessage::BugMessage, QObject::tr("Upgrade query error"));
  messageMap[1400036] = NMessage(NMessage::BugMessage, QObject::tr("Software policy query error"));
}
