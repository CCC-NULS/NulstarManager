#include <WCAbout.h>
#include <WCClient.h>
#include <WCConnector.h>
#include <WCLog.h>
#include <WCSoftware.h>
#include <WCSystemObject.h>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QIcon>
#include <QKeySequence>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QSize>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "WCMainWindow.h"

WCMainWindow::WCMainWindow(QWidget *parent)
            : QMainWindow(parent)
{
  setWindowTitle("Worldcoind Manager");
  setWindowIcon(QIcon(":/Resources/Images/WorldcoinManagerLogo.png"));

  createActions();
  createMdiArea();
  createObjects();
  createConnections();
  createMenus();
  createToolbars();
  createDocks();
  createStatusbar();
  initObjects();

  _log->activate(QAction::Trigger);
}

WCMainWindow::~WCMainWindow()
{

}

void WCMainWindow::closeEvent(QCloseEvent* event)
{
  event->accept();
  qApp->quit();
}

void WCMainWindow::connectionStatusChanged()
{
  if(_Connector->status())
    _connectionStatus->setPixmap(QPixmap(":/Resources/Images/Connected.png").scaled(18,18));
  else
    _connectionStatus->setPixmap(QPixmap(":/Resources/Images/NetworkConnectionOff.png").scaled(18,18));
  //QStringList dbParams = _Connector->dbParams();
  QStringList ftpParams = _Connector->ftpParams();
  _Software->setFtpParams(ftpParams);
}

void WCMainWindow::createActions()
{
  _about = new QAction(tr("&Acerca De"), this);
  _about->setIcon(QIcon(":/Resources/Images/WorldcoinManagerLogo.png"));
  _about->setShortcut(QKeySequence(Qt::Key_F12));
  _about->setStatusTip(tr("Show information about the product"));
  _about->setObjectName("About");

  _cascade = new QAction(tr("&Cascade"), this);
  _cascade->setIcon(QIcon(":/Resources/Images/WindowsCascade.png"));
  _cascade->setShortcut(QKeySequence(Qt::Key_F10));
  _cascade->setStatusTip(tr("Orders windows in cascade"));
  _cascade->setObjectName("Cascade");

  _tile = new QAction(tr("&Tile"), this);
  _tile->setIcon(QIcon(":/Resources/Images/WindowsTile.png"));
  _tile->setShortcut(QKeySequence(Qt::Key_F11));
  _tile->setStatusTip(tr("Tile"));
  _tile->setObjectName("Tile");

  _exit = new QAction(tr("&Exit"), this);
  _exit->setIcon(QIcon(":/Resources/Images/Exit.png"));
  _exit->setShortcut(QKeySequence(Qt::Key_F4));
  _exit->setStatusTip(tr("Exit program"));
  _exit->setObjectName("Exit");

  _connect = new QAction(tr("Co&nnection"), this);
  _connect->setIcon(QIcon(":/Resources/Images/NetworkConnection.png"));
  _connect->setShortcut(QKeySequence(Qt::Key_F5));
  _connect->setStatusTip(tr("Connect to server"));
  _connect->setObjectName("Connect");

  _systemObject = new QAction(tr("&Objects"), this);
  _systemObject->setIcon(QIcon(":/Resources/Images/SystemObjects.png"));
  _systemObject->setShortcut(QKeySequence(Qt::Key_F6));
  _systemObject->setStatusTip(tr("System objects management"));
  _systemObject->setObjectName("SystemObject");

  _software = new QAction(tr("&Software"), this);
  _software->setIcon(QIcon(":/Resources/Images/Software.png"));
  _software->setShortcut(QKeySequence(Qt::Key_F7));
  _software->setStatusTip(tr("Software management"));
  _software->setObjectName("Software");

  _client = new QAction(tr("&Client"), this);
  _client->setIcon(QIcon(":/Resources/Images/Client.png"));
  _client->setShortcut(QKeySequence(Qt::Key_F8));
  _client->setStatusTip(tr("Client Management"));
  _client->setObjectName("Client");

  _log = new QAction(tr("&Log"), this);
  _log->setIcon(QIcon(":/Resources/Images/Log.png"));
  _log->setShortcut(QKeySequence(Qt::Key_F9));
  _log->setStatusTip(tr("Activity Log"));
  _log->setObjectName("Log");
  _log->setCheckable(true);
}

