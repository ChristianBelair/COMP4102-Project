
#include "customvideowidget.h"

#include "customvideowidgetsurface.h"
#include <iostream>
#include <QtWidgets>
#include <qvideosurfaceformat.h>

namespace Ui {

    CustomVideoWidget::CustomVideoWidget(QWidget *parent, bool vFlag)
        : QWidget(parent)
        , surface(0)
    {
        std::cout << "CustomVideoWidget ctor" << std::endl;

        setAutoFillBackground(false);
        setAttribute(Qt::WA_NoSystemBackground, true);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Qt::black);
        setPalette(palette);

        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

        surface = new CustomVideoWidgetSurface(this, vFlag);
    }

    CustomVideoWidget::~CustomVideoWidget()
    {
        delete surface;
    }

    QSize CustomVideoWidget::sizeHint() const
    {
        return surface->surfaceFormat().sizeHint();
    }

    void CustomVideoWidget::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);

        if (surface->isActive()) {
            const QRect videoRect = surface->videoRect();

            if (!videoRect.contains(event->rect())) {
                QRegion region = event->region();
                region = region.subtracted(videoRect);

                QBrush brush = palette().window();

                for (const QRect &rect : region)
                    painter.fillRect(rect, brush);
            }

            surface->paint(&painter);
        } else {
            painter.fillRect(event->rect(), palette().window());
        }
    }

    void CustomVideoWidget::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);

        surface->updateVideoRect();
    }

}
