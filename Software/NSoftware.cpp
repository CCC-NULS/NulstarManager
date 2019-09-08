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

#include "NSoftware.h"
#include "NSoftwareRecord.h"

NSoftware::NSoftware(QWidget* parent)
         : QWidget(parent) {
  setupUi(this);
  pRecord = new NSoftwareRecord(this);
  fCreateModel();  
  mSoftwareFile.setFileName("Software.ndd");
}


void NSoftware::fCreateModel() {
  QStringList lHeaders;
  lHeaders << tr("Code") << tr("Software Name") << tr("Description");
  pSoftwareModel = new QStandardItemModel(0, 3, this);
  pSoftwareModel->setHorizontalHeaderLabels(lHeaders);

  pProxyModel = new QSortFilterProxyModel(this);
  pProxyModel->setSortRole(Qt::DisplayRole);
  pProxyModel->setDynamicSortFilter(true);
  pProxyModel->setSourceModel(pSoftwareModel);

  tvwSoftware->setSortingEnabled(true);
  tvwSoftware->setModel(pProxyModel);
  tvwSoftware->sortByColumn(0, Qt::AscendingOrder);
  tvwSoftware->setColumnWidth(0,110);
  tvwSoftware->setColumnWidth(1,110);

  connect(tbtRefresh, SIGNAL(clicked()), this, SLOT(fLoadModel()));
  connect(tbtAdd, SIGNAL(clicked()), this, SLOT(fAddRecord()));
  connect(tbtEdit, SIGNAL(clicked()), this, SLOT(fEditRecord()));
  connect(tbtDelete, SIGNAL(clicked()), this, SLOT(fDeleteRecord()));
  connect(pRecord, SIGNAL(sObjectEdited(QStringList)), this, SLOT(fSaveEditRecord(QStringList)));
  connect(pRecord, SIGNAL(sObjectInserted(QStringList)), this, SLOT(fSaveAddRecord(QStringList)));
  connect(tvwSoftware, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fEditRecord()));
}

void NSoftware::fLoadModel() {
  if(mSoftwareFile.open(QFile::ReadOnly)) {
    pSoftwareModel->removeRows(0, pSoftwareModel->rowCount());
    QTextStream lStream(&mSoftwareFile);
    while(!lStream.atEnd()) {
      QString lLine = lStream.readLine();
      QStringList lRecord = lLine.split("|");
      if(lRecord.size() == 3)
        fInsertRecordOnTable(lRecord);
      else {
        QString lMessage(tr("Software.ndd has incorrect number of fields."));
        emit sEventGenerated(5001001, lMessage, NMessage::ErrorMessage);
      }
    }
    mSoftwareFile.close();
  }
  else {
    QString lMessage(tr("Software.ndd file could not be opened! Impossible to create package!"));
    emit sEventGenerated(5001002,lMessage, NMessage::ErrorMessage);
  }
}

bool NSoftware::fSaveModel() {
  if(mSoftwareFile.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream lStream(&mSoftwareFile);
    for(int i = 0; i < pSoftwareModel->rowCount(); i++) {
      QString lLine = QString("%1|%2|%3").arg(pSoftwareModel->data(pSoftwareModel->index(i,0), Qt::EditRole).toString()).arg(pSoftwareModel->data(pSoftwareModel->index(i,1), Qt::EditRole).toString()).arg(pSoftwareModel->data(pSoftwareModel->index(i,2), Qt::EditRole).toString());
      lStream << lLine << endl;
    }
    mSoftwareFile.close();
  }
  else
    return false;
  emit sDataChanged();
  return true;
}

void NSoftware::fDeleteRecord() {
  QModelIndexList lSelection = tvwSoftware->selectionModel()->selectedIndexes();
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
  if(pSoftwareModel->removeRow(lSelection.at(0).row())) {
    if(fSaveModel()) {
      QString lMessage(tr("Software '%1' deleted successfully!").arg(lName));
      emit sEventGenerated(1001003,lMessage, NMessage::SuccessMessage);
    }
    else {
      QString lMessage(tr("Failed to delete software '%1'").arg(lName));
      emit sEventGenerated(5001006,lMessage, NMessage::ErrorMessage);
    }
  }
  else {
    QString lMessage(tr("Software model does not contain selected row!"));
    emit sEventGenerated(4001001,lMessage, NMessage::BugMessage);
  }
}

