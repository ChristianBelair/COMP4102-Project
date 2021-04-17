#include "customvideowidgetsurface.h"

#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>
#include <iostream>
#include <shared/cvmatandqimage.h>
#include <opencv2/opencv.hpp>

namespace Ui {

    CustomVideoWidgetSurface::CustomVideoWidgetSurface(QWidget *widget, QObject *parent, bool vFlag)
        : QAbstractVideoSurface(parent)
        , widget(widget)
        , imageFormat(QImage::Format_Invalid)
    {
        std::cout << "CustomVideoWidgetSurface ctor, image format " << imageFormat << std::endl;
        viewFlag = vFlag;
    }

    QList<QVideoFrame::PixelFormat> CustomVideoWidgetSurface::supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType) const
    {
        if (handleType == QAbstractVideoBuffer::NoHandle) {            return QList<QVideoFrame::PixelFormat>()
                    << QVideoFrame::Format_RGB32
                    << QVideoFrame::Format_ARGB32
                    << QVideoFrame::Format_ARGB32_Premultiplied
                    << QVideoFrame::Format_RGB565
                    << QVideoFrame::Format_RGB555
                    << QVideoFrame::Format_ABGR32;
        } else {
            return QList<QVideoFrame::PixelFormat>();
        }
    }

    bool CustomVideoWidgetSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
    {
        const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
        const QSize size = format.frameSize();

        return imageFormat != QImage::Format_Invalid
                && !size.isEmpty()
                && format.handleType() == QAbstractVideoBuffer::NoHandle;
    }

    bool CustomVideoWidgetSurface::start(const QVideoSurfaceFormat &format)
    {
        const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
        const QSize size = format.frameSize();

        if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
            this->imageFormat = imageFormat;
            imageSize = size;
            sourceRect = format.viewport();

            QAbstractVideoSurface::start(format);

            widget->updateGeometry();
            updateVideoRect();

            return true;
        } else {
            return false;
        }
    }

    void CustomVideoWidgetSurface::stop()
    {
        currentFrame = QVideoFrame();
        targetRect = QRect();

        QAbstractVideoSurface::stop();

        widget->update();
    }

    bool CustomVideoWidgetSurface::present(const QVideoFrame &frame)
    {
        std::cout << "Present tick " << std::endl;
        if (surfaceFormat().pixelFormat() != frame.pixelFormat()
                || surfaceFormat().frameSize() != frame.size()) {
            setError(IncorrectFormatError);
            std::cout << "incorrect format" << std::endl;
            stop();

            return false;
        } else {
            // Convert the frame
            try {
                // Convert to QImage for processing
                QImage conv = frame.image();

                // Send image through pipeline
                conv = pipeline.ProcessFrame(conv, viewFlag);

                // Convert back to QVideoFrame for rendering
                currentFrame = QVideoFrame(conv);
                widget->repaint(targetRect);
            }  catch (cv::Exception& e) {
                // Things tend to break here when changing video streams, we can just eat exceptions for now?
                std::cout << e.what() << std::endl;
                return true;
            }
            return true;
        }
    }
    //! [4]

    //! [5]
    void CustomVideoWidgetSurface::updateVideoRect()
    {
        QSize size = surfaceFormat().sizeHint();
        size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);

        targetRect = QRect(QPoint(0, 0), size);
        targetRect.moveCenter(widget->rect().center());
    }
    //! [5]

    //! [6]
    void CustomVideoWidgetSurface::paint(QPainter *painter)
    {
        if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
            const QTransform oldTransform = painter->transform();

            if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
               painter->scale(1, -1);
               painter->translate(0, -widget->height());
            }

            QImage image(
                    currentFrame.bits(),
                    currentFrame.width(),
                    currentFrame.height(),
                    currentFrame.bytesPerLine(),
                    imageFormat);

            painter->drawImage(targetRect, image, sourceRect);

            painter->setTransform(oldTransform);

            currentFrame.unmap();
        }
    }
    //! [6]

    void CustomVideoWidgetSurface::setVFlag(bool vFlag) {
        viewFlag = vFlag;
    }
}
