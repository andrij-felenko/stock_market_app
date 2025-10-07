#include <QtGui/QGuiApplication>
#include <QQuickStyle>
#include <QIcon>

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

    qDebug() << "Device supports OpenSSL: ";
    qDebug() << "Supports SSL: " << QSslSocket::supportsSsl();
    qDebug() << "Runtime uses: " << QSslSocket::sslLibraryVersionString();
    qDebug() << "Build uses: "   << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "";

    // util::ResDir::list_qrc_files();
    sdk::Nexus->init();

    return app.exec();
}
