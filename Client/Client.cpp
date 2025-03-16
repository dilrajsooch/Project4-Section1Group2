#include "Client.h"
#include <QVBoxLayout>
#include <QMessageBox>

Client::Client(QWidget *parent)
    : QWidget(parent), isAuthenticated(false) {
    setWindowTitle("Discussion Board Client");

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Status Label
    statusLabel = new QLabel("Disconnected", this);
    layout->addWidget(statusLabel);

    // Input Field
    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Enter command (e.g., LOGIN|Lebron23,James23)");
    layout->addWidget(inputField);

    // Filter Type Dropdown
    filterTypeDropdown = new QComboBox(this);
    filterTypeDropdown->addItem("Select Filter Type");
    filterTypeDropdown->addItem("AUTHOR");
    filterTypeDropdown->addItem("TOPIC");
    layout->addWidget(filterTypeDropdown);

    // Buttons
    QPushButton *sendButton = new QPushButton("Send Command", this);
    layout->addWidget(sendButton);

    QPushButton *exitButton = new QPushButton("Exit", this);
    layout->addWidget(exitButton);

    // Response Display
    responseDisplay = new QTextBrowser(this);
    layout->addWidget(responseDisplay);

    setLayout(layout);

    // Socket
    socket = new QTcpSocket(this);

    // Connect Signals
    connect(sendButton, &QPushButton::clicked, this, &Client::sendCommand);
    connect(exitButton, &QPushButton::clicked, this, &Client::exitClient);
    connect(socket, &QTcpSocket::readyRead, this, &Client::readResponse);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);

    // Connect to Server
    socket->connectToHost("172.16.5.12", 27000);
}

void Client::sendCommand() {
    QString command = inputField->text().trimmed();

    // Handle FILTER command
    if (command.startsWith("FILTER|")) {
        if (filterTypeDropdown->currentText() == "Select Filter Type") {
            QMessageBox::warning(this, "Filter Error", "Please select a valid filter type.");
            return;
        }
        command = "FILTER|" + filterTypeDropdown->currentText() + "," + command.mid(7);
    }

    // Authentication enforcement
    if (!isAuthenticated && !command.startsWith("LOGIN|")) {
        QMessageBox::warning(this, "Authentication Error", "Please log in first.");
        return;
    }

    if (command.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Command cannot be empty!");
        return;
    }

    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(command.toUtf8());
        responseDisplay->append("Client: " + command);
        inputField->clear();
    } else {
        QMessageBox::critical(this, "Connection Error", "Not connected to the server!");
    }
}

void Client::readResponse() {
    QString response = socket->readAll();
    responseDisplay->append("Server: " + response);

    if (response.startsWith("ACK|LOGIN")) {
        isAuthenticated = true;
        statusLabel->setText("Authenticated");
    }
}

void Client::onConnected() {
    statusLabel->setText("Connected to server");
}

void Client::onDisconnected() {
    statusLabel->setText("Disconnected");
    isAuthenticated = false;
}

void Client::exitClient() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write("EXIT|");
        socket->disconnectFromHost();
    }
    close();
}
