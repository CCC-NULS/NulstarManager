#ifndef WCCONNECTOR_H
#define WCCONNECTOR_H

#include <QHostAddress>
#include <QObject>
#include <QWidget>

#include "Connector.h"
#include "ui_Connector.h"

class QFtp;
class QLineEdit;

class WCConnector : public QWidget, private Ui::uiConnector
{
  Q_OBJECT

  public:
    explicit WCConnector(QWidget* parent = 0);

    bool status() const { return _status; }
    bool status(const QLineEdit* lineEdit) const { return lineEdit->property("Status").toBool(); }
    void createConnections();
    void createObjects();
    void createValidators();
    QStringList dbParams();
    QStringList ftpParams();

  protected:
    bool checkUIStatus() const;

  private:
    bool _status;
    int _ftpClose;
    int _ftpConnectId;
    int _ftpLoginId;
    QFtp* _ftp;
    QHostAddress _mainAddress;
    QString _mainConnection;

  public slots:
    void establishConnection();
    void loadSettings();
    void saveSettings();
    void setStatus(QLineEdit* lineEdit, bool status);

  protected slots:
    void processFtpCommandResult(int id, bool error);

  private slots:
    void validateParameter(const QString& text);

  signals:
    void connected();
    void disconnected();
    void eventGenerated(int code);
};

#endif // WCCONNECTOR_H
