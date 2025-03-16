#include <QApplication>
#include "Client.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Client client;
    client.resize(400, 600);
    client.show();

    return app.exec();
}
