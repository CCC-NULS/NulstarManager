#include <QApplication>
#include <QColor>
#include <QPixmap>
#include <QPixmapCache>
#include <QString>
#include <QTextCodec>
#include <NConstants.h>
#include <NSplashScreen.h>
#include <JlCompress.h>
#include "NMainWindow.h"
#include "NulstarManagerVersion.h"

int main(int argc, char *argv[])
{
  QApplication managerApp(argc, argv);

  managerApp.addLibraryPath(managerApp.applicationDirPath() + "/SystemPlugins");
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  managerApp.setApplicationName("Nulstar Manager");
  managerApp.setOrganizationDomain("nulstar.com");
  managerApp.setOrganizationName("Nulstar");
  managerApp.setQuitOnLastWindowClosed(false);

  QString style = QString("* { font: 75 8pt \"Cantarell\" }"
                " QGroupBox { font: bold 9pt \" Cantarell\" }"
                " QLineEdit"
                " { font: italic 8pt \" Cantarell\";"
                " color: rgb(160, 160, 160) ;"
                " border: 1px solid gray ;"
                " border-radius: 6px ;"
                " padding: 0 3px; }"
                " QLineEdit:focus { "
                " border: 1px solid rgb%1; }"

                " QDateEdit"
                " { font: italic 8pt \" Cantarell\";"
                " color: rgb(160, 160, 160) ;"
                " border: 1px solid gray ;"
                " border-radius: 6px ;"
                " background-color: rgb%2 ;"
                " padding: 0 3px; }"
                " QDateEdit:focus { "
                " border: 1px solid rgb%1; }"

                " QDateEdit::down-arrow { "
                " image: url(:/Resources/Images/ArrowDown.png);} "
                " QDateEdit::drop-down { "
                " subcontrol-origin: padding; "
                " subcontrol-position: top right; "
                " width: 15px; "
                " border-left-width: 1px;"
                " border-left-color: darkgray; "
                " border-left-style: solid; /* just a single line */ "
                " border-top-right-radius: 6px; /* same radius as the QComboBox */"
                " border-bottom-right-radius: 6px; } "

                " QDateEdit::down-arrow:on { /* shift the arrow when popup is open */"
                " top: 1px; "
                " left: 1px; }"

                " QTextEdit"
                " { font: italic 8pt \" Cantarell\";"
                " color: rgb(160, 160, 160) ;"
                " border: 1px solid gray ;"
                " border-radius: 6px ;"
                " padding: 2px 3px 2px 1px; }"

                " QTextEdit:focus { "
                " border: 1px solid rgb%1; }"

                " QComboBox { "
                " border: 1px solid gray; "
                " border-radius: 6px; "
                " padding: 1px 18px 1px 3px; "
                " min-width: 6em; }"

                " QComboBox:focus { "
                " border: 1px solid rgb%1; }"

                " QComboBox { "
                " background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                " stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, "
                " stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } "
                " QComboBox:!editable:on, QComboBox::drop-down:editable:on { "
                " background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                " stop: 0 #D3D3D3, stop: 0.4 #D8D8D8, "
                " stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1); } "
                " QComboBox:on { /* shift the text when the popup opens */ "
                " padding-top: 3px; "
                " padding-left: 4px; }"
                " QComboBox::drop-down { "
                " subcontrol-origin: padding; "
                " subcontrol-position: top right; "
                " width: 15px; "
                " border-left-width: 1px;"
                " border-left-color: darkgray; "
                " border-left-style: solid; /* just a single line */ "
                " border-top-right-radius: 3px; /* same radius as the QComboBox */"
                " border-bottom-right-radius: 3px; } "

                " QComboBox::down-arrow { "
                " image: url(:/Resources/Images/ArrowDown.png);} "

               " QComboBox::down-arrow:on { /* shift the arrow when popup is open */"
               " top: 1px; "
               " left: 1px; }"
               " QComboBox QAbstractItemView { "
               " border: 1px solid darkgray; "
               " selection-background-color: lightgray; }"

               " QPushButton { "
               " border: 1px solid #8f8f91; "
               " border-radius: 6px; "
               " padding: 3px 5px 2px 5px;} "
               " QPushButton:flat { "
               " border: none; }"
               " QPushButton:!flat:focus { "
               " border: 1px solid rgb%1; }"

               " QPushButton:!flat { "
               " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); }"
               " QPushButton:!flat:pressed { "
               " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }"

               ).arg(NConstants::colorToRgbText(NConstants::orange())).arg(NConstants::colorToRgbText(NConstants::lightBlue()));

  managerApp.setStyleSheet(style);
  JlCompress::extractDir("ww", qApp->applicationDirPath()  );
  NMainWindow panel;
  NSplashScreen splash;
  splash.setWindowFlags(Qt::FramelessWindowHint);
  QPixmap* banner = QPixmapCache::find("Splash");
  splash.setStyleSheet("background-color: rgb(0,0,0)");
  splash.setPixmap(*banner);
  splash.setSoftwareName(managerApp.applicationName());
  splash.setVersion(APP_VERSION);
  splash.setVersionAlias(APP_VERSION_NAME);
  splash.setActionText(QObject::tr("Loading ..."));
  splash.showSplash(3);

  QObject::connect(&splash, SIGNAL(closing()), &panel, SLOT(showMax()));
  return managerApp.exec();
}
