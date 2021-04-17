#ifndef CUSTOMVIDEOWIDGET_H
#define CUSTOMVIDEOWIDGET_H


#include "customvideowidgetsurface.h"

#include <QWidget>

namespace Ui {
    class CustomVideoWidget : public QWidget
    {
        Q_OBJECT

    public:
        CustomVideoWidget(QWidget *parent = 0, bool vFlag = true);
        ~CustomVideoWidget();

        QAbstractVideoSurface *videoSurface() const { return surface; }

        QSize sizeHint() const override;

        void setVFlag(bool vFlag = true);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        CustomVideoWidgetSurface *surface;
    };
}

#endif // CUSTOMVIDEOWIDGET_H
