#include <QApplication>
#include <QPixmap>
#include <QPixmapCache>
#include <QtWidgets>
#include <WCVersion.h>

#include "WCAbout.h"

WCAbout::WCAbout(QWidget *parent)
       : QWidget(parent)
{
  setMinimumWidth(_splash.width());
  setMinimumHeight(_splash.height());
  setMaximumWidth(_splash.width());
  setMaximumHeight(_splash.height());

  QPixmap banner(":/Resources/Images/SplashScreen.png");
  QPixmapCache::insert("Splash", banner);

  _splash.setParent(this);
  _splash.move(0,0);
  _splash.setStyleSheet("background-color: rgba(240,255, 240, 180)");
  _splash.setPixmap(banner);
  _splash.setSoftwareName(qApp->applicationName());
  _splash.setVersion(WCVersion::worldcoinManagerVersion());
  _splash.setVersionAlias(WCVersion::worldcoinManagerAlias());
  _splash.setActionText(("Copyright (c) 2014"));
}

void WCAbout::showSplash()
{
  show();
  _splash.showSplash();
}
