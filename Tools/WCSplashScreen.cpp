#include <QDesktopWidget>
#include <QSize>
#include <QTimer>

#include "WCSplashScreen.h"

WCSplashScreen::WCSplashScreen(QWidget* parent)
              : QDialog(parent)
{
  setupUi(this);
  moveToCenter();
}

void WCSplashScreen::moveToCenter()
{
  QDesktopWidget* desktop = QApplication::desktop();
  int screenWidth, width;
  int screenHeight, height;
  int x, y;
  QSize windowSize;

  screenWidth = desktop->width(); // get width of screen
  screenHeight = desktop->height(); // get height of screen
  windowSize = size(); // size of our application window
  width = windowSize.width();
  height = windowSize.height();
  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  move ( x, y );
}

void WCSplashScreen::showSplash(int time)
{
  show();
  if(time)
  {
    int totTime = time * 1000;
    QTimer::singleShot(totTime, this, SIGNAL(closing()));
    QTimer::singleShot(totTime, this, SLOT(close()));
  }
}
