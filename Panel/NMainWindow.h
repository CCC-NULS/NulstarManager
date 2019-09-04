#ifndef NMAINWINDOW_H
#define NMAINWINDOW_H

#include <NMessagePool.h>
#include <QMainWindow>
#include <QMap>
#include "NulstarManagerVersion.h"

class NAbout;
class NClient;
class NConnector;
class NLog;
class NSoftware;
class NPlatform;
class QAction;
class QCloseEvent;
class QDockWidget;
class QLabel;
class QMdiArea;
class QMdiSubWindow;
class QMenu;
class QToolBar;

class NMainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit NMainWindow(QWidget *parent = nullptr);
    ~NMainWindow();

  protected:
    void closeEvent(QCloseEvent* event);

  private:
    NAbout* pAbout;
//**    WCClient* _Client;
    NMessagePool mMessagePool;
    NLog* pLog;
    NSoftware* pSoftware;
    NPlatform* pPlatform;
    QAction* _about;
    QAction* _cascade;
    QAction* _client;
    QAction* _exit;
    QAction* _log;
    QAction* _software;
    QAction* _platform;
    QAction* _tile;
    QDockWidget* _logDock;
    QMap<QAction*, QMdiSubWindow*> _loadedSubWindows;
    QMdiArea* _centralWidget;
    QMenu* _file;
    QMenu* _help;
    QMenu* _management;
    QMenu* _window;
    QToolBar* _filetb;
    QToolBar* _helptb;
    QToolBar* _managementtb;
    QToolBar* _windowtb;

    void createActions();
    void createConnections();    
    void createDocks();
    void createMdiArea();
    void createMenus();
    void createObjects();
    void createStatusbar();
    void createToolbars();
    void initObjects();
    void moveToCenter(QMdiSubWindow* subWindow);

  private slots:
   // void connectionStatusChanged();
    void showAbout();
    void showClient();
    void showLog(bool show);
    void showMax();
    void showSoftware();
    void showPlatform();
    void fProcessEvent(int code, const QString& message, int type);
};

#endif // NMAINWINDOW_H
