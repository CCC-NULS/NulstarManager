#ifndef NPLATFORMRECORD_H
#define NPLATFORMRECORD_H

#include <QDialog>
#include <QLineEdit>
#include <QStringList>

#include "Platform.h"
#include "ui_NPlatformRecord.h"

class NPlatformRecord : public QDialog, private Ui::uiPlatformRecord
{
  Q_OBJECT

  public:
    explicit NPlatformRecord(QWidget* parent = nullptr);
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
    void sEventGenerated(int lCode, QString lMessage,int lType);
    void sObjectEdited(const QStringList& values);
    void sObjectInserted(const QStringList& values);
};

#endif //NPLATFORMRECORD_H
