#ifndef NSOFTWARERECORD_H
#define NSOFTWARERECORD_H

#include <QDialog>
#include <QLineEdit>
#include <QStringList>

#include "Software.h"
#include "ui_NSoftwareRecord.h"

class NSoftwareRecord : public QDialog, private Ui::uiSoftwareRecord
{
  Q_OBJECT

  public:
    explicit NSoftwareRecord(QWidget* parent = nullptr);
    bool status(const QLineEdit* lineEdit) const { return lineEdit->property("Status").toBool(); }
    void configUi();
    void createConnections();
    void createValidators();
    void editRecord(const QStringList& values);
    void fSetDefault();
    void fSetStatus(QLineEdit* lineEdit, bool status);

  protected:
    bool checkUIStatus() const;

  private:
    bool _addingRecord;

  private slots:
    void saveRecord();
    void validateParameter(const QString& text);

  signals:
    void sObjectEdited(const QStringList& values);
    void sObjectInserted(const QStringList& values);
};

#endif //NSOFTWARERECORD_H
