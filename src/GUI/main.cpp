#include "camera.h"

#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    Camera c;
    c.show();
    return a.exec();
}
