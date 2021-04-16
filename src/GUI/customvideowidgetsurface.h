#ifndef CUSTOMVIDEOWIDGETSURFACE_H
#define CUSTOMVIDEOWIDGETSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QRect>
#include <QVideoFrame>
#include "shared/imagepipeline.h"

namespace Ui {
    class CustomVideoWidgetSurface : public QAbstractVideoSurface
    {
        Q_OBJECT

        public:
            CustomVideoWidgetSurface(QWidget *widget, QObject *parent = 0, bool vFlag = true);

            QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                    QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
            bool isFormatSupported(const QVideoSurfaceFormat &format) const override;

            bool start(const QVideoSurfaceFormat &format) override;
            void stop() override;

            bool present(const QVideoFrame &frame) override;

            QRect videoRect() const { return targetRect; }
            void updateVideoRect();

            void paint(QPainter *painter);

        private:
            QWidget *widget;
            QImage::Format imageFormat;
            QRect targetRect;
            QSize imageSize;
            QRect sourceRect;
            QVideoFrame currentFrame;
            // Custom image pipeline
            ass::ImagePipeline pipeline;
            bool viewFlag;
    };
}

#endif // CUSTOMVIDEOWIDGETSURFACE_H
