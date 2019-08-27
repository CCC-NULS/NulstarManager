#ifndef NABOUT_H
#define NABOUT_H

#include <NSplashScreen.h>
#include <QObject>
#include <QWidget>

#include "About.h"

class NAbout : public QWidget
{
  Q_OBJECT

  public:
    explicit NAbout(const QString& lVersion, const QString& lVersionName, QWidget* parent = nullptr);

  private:
    NSplashScreen _splash;

  public slots:
    void showSplash();
};

#endif
