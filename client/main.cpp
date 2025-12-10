#include <QtGui/QGuiApplication>
#include <QIcon>
#include <QQuickStyle>

#include <QDebug>

#include <QSslSocket>

#include <client.h>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");

    app.setOrganizationDomain("some.io");
    app.setOrganizationName("AndrijFelenko");
    app.setApplicationName("StockManager");
    app.setWindowIcon(QIcon(":/rc/images/Stock_app_logo2.png"));

    qInfo() << "OpenSSL support:" << QSslSocket::supportsSsl();
    qInfo() << "Runtime SSL library:" << QSslSocket::sslLibraryVersionString();
    qInfo() << "Build-time SSL library:" << QSslSocket::sslLibraryBuildVersionString();

    // util::ResDir::list_qrc_files();
    Client.init();

    return app.exec();
}
