#include "GUI/cameraview.h"

#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    CameraView driverView;
    CameraView roadView;
    driverView.show();
    roadView.show();
    return a.exec();
}
