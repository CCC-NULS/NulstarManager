#ifndef WCMAINWINDOW_H
#define WCMAINWINDOW_H

#include <WCMessagePool.h>
#include <QMainWindow>
#include <QMap>

class WCAbout;
class WCClient;
class WCConnector;
class WCLog;
class WCSoftware;
class WCSystemObject;
class QAction;
class QCloseEvent;
class QDockWidget;
class QLabel;
class QMdiArea;
class QMdiSubWindow;
class QMenu;
class QToolBar;

class WCMainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit WCMainWindow(QWidget *parent = 0);
    ~WCMainWindow();

  protected:
    void closeEvent(QCloseEvent* event);

  private:
    WCAbout* _About;
    WCClient* _Client;
    WCConnector* _Connector;
    WCMessagePool _MessagePool;
    WCLog* _Log;
    WCSoftware* _Software;
    WCSystemObject* _SystemObject;
    QAction* _about;
    QAction* _cascade;
    QAction* _client;
    QAction* _connect;
    QAction* _exit;
    QAction* _log;
    QAction* _software;
    QAction* _systemObject;
    QAction* _tile;
    QDockWidget* _logDock;
    QLabel* _connectionStatus;
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
    void connectionStatusChanged();
    void showAbout();
    void showClient();
    void showConnect();
    void showLog(bool show);
    void showMax();
    void showSoftware();
    void showSystemObject();
    void processEvent(int code);
    void processEvent(const QString& message, int type);
};

#endif // WCMAINWINDOW_H
