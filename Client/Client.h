#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class Client : public QWidget {
    Q_OBJECT

private:
    QTcpSocket *socket;
    QTextBrowser *responseDisplay;
    QLineEdit *inputField;
    QLabel *statusLabel;
    QComboBox *filterTypeDropdown;
    bool isAuthenticated;

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void sendCommand();
    void readResponse();
    void onConnected();
    void onDisconnected();
    void exitClient();
};

#endif // CLIENT_H
