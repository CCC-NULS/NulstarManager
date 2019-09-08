#ifndef NCREATEPACKAGE_H
#define NCREATEPACKAGE_H

#include <QFile>
#include <QWidget>
#include <NMessage.h>

#include "CreatePackage.h"
#include "ui_NCreatePackage.h"

class QSortFilterProxyModel;
class QStandardItemModel;

class CREATEPACKAGESHARED_EXPORT NCreatePackage : public QWidget, private Ui::uiCreatePackage
{
  Q_OBJECT

  public:
    explicit NCreatePackage(QWidget* parent = nullptr);
    void fCreateModel();

  protected:
    void fLoadPlatform();

  private:
    QStandardItemModel* pFileModel;
    QStandardItemModel* pLogModel;
    QSortFilterProxyModel* pProxyModelFiles;
    QSortFilterProxyModel* pProxyModelLog;
    QFile mCreatePackageFile;

  public slots:
    void fAddLog();
    void fDeleteLog();
    void fLoadFiles();
    void fLoadTables();

  signals:
    void sEventGenerated(int lCode,QString lMessage,int lType);
};

#endif // NCREATEPACKAGE_H
