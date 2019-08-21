#ifndef WCABOUT_H
#define WCABOUT_H

#include <WCSplashScreen.h>
#include <QObject>
#include <QWidget>

#include "About.h"

class WCAbout : public QWidget
{
  Q_OBJECT

  public:
    explicit WCAbout(QWidget* parent = 0);

  private:
    WCSplashScreen _splash;

  public slots:
    void showSplash();
};

#endif
