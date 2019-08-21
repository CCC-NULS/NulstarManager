#ifndef WCCONSTANTS_H
#define WCCONSTANTS_H

#include <QColor>
#include <QObject>
#include <QString>

#include "Definitions.h"

class WCConstants
{
  public:
    enum CustomRegExp { HostRegExp, IdentifierRegExp, IntRegExp, NameRegExp, PortRegExp, ResourceRegExp, VersionRegExp, SerialNumberExp, MacExp };
    enum CustomRole { StatusRole = Qt::UserRole + 1 };
    enum MessageType { SuccessMessage, InfoMessage, WarningMessage, ErrorMessage, BugMessage };
    enum OperatingSystem { Linux64 = 1, Windows32 = 2, Windows64 = 3 };
    static QColor cyan() { return QColor(96, 177, 207); }
    static QColor darkBlue() { return QColor(12, 12, 90); }
    static QColor darkGreen() { return QColor(3, 102, 73); }
    static QColor darkPurple() { return QColor(135, 11, 162); }
    static QColor darkRed() { return QColor(143, 13, 6); }
    static QColor green() { return QColor(100, 130, 100); }
    static QColor lightBlue() { return QColor(233, 240, 255); }
    static QColor lightRed() { return QColor(251, 223, 223); }
    static QColor orange() { return QColor(234, 185, 14); }
    static QColor white() { return QColor(255, 255, 255); }
    static QString colorToRgbText(const QColor& color);
    static QString configurationFile() { return "PulzarManager.cfg"; }
    static QString defaultDBHost() { return "127.0.0.1"; }
    static QString defaultDBName() { return "PulzarManagerDB"; }
    static QString defaultDBPassword() { return ""; }
    static QString defaultDBPort() { return "3306"; }
    static QString defaultDBUser() { return "PulzarManager"; }
    static QString defaultFSDirectory() { return "Releases"; }
    static QString defaultFSHost() { return "127.0.0.1"; }
    static QString defaultFSPort() { return "21"; }
    static QString defaultFSUser() { return "PulzarManager"; }
    static QString defaultFSPassword() { return ""; }
    static QString defaultMacAny() { return "00:00:00:00:00:00"; }
    static QString defaultMacUnknown() { return "00:00:00:00:00:01"; }
    static QString mainDBConnection() { return "MainConnection"; }
};

#endif // WCCONSTANTS_H
