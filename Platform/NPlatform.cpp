#include <QDesktopWidget>
#include <QDate>
#include <QFont>
#include <QList>
#include <QMessageBox>
#include <QModelIndexList>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <NConstants.h>

#include "NPlatform.h"
#include "NPlatformRecord.h"

NPlatform::NPlatform(QWidget* parent)
         : QWidget(parent) {
  setupUi(this);
  pRecord = new NPlatformRecord(this);
  fCreateModel();  
  mPlatformFile.setFileName("Platform.ndd");
}


void NPlatform::fCreateModel() {
  QStringList lHeaders;
  lHeaders << tr("Code") << tr("OS Name") << tr("Description");
  pPlatformModel = new QStandardItemModel(0, 3, this);
  pPlatformModel->setHorizontalHeaderLabels(lHeaders);

  pProxyModel = new QSortFilterProxyModel(this);
  pProxyModel->setSortRole(Qt::DisplayRole);
  pProxyModel->setDynamicSortFilter(true);
  pProxyModel->setSourceModel(pPlatformModel);

  tvwPlatform->setSortingEnabled(true);
  tvwPlatform->setModel(pProxyModel);
  tvwPlatform->sortByColumn(0, Qt::AscendingOrder);
  tvwPlatform->setColumnWidth(0,110);
  tvwPlatform->setColumnWidth(1,110);

  connect(tbtRefresh, SIGNAL(clicked()), this, SLOT(fLoadModel()));
  connect(tbtAdd, SIGNAL(clicked()), this, SLOT(fAddRecord()));
  connect(tbtEdit, SIGNAL(clicked()), this, SLOT(fEditRecord()));
  connect(tbtDelete, SIGNAL(clicked()), this, SLOT(fDeleteRecord()));
  connect(pRecord, SIGNAL(sEventGenerated(int,QString,int)), this, SIGNAL(sEventGenerated(int,QString,int)));
  connect(pRecord, SIGNAL(sObjectEdited(QStringList)), this, SLOT(fSaveEditRecord(QStringList)));
  connect(pRecord, SIGNAL(sObjectInserted(QStringList)), this, SLOT(fSaveAddRecord(QStringList)));
  connect(tvwPlatform, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fEditRecord()));
}

void NPlatform::fLoadModel() {
  if(mPlatformFile.open(QFile::ReadOnly)) {
    pPlatformModel->removeRows(0, pPlatformModel->rowCount());
    QTextStream lStream(&mPlatformFile);
    while(!lStream.atEnd()) {
      QString lLine = lStream.readLine();
      QStringList lRecord = lLine.split("|");
      if(lRecord.size() == 3)
        fInsertRecordOnTable(lRecord);
      else {
        QString lMessage(tr("Platform.ndd has incorrect number of fields."));
        emit sEventGenerated(5000001, lMessage, NMessage::ErrorMessage);
      }
    }
    mPlatformFile.close();
  }
  else {
    QString lMessage(tr("Platform.ndd file could not be opened! Impossible to create package!"));
    emit sEventGenerated(5000002,lMessage, NMessage::ErrorMessage);
  }
}

bool NPlatform::fSaveModel() {
  if(mPlatformFile.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream lStream(&mPlatformFile);
    for(int i = 0; i < pPlatformModel->rowCount(); i++) {
      QString lLine = QString("%1|%2|%3").arg(pPlatformModel->data(pPlatformModel->index(i,0), Qt::EditRole).toString()).arg(pPlatformModel->data(pPlatformModel->index(i,1), Qt::EditRole).toString()).arg(pPlatformModel->data(pPlatformModel->index(i,2), Qt::EditRole).toString());
      lStream << lLine << endl;
    }
    mPlatformFile.close();
  }
  else
    return false;
  return true;
}

void NPlatform::fDeleteRecord() {
  QModelIndexList lSelection = tvwPlatform->selectionModel()->selectedIndexes();
  if(!lSelection.count())
    return;
  QString lName(lSelection.at(1).data(Qt::DisplayRole).toString());

  QMessageBox msgBox;
  msgBox.setText(tr("Delet confirmation."));
  msgBox.setInformativeText(tr("Are you sure you want to delete platform '%1'").arg(lName));
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  QGridLayout* layout = static_cast<QGridLayout*>(msgBox.layout());
  layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
  int lRet = msgBox.exec();
  if(lRet == QMessageBox::Cancel)
    return;
  if(pPlatformModel->removeRow(lSelection.at(0).row())) {
    if(fSaveModel()) {
      QString lMessage(tr("Platform '%1' deleted successfully!").arg(lName));
      emit sEventGenerated(1000003,lMessage, NMessage::SuccessMessage);
    }
    else {
      QString lMessage(tr("Failed to delete platform '%1'").arg(lName));
      emit sEventGenerated(5000006,lMessage, NMessage::ErrorMessage);
    }
  }
  else {
    QString lMessage(tr("Platform model does not contain selected row!"));
    emit sEventGenerated(4000001,lMessage, NMessage::BugMessage);
  }
}

void NPlatform::fAddRecord() {
  fMoveToCenter(pRecord);
  pRecord->setWindowTitle(tr("Add platform"));
  pRecord->fSetDefault();
  pRecord->exec();
}