void WCMainWindow::createConnections()
{
  connect(_exit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(_about, SIGNAL(triggered()), this, SLOT(showAbout()));
  connect(_cascade, SIGNAL(triggered()), _centralWidget, SLOT(cascadeSubWindows()));
  connect(_client, SIGNAL(triggered()), this, SLOT(showClient()));
  connect(_tile, SIGNAL(triggered()), _centralWidget, SLOT(tileSubWindows()));
  connect(_log, SIGNAL(triggered(bool)), this, SLOT(showLog(bool)));
  connect(_connect, SIGNAL(triggered()), this, SLOT(showConnect()));
  connect(_software, SIGNAL(triggered()), this, SLOT(showSoftware()));
  connect(_systemObject, SIGNAL(triggered()), this, SLOT(showSystemObject()));
  connect(_Client, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
  connect(_Client, SIGNAL(eventGenerated(QString, int)), this, SLOT(processEvent(QString,int)));
  connect(_Connector, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
  connect(_Connector, SIGNAL(connected()), this, SLOT(connectionStatusChanged()));
  connect(_Connector, SIGNAL(disconnected()), this, SLOT(connectionStatusChanged()));
  connect(_Software, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
  connect(_Software, SIGNAL(eventGenerated(QString, int)), this, SLOT(processEvent(QString,int)));
  connect(_SystemObject, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
}

void WCMainWindow::createDocks()
{
  _logDock = new QDockWidget(tr("Logs"), this);
  _logDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  _logDock->setWidget(_Log);
  _logDock->setFeatures(QDockWidget::DockWidgetMovable);
  addDockWidget(Qt::BottomDockWidgetArea, _logDock);
  _logDock->hide();
}

void WCMainWindow::createMdiArea()
{
  _centralWidget = new QMdiArea(this);
  setCentralWidget(_centralWidget);
}

void WCMainWindow::createMenus()
{
  _file = menuBar()->addMenu(tr("&File"));
  _file->addAction(_connect);
  _file->addAction(_log);
  _file->addSeparator();
  _file->addAction(_exit);

  _management = menuBar()->addMenu(tr("&Management"));
  _management->addAction(_systemObject);
  _management->addAction(_software);
  _management->addAction(_client);

  _window = menuBar()->addMenu(tr("&Window"));
  _window->addAction(_cascade);
  _window->addAction(_tile);

  _help = menuBar()->addMenu(tr("&Help"));
  _help->addAction(_about);
}

void WCMainWindow::createObjects()
{
  _About = new WCAbout();
  _Client = new WCClient();
  _Connector = new WCConnector(); // Parent is attached when action is executed
  _Log = new WCLog(this);
  _Software = new WCSoftware();
  _SystemObject = new WCSystemObject();
}

void WCMainWindow::createStatusbar()
{
  _connectionStatus = new QLabel (this);
  _connectionStatus->setPixmap(QPixmap(":/Resources/Images/NetworkConnectionOff").scaled(18,18));
  _connectionStatus->setAlignment(Qt::AlignRight);

  statusBar()->addPermanentWidget(_connectionStatus);
  statusBar()->setSizeGripEnabled(true);
}

void WCMainWindow::createToolbars()
{
  _filetb = addToolBar(tr("&File"));
  _filetb->addAction(_connect);
  _filetb->addAction(_log);
  _filetb->addSeparator();
  _filetb->addAction(_exit);

  _managementtb = addToolBar(tr("&Management"));
  _managementtb->addAction(_systemObject);
  _managementtb->addAction(_software);
  _managementtb->addAction(_client);

  _windowtb = addToolBar(tr("&Window"));
  _windowtb ->addAction(_cascade);
  _windowtb ->addAction(_tile);

  _helptb = addToolBar(tr("&Help"));
  _helptb->addAction(_about);
}

void WCMainWindow::initObjects()
{
  _Connector->loadSettings();
}

void WCMainWindow::moveToCenter(QMdiSubWindow* subWindow)
{
  Q_ASSERT(subWindow);

  int screenWidth, width;
  int screenHeight, height;
  int x, y;

  QSize windowSize;
  screenWidth = _centralWidget->viewport()->width();
  screenHeight = _centralWidget->viewport()->height();
  windowSize = subWindow->size();
  width = windowSize.width();
  height = windowSize.height();
  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  subWindow->move(x,y);
}

void WCMainWindow::showAbout()
{
  _About->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
  _About->showSplash();

  int screenWidth, width1;
  int screenHeight, height1;
  int x, y;

  QSize windowSize;
  screenWidth = QApplication::desktop()->width();
  screenHeight = QApplication::desktop()->height();
  windowSize = _About->size();
  width1 = windowSize.width();
  height1 = windowSize.height();
  x = (screenWidth - width1) / 2;
  y = (screenHeight - height1) / 2;

  _About->move(x,y);
}

void WCMainWindow::showClient()
{
  if(_loadedSubWindows.contains(_client))
  {
    _loadedSubWindows.value(_client)->show();
    _Client->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_Client, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_client->text().remove("&"));
    subWindow->setWindowIcon(_client->icon());
    subWindow->show();
    subWindow->resize(580,250);
    _Client->show();
    _loadedSubWindows[_client] = subWindow;
  }
  _Client->loadTable();
}

void WCMainWindow::showConnect()
{
  if(_loadedSubWindows.contains(_connect))
  {
    _loadedSubWindows.value(_connect)->show();
    _Connector->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_Connector, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_connect->text().remove("&"));
    subWindow->setWindowIcon(_connect->icon());

    moveToCenter(subWindow);
    subWindow->show();
    _Connector->show();

    subWindow->setMinimumSize(subWindow->size());
    subWindow->setMaximumSize(subWindow->size());
    _loadedSubWindows[_connect] = subWindow;
  }
}

void WCMainWindow::showLog(bool show)
{
  _logDock->setVisible(show);
}

void WCMainWindow::showMax()
{
  showMaximized();
  showConnect();
}

void WCMainWindow::showSoftware()
{
  if(_loadedSubWindows.contains(_software))
  {
    _loadedSubWindows.value(_software)->show();
    _Software->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_Software, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_software->text().remove("&"));
    subWindow->setWindowIcon(_software->icon());
    subWindow->show();
    subWindow->resize(580,250);
    _Software->show();
    _loadedSubWindows[_software] = subWindow;
  }
  _Software->loadTable();
}

void WCMainWindow::showSystemObject()
{
  if(_loadedSubWindows.contains(_systemObject))
  {
    _loadedSubWindows.value(_systemObject)->show();
    _SystemObject->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_SystemObject, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_systemObject->text().remove("&"));
    subWindow->setWindowIcon(_systemObject->icon());
    subWindow->show();
    subWindow->resize(580,250);
    _SystemObject->show();
    _loadedSubWindows[_systemObject] = subWindow;
  }
  _SystemObject->loadTable();
}

void WCMainWindow::processEvent(int code)
{
  WCMessage message = _MessagePool.message(code);
  statusBar()->showMessage(message.text());
  _Log->appendEntry(code, message);
}

void WCMainWindow::processEvent(const QString& message, int type)
{
  WCMessage mes(type, message);
  statusBar()->showMessage(message);
  _Log->appendEntry(9000000, mes);
}
