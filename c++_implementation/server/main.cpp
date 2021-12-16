#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server(QCoreApplication::arguments().at(1));

    return a.exec();
}
