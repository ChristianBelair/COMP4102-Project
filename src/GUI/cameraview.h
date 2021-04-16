#ifndef CAMERA_H
#define CAMERA_H

#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QPushButton>
#include <QMainWindow>
#include <QComboBox>
#include "ui_cameraview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CameraView; }
QT_END_NAMESPACE


class CameraView : public QMainWindow
{
    Q_OBJECT

public:
    CameraView();

private slots:
    void setCamera(const QCameraInfo &cameraInfo);

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();
    void setMuted(bool);

    void toggleLock();
    void takeImage();
    void displayCaptureError(int, QCameraImageCapture::Error, const QString &errorString);

    void displayRecorderError();
    void displayCameraError();

    void updateCameraDevice(int index);

    void updateCameraState(QCamera::State);
    void updateCaptureMode();
    void updateRecorderState(QMediaRecorder::State state);
    void setExposureCompensation(int index);

    void updateRecordTime();

    void updateLockStatus(QCamera::LockStatus, QCamera::LockChangeReason);

    void readyForCapture(bool ready);
    void imageSaved(int id, const QString &fileName);

    void applySettings();
    QVariant getCBValue(const QComboBox *box) const;

    void showHelp();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void refreshSettings(QScopedPointer<QMediaRecorder> &recorder);

private:
    Ui_CameraView *ui;

    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QMediaRecorder> m_mediaRecorder;

    QImageEncoderSettings m_imageSettings;
    QAudioEncoderSettings m_audioSettings;
    QVideoEncoderSettings m_videoSettings;
    QPushButton m_settingsApply;
    QVector<QVariant> cameras;
    QString m_videoContainerFormat;
    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
};


#endif // CAMERA_H
