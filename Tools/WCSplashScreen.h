#ifndef WCSPLASHSCREEN_H
#define WCSPLASHSCREEN_H

#include <QDialog>
#include <QPixmap>
#include <QString>

#include "Tools.h"
#include "ui_WCSplashScreen.h"

class  WCSplashScreen :  public QDialog, Ui::uiSplashScreen
{
  Q_OBJECT

  public:
    explicit WCSplashScreen(QWidget* parent = 0);

    void moveToCenter();
    void setPixmap(const QPixmap& image) { lblSplash->setPixmap(image); }
    void setSoftwareName(const QString& name) { lblSoftwareName->setText(name); }
    void setVersion(const QString& version) { lblVersion->setText(version); }
    void setVersionAlias(const QString& alias) { lblVersionAlias->setText(alias); }
    void setActionText(const QString& actionText) { lblAction->setText(actionText); }
    void setSystemObject(const QString& systemObject) { lblSystemObject->setText(systemObject); }
    void showSplash(int time = 0); // Tiempo en segundos

  signals:
    void closing();
};

#endif // WCSPLASHSCREEN_H
