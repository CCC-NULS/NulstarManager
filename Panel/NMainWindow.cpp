#include <NAbout.h>
//#include <NClient.h>
//#include <NConnector.h>
#include <NLog.h>
//#include <NSoftware.h>
//#include <NSystemObject.h>
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

#include "NMainWindow.h"

NMainWindow::NMainWindow(QWidget *parent)
            : QMainWindow(parent)
{
  setWindowTitle("Nulstar Manager");
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

NMainWindow::~NMainWindow()
{

}

void NMainWindow::closeEvent(QCloseEvent* event)
{
  event->accept();
  qApp->quit();
}

void NMainWindow::createActions()
{
  _about = new QAction(tr("&About"), this);
  _about->setIcon(QIcon(":/Resources/Images/NulstarManagerLogo.png"));
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

void NMainWindow::createConnections()
{
  connect(_exit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(_about, SIGNAL(triggered()), this, SLOT(showAbout()));
  connect(_cascade, SIGNAL(triggered()), _centralWidget, SLOT(cascadeSubWindows()));
  connect(_client, SIGNAL(triggered()), this, SLOT(showClient()));
  connect(_tile, SIGNAL(triggered()), _centralWidget, SLOT(tileSubWindows()));
  connect(_log, SIGNAL(triggered(bool)), this, SLOT(showLog(bool)));
  connect(_software, SIGNAL(triggered()), this, SLOT(showSoftware()));
  connect(_systemObject, SIGNAL(triggered()), this, SLOT(showSystemObject()));
//  connect(_Client, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
//  connect(_Client, SIGNAL(eventGenerated(QString, int)), this, SLOT(processEvent(QString,int)));
//  connect(_Software, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
//  connect(_Software, SIGNAL(eventGenerated(QString, int)), this, SLOT(processEvent(QString,int)));
//  connect(_SystemObject, SIGNAL(eventGenerated(int)), this, SLOT(processEvent(int)));
}

void NMainWindow::createDocks()
{
  _logDock = new QDockWidget(tr("Logs"), this);
  _logDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
  _logDock->setWidget(_Log);
  _logDock->setFeatures(QDockWidget::DockWidgetMovable);
  addDockWidget(Qt::BottomDockWidgetArea, _logDock);
  _logDock->hide();
}

void NMainWindow::createMdiArea()
{
  _centralWidget = new QMdiArea(this);
  setCentralWidget(_centralWidget);
}

void NMainWindow::createMenus()
{
  _file = menuBar()->addMenu(tr("&File"));
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

void NMainWindow::createObjects()
{
  _About = new NAbout(APP_VERSION, APP_VERSION_NAME);
 // _Client = new NClient();
 // _Connector = new NConnector(); // Parent is attached when action is executed
  _Log = new NLog(this);
//  _Software = new NSoftware();
//  _SystemObject = new NSystemObject();
}

void NMainWindow::createStatusbar()
{
  statusBar()->setSizeGripEnabled(true);
}

void NMainWindow::createToolbars()
{
  _filetb = addToolBar(tr("&File"));
  _filetb->addAction(_exit);
  _filetb->addSeparator();
  _filetb->addAction(_log);

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

void NMainWindow::initObjects()
{
  //_Connector->loadSettings();
}

void NMainWindow::moveToCenter(QMdiSubWindow* subWindow)
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

void NMainWindow::showAbout()
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

void NMainWindow::showClient()
{
/*  if(_loadedSubWindows.contains(_client))
  {
    _loadedSubWindows.value(_client)->show();
    _Client->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_Client, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindonloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_client->text().remove("&"));
    subWindow->setWindowIcon(_client->icon());
    subWindow->show();
    subWindow->resize(580,250);
    _Client->show();
    _loadedSubWindows[_client] = subWindow;
  }
  _Client->loadTable();*/
}

void NMainWindow::showLog(bool show)
{
  _logDock->setVisible(show);
}

void NMainWindow::showMax()
{
  showMaximized();
}

void NMainWindow::showSoftware()
{
 /* if(_loadedSubWindows.contains(_software))
  {
    _loadedSubWindows.value(_software)->show();
    _Software->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_Software, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindonloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_software->text().remove("&"));
    subWindow->setWindowIcon(_software->icon());
    subWindow->show();
    subWindow->resize(580,250);
    _Software->show();
    _loadedSubWindows[_software] = subWindow;
  }
  _Software->loadTable();*/
}

void NMainWindow::showSystemObject()
{
/*  if(_loadedSubWindows.contains(_systemObject))
  {
    _loadedSubWindows.value(_systemObject)->show();
    _SystemObject->show();
  }
  else
  {
    QMdiSubWindow* subWindow = _centralWidget->addSubWindow(_SystemObject, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindonloseButtonHint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    subWindow->setWindowTitle(_systemObject->text().remove("&"));
    subWindow->setWindowIcon(_systemObject->icon());
    subWindow->show();
    subWindow->resize(580,250);
    _SystemObject->show();
    _loadedSubWindows[_systemObject] = subWindow;
  }
  _SystemObject->loadTable();*/
}

void NMainWindow::processEvent(int code)
{
  NMessage message = _MessagePool.message(code);
  statusBar()->showMessage(message.text());
  _Log->appendEntry(code, message);
}

void NMainWindow::processEvent(const QString& message, int type)
{
  NMessage mes(type, message);
  statusBar()->showMessage(message);
  _Log->appendEntry(9000000, mes);
}
