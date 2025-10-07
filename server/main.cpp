#include <QtGui/QGuiApplication>
#include <QSslSocket>

#include "nexus.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    app.setOrganizationDomain("some.io");
    app.setOrganizationName("AndrijFelenko");
    app.setApplicationName("StockManagerServer");

    qDebug() << "Device supports OpenSSL: ";
    qDebug() << "Supports SSL: " << QSslSocket::supportsSsl();
    qDebug() << "Runtime uses: " << QSslSocket::sslLibraryVersionString();
    qDebug() << "Build uses: "   << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "";

    // util::ResDir::list_qrc_files();
    DB->init();

    return app.exec();
}
