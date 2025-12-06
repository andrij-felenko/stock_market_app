#include <QtGui/QGuiApplication>

#include <QDebug>
#include <QSslSocket>

#include "nexus.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    app.setOrganizationDomain("some.io");
    app.setOrganizationName("AndrijFelenko");
    app.setApplicationName("StockManagerServer");

    qInfo() << "OpenSSL support:" << QSslSocket::supportsSsl();
    qInfo() << "Runtime SSL library:" << QSslSocket::sslLibraryVersionString();
    qInfo() << "Build-time SSL library:" << QSslSocket::sslLibraryBuildVersionString();

    // util::ResDir::list_qrc_files();
    DB->init();

    return app.exec();
}