void NPlatform::fEditRecord() {
  fMoveToCenter(pRecord);
  pRecord->setWindowTitle(tr("Edit platform"));
  QString lCode(pPlatformModel->data(pPlatformModel->index(tvwPlatform->currentIndex().row(), 0), Qt::EditRole).toString());
  QString lName(pPlatformModel->data(pPlatformModel->index(tvwPlatform->currentIndex().row(), 1), Qt::EditRole).toString());
  QString lDesc(pPlatformModel->data(pPlatformModel->index(tvwPlatform->currentIndex().row(), 2), Qt::EditRole).toString());
  QStringList lValues( {lCode, lName, lDesc} );
  pRecord->editRecord(lValues);
  pRecord->exec();
}

void NPlatform::fSaveAddRecord(const QStringList& lValues) {
  QList<QStandardItem*> lRepeatedItems = pPlatformModel->findItems(lValues.at(0), Qt::MatchExactly, 0);
  if(lRepeatedItems.size()) {
    QString lMessage(tr("Platform code '%1' already exist. Record could not be saved.").arg(lValues.at(0)));
    emit sEventGenerated(5000003,lMessage, NMessage::ErrorMessage);
    return;
  }
  lRepeatedItems = pPlatformModel->findItems(lValues.at(1), Qt::MatchExactly, 1);
  if(lRepeatedItems.size()) {
    QString lMessage(tr("Platform name '%1' already exist. Record could not be saved.").arg(lValues.at(1)));
    emit sEventGenerated(5000004,lMessage, NMessage::ErrorMessage);
    return;
  }
  fInsertRecordOnTable(lValues);
  if(fSaveModel()) {
    QString lMessage(tr("Platform '%1' saved successfully.").arg(lValues.at(1)));
    emit sEventGenerated(1000002,lMessage, NMessage::SuccessMessage);
  }
  else {
    pPlatformModel->removeRows(pPlatformModel->rowCount() - 1, 1);
    QString lMessage(tr("Platform '%1' could not be saved. Error opening file.").arg(lValues.at(1)));
    emit sEventGenerated(5000005,lMessage, NMessage::ErrorMessage);
  }
}

void NPlatform::fSaveEditRecord(const QStringList& lValues) {
  QList<QStandardItem*> lRepeatedItems = pPlatformModel->findItems(lValues.at(0), Qt::MatchExactly, 1);
  if(lRepeatedItems.size()) {
    QString lMessage(tr("Platform name '%1' already exist. Record could not be saved.").arg(lValues.at(1)));
    emit sEventGenerated(5000004,lMessage, NMessage::ErrorMessage);
    return;
  }
  int lRow = tvwPlatform->currentIndex().row();
  fEditRecordOnTable(lRow, lValues);
  if(fSaveModel()) {
    QString lMessage(tr("Platform '%1' saved successfully.").arg(lValues.at(1)));
    emit sEventGenerated(1000002,lMessage, NMessage::SuccessMessage);
  }
  else {
    pPlatformModel->removeRows(pPlatformModel->rowCount() - 1, 1);
    QString lMessage(tr("Platform '%1' could not be saved. Error opening file.").arg(lValues.at(1)));
    emit sEventGenerated(5000005,lMessage, NMessage::ErrorMessage);
  }
}

void NPlatform::fEditRecordOnTable(int lRow, const QStringList& lValues) {
  QStandardItem* codItem = pPlatformModel->item(lRow,0);
  codItem->setData(lValues.at(0), Qt::EditRole);
  codItem->setData(lValues.at(0), Qt::DisplayRole);
  QStandardItem* osItem = pPlatformModel->item(lRow,1);
  osItem->setData(lValues.at(1), Qt::EditRole);
  osItem->setData(lValues.at(1), Qt::DisplayRole);
  QStandardItem* descItem = pPlatformModel->item(lRow,2);
  descItem->setData(lValues.at(2), Qt::EditRole);
  descItem->setData(lValues.at(2), Qt::DisplayRole);
}

void NPlatform::fInsertRecordOnTable(const QStringList& values) {
  QFont curFont = font();
  curFont.setItalic(true);
  QStandardItem* codItem = new QStandardItem(values.at(0));
  codItem->setData(values.at(0), Qt::EditRole);
  codItem->setData(NConstants::orange(), Qt::ForegroundRole);
  QStandardItem* osItem = new QStandardItem(values.at(1));
  osItem->setData(values.at(1), Qt::EditRole);
  osItem->setData(NConstants::green(), Qt::ForegroundRole);
  QStandardItem* descItem = new QStandardItem(values.at(2));
  descItem->setData(NConstants::darkBlue(), Qt::ForegroundRole);
  descItem->setData(values.at(2), Qt::EditRole);
  descItem->setData(curFont, Qt::FontRole);

  QList<QStandardItem*> items;
  items << codItem << osItem << descItem;
  pPlatformModel->appendRow(items);
  tvwPlatform->selectRow(pPlatformModel->rowCount() - 1);
}


void NPlatform::fMoveToCenter(QWidget* window)
{
  Q_ASSERT(window);

  QDesktopWidget* desktop = QApplication::desktop();
  int screenWidth, width;
  int screenHeight, height;
  int x, y;
  QSize windowSize;
  screenWidth = desktop->width(); // get width of screen
  screenHeight = desktop->height(); // get height of screen
  windowSize = window->size(); // size of our application window
  width = windowSize.width();
  height = windowSize.height();

  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  window->move(x,y);
}