void NSoftware::fAddRecord() {
  fMoveToCenter(pRecord);
  pRecord->setWindowTitle(tr("Add software"));
  pRecord->fSetDefault();
  pRecord->exec();
}

void NSoftware::fEditRecord() {
  fMoveToCenter(pRecord);
  pRecord->setWindowTitle(tr("Edit software"));
  QString lCode(pSoftwareModel->data(pSoftwareModel->index(tvwSoftware->currentIndex().row(), 0), Qt::EditRole).toString());
  QString lName(pSoftwareModel->data(pSoftwareModel->index(tvwSoftware->currentIndex().row(), 1), Qt::EditRole).toString());
  QString lDesc(pSoftwareModel->data(pSoftwareModel->index(tvwSoftware->currentIndex().row(), 2), Qt::EditRole).toString());
  QStringList lValues( {lCode, lName, lDesc} );
  pRecord->editRecord(lValues);
  pRecord->exec();
}

void NSoftware::fSaveAddRecord(const QStringList& lValues) {
  QList<QStandardItem*> lRepeatedItems = pSoftwareModel->findItems(lValues.at(0), Qt::MatchExactly, 0);
  if(lRepeatedItems.size()) {
    QString lMessage(tr("Software code '%1' already exist. Record could not be saved.").arg(lValues.at(0)));
    emit sEventGenerated(5001003,lMessage, NMessage::ErrorMessage);
    return;
  }
  lRepeatedItems = pSoftwareModel->findItems(lValues.at(1), Qt::MatchExactly, 1);
  if(lRepeatedItems.size()) {
    QString lMessage(tr("Software name '%1' already exist. Record could not be saved.").arg(lValues.at(1)));
    emit sEventGenerated(5001004,lMessage, NMessage::ErrorMessage);
    return;
  }
  fInsertRecordOnTable(lValues);
  if(fSaveModel()) {
    QString lMessage(tr("Software '%1' saved successfully.").arg(lValues.at(1)));
    emit sEventGenerated(1001002,lMessage, NMessage::SuccessMessage);
  }
  else {
    pSoftwareModel->removeRows(pSoftwareModel->rowCount() - 1, 1);
    QString lMessage(tr("Software '%1' could not be saved. Error opening file.").arg(lValues.at(1)));
    emit sEventGenerated(5001005,lMessage, NMessage::ErrorMessage);
  }
}

void NSoftware::fSaveEditRecord(const QStringList& lValues) {
  QList<QStandardItem*> lRepeatedItems = pSoftwareModel->findItems(lValues.at(0), Qt::MatchExactly, 1);
  if(lRepeatedItems.size()) {
    QString lMessage(tr("Software name '%1' already exist. Record could not be saved.").arg(lValues.at(1)));
    emit sEventGenerated(5001004,lMessage, NMessage::ErrorMessage);
    return;
  }
  int lRow = tvwSoftware->currentIndex().row();
  fEditRecordOnTable(lRow, lValues);
  if(fSaveModel()) {
    QString lMessage(tr("Software '%1' saved successfully.").arg(lValues.at(1)));
    emit sEventGenerated(1001002,lMessage, NMessage::SuccessMessage);
  }
  else {
    pSoftwareModel->removeRows(pSoftwareModel->rowCount() - 1, 1);
    QString lMessage(tr("Software '%1' could not be saved. Error opening file.").arg(lValues.at(1)));
    emit sEventGenerated(5001005,lMessage, NMessage::ErrorMessage);
  }
}

void NSoftware::fEditRecordOnTable(int lRow, const QStringList& lValues) {
  QStandardItem* codItem = pSoftwareModel->item(lRow,0);
  codItem->setData(lValues.at(0), Qt::EditRole);
  codItem->setData(lValues.at(0), Qt::DisplayRole);
  QStandardItem* osItem = pSoftwareModel->item(lRow,1);
  osItem->setData(lValues.at(1), Qt::EditRole);
  osItem->setData(lValues.at(1), Qt::DisplayRole);
  QStandardItem* descItem = pSoftwareModel->item(lRow,2);
  descItem->setData(lValues.at(2), Qt::EditRole);
  descItem->setData(lValues.at(2), Qt::DisplayRole);
}

void NSoftware::fInsertRecordOnTable(const QStringList& values) {
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
  pSoftwareModel->appendRow(items);
  tvwSoftware->selectRow(pSoftwareModel->rowCount() - 1);
}


void NSoftware::fMoveToCenter(QWidget* window)
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
