#include <QApplication>
#include <QPixmap>
#include <QPixmapCache>
#include <QtWidgets>

#include "NAbout.h"

NAbout::NAbout(const QString &lVersion, const QString &lVersionName, QWidget *parent)
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
  _splash.setStyleSheet("background-color: rgba(0,0,0)");
 // _splash.setStyleSheet("background-color: rgba(240,255, 240, 180)");
  _splash.setPixmap(banner);
  _splash.setSoftwareName(qApp->applicationName());
  _splash.setVersion(lVersion);
  _splash.setVersionAlias(lVersionName);
  _splash.setActionText(("Copyright (c) 2019"));
}

void NAbout::showSplash()
{
  show();
  _splash.showSplash();
}
