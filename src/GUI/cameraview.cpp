#include <chrono>
#include <thread>
#include <iostream>
#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QPalette>
#include <QtWidgets>
#include <QVideoWidget>

#include "cameraview.h"
#include "userhelpdialog.h"

Q_DECLARE_METATYPE(QCameraInfo)

CameraView::CameraView() : ui(new Ui::CameraView)
{
    ui->setupUi(this);

    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        ui->cameraComboBox->addItem(cameraInfo.description());
        cameras.append(QVariant::fromValue(cameraInfo));
    }
    connect(ui->cameraComboBox, SIGNAL(activated(int)), this, SLOT(updateCameraDevice(int)));
    setCamera(QCameraInfo::defaultCamera());

    connect(ui->buttApply, &QPushButton::released, this, &CameraView::applySettings);
    connect(ui->buttPlay, &QPushButton::released, this, &CameraView::startCamera);
    connect(ui->buttPause, &QPushButton::released, this, &CameraView::stopCamera);

    connect(ui->buttOpenFile, &QPushButton::released, this, &CameraView::openFile);
    connect(ui->buttPlayVideo, &QPushButton::released, this, &CameraView::play);

    ui->buttPlay->setHidden(true);
    ui->buttPlayVideo->setEnabled(false);

    connect(ui->buttHelp, &QPushButton::released, this, &CameraView::showHelp);

    m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);

    connect(ui->videoSlider, &QAbstractSlider::sliderMoved, this, &CameraView::setPosition);
    
    ui->roadViewPlayer->setVFlag(false);
    m_mediaPlayer->setVideoOutput(ui->roadViewPlayer->videoSurface());
    connect(m_mediaPlayer, &QMediaPlayer::stateChanged, this, &CameraView::mediaStateChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &CameraView::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &CameraView::durationChanged);
    connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &CameraView::handleError);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CameraView::refreshEyeTrackingLabel);
    timer->start(100);
}

void CameraView::refreshEyeTrackingLabel() {
    ui->EyeTrackingStateLabel->setText(QString::fromStdString(eyeTrackingState.GetEyeTrackingStateString()));
}

void CameraView::showHelp() {
    UserHelpDialog helpDialog;
    helpDialog.setWindowFlags(helpDialog.windowFlags() & ~ Qt::WindowContextHelpButtonHint);
    helpDialog.setModal(true);
    helpDialog.exec();
    helpDialog.show();
}

void CameraView::refreshSettings(QScopedPointer<QMediaRecorder> &recorder)
{
    std::cout << "refresh settings" << std::endl;

    // Resolution stuff
    ui->resolutionBox->clear();
    ui->resolutionBox->addItem(tr("Default"));
    const QList<QSize> supportedResolutions = recorder->supportedResolutions();
    for (const QSize &resolution : supportedResolutions) {
        ui->resolutionBox->addItem(QString("%1x%2").arg(resolution.width()).arg(resolution.height()),
                                        QVariant(resolution));
    }

    // Frame rate stuff
    ui->framerateBox->clear();
    ui->framerateBox->addItem(tr("Default"));
    const QList<qreal> supportedFrameRates = recorder->supportedFrameRates();
    for (qreal rate : supportedFrameRates) {
        QString rateString = QString("%1").arg(rate, 0, 'f', 2);
        ui->framerateBox->addItem(rateString, QVariant(rate));
    }
}


void CameraView::applySettings() {
    std::cout << "apply settings" << std::endl;

    // Pull settings from the boxes
    QVideoEncoderSettings settings = m_mediaRecorder->videoSettings();
    settings.setResolution(getCBValue(ui->resolutionBox).toSize());
    settings.setFrameRate(getCBValue(ui->framerateBox).value<qreal>());

    m_mediaRecorder->setEncodingSettings(m_mediaRecorder->audioSettings(), settings);
}

QVariant CameraView::getCBValue(const QComboBox *box) const {
    int selected = box->currentIndex();
    return box->itemData(selected);
}

void CameraView::setCamera(const QCameraInfo &cameraInfo)
{
    std::cout << "Set camera event " << std::endl;
    m_camera.reset(new QCamera(cameraInfo));

    connect(m_camera.data(), &QCamera::stateChanged, this, &CameraView::updateCameraState);
    connect(m_camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &CameraView::displayCameraError);

    m_mediaRecorder.reset(new QMediaRecorder(m_camera.data()));
    connect(m_mediaRecorder.data(), &QMediaRecorder::stateChanged, this, &CameraView::updateRecorderState);

    connect(m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this, &CameraView::updateRecordTime);
    connect(m_mediaRecorder.data(), QOverload<QMediaRecorder::Error>::of(&QMediaRecorder::error), this, &CameraView::displayRecorderError);

    m_mediaRecorder->setMetaData(QMediaMetaData::Title, QVariant(QLatin1String("Test Title")));

    //connect(ui->exposureCompensation, &QAbstractSlider::valueChanged, this, &Camera::setExposureCompensation);

    m_camera->setViewfinder(ui->viewfinder->videoSurface());

    updateCameraState(m_camera->state());
    updateLockStatus(m_camera->lockStatus(), QCamera::UserRequest);
    updateRecorderState(m_mediaRecorder->state());

    connect(m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of(&QCamera::lockStatusChanged),
            this, &CameraView::updateLockStatus);

    //refreshSettings(m_mediaRecorder);
    refreshSettings(m_mediaRecorder);
    updateCaptureMode();
    m_camera->start();
}

void CameraView::keyPressEvent(QKeyEvent * event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case Qt::Key_CameraFocus:
        displayViewfinder();
        m_camera->searchAndLock();
        event->accept();
        break;
    case Qt::Key_Camera:
        if (m_camera->captureMode() == QCamera::CaptureStillImage) {
            takeImage();
        } else {
            if (m_mediaRecorder->state() == QMediaRecorder::RecordingState)
                stop();
            else
                record();
        }
        event->accept();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void CameraView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case Qt::Key_CameraFocus:
        m_camera->unlock();
        break;
    default:
        QMainWindow::keyReleaseEvent(event);
    }
}

void CameraView::updateRecordTime()
{
    QString str = QString("Recorded %1 sec").arg(m_mediaRecorder->duration()/1000);
    ui->statusbar->showMessage(str);
}

void CameraView::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(ui->viewfinder->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);

    // ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display captured image for 4 seconds.
    displayCapturedImage();
    QTimer::singleShot(4000, this, &CameraView::displayViewfinder);
}

void CameraView::record()
{
    m_mediaRecorder->record();
    updateRecordTime();
}

void CameraView::pause()
{
    m_mediaRecorder->pause();
}

void CameraView::stop()
{

    m_mediaRecorder->stop();
}

void CameraView::setMuted(bool muted)
{
    m_mediaRecorder->setMuted(muted);
}

void CameraView::toggleLock()
{
    switch (m_camera->lockStatus()) {
    case QCamera::Searching:
    case QCamera::Locked:
        m_camera->unlock();
        break;
    case QCamera::Unlocked:
        m_camera->searchAndLock();
    }
}

void CameraView::updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
    QColor indicationColor = Qt::black;

    switch (status) {
    case QCamera::Searching:
        indicationColor = Qt::yellow;
        ui->statusbar->showMessage(tr("Focusing..."));
        break;
    case QCamera::Locked:
        indicationColor = Qt::darkGreen;
        ui->statusbar->showMessage(tr("Focused"), 2000);
        break;
    case QCamera::Unlocked:
        indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
        if (reason == QCamera::LockFailed)
            ui->statusbar->showMessage(tr("Focus Failed"), 2000);
    }
}

void CameraView::takeImage()
{
    m_isCapturingImage = true;
}

void CameraView::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    m_isCapturingImage = false;
}

void CameraView::startCamera()
{
    ui->buttPlay->setHidden(true);
    ui->buttPause->setHidden(false);
    m_camera->start();
}

void CameraView::stopCamera()
{
    ui->buttPause->setHidden(true);
    ui->buttPlay->setHidden(false);
    m_camera->stop();
}

void CameraView::updateCaptureMode()
{
    QCamera::CaptureModes captureMode = QCamera::CaptureVideo;

    if (m_camera->isCaptureModeSupported(captureMode))
        m_camera->setCaptureMode(captureMode);
}

void CameraView::updateCameraState(QCamera::State state)
{
    switch (state) {
    case QCamera::ActiveState:
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        //ui->captureWidget->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        ui->actionSettings->setEnabled(false);
    }
}

void CameraView::updateRecorderState(QMediaRecorder::State state)
{
    std::cout << "Update recorder state" << std::endl;
}

void CameraView::setExposureCompensation(int index)
{
    m_camera->exposure()->setExposureCompensation(index*0.5);
}

void CameraView::displayRecorderError()
{
    QMessageBox::warning(this, tr("Capture Error"), m_mediaRecorder->errorString());
}

void CameraView::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), m_camera->errorString());
}

void CameraView::updateCameraDevice(int index)
{
    std::cout << "UpdateCameraDevice event " << index << std::endl;
    setCamera(qvariant_cast<QCameraInfo>(cameras[index]));
}

void CameraView::displayViewfinder()
{
    // ui->verticalLayout->setCurrentIndex(0);
}

void CameraView::displayCapturedImage()
{
    // ui->verticalLayout->setCurrentIndex(1);
}

void CameraView::readyForCapture(bool ready)
{
    //ui->takeImageButton->setEnabled(ready);
    std::cout << "Ready for capture " << ready << std::endl;
}

void CameraView::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    ui->statusbar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    m_isCapturingImage = false;
    if (m_applicationExiting)
        close();
}

void CameraView::closeEvent(QCloseEvent *event)
{
    if (m_isCapturingImage) {
        setEnabled(false);
        m_applicationExiting = true;
        event->ignore();
    } else {
        event->accept();
    }
}

void CameraView::setUrl(const QUrl &url)
{
    ui->errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    m_mediaPlayer->setMedia(url);
    ui->buttPlayVideo->setEnabled(true);
}

void CameraView::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    QStringList supportedMimeTypes = m_mediaPlayer->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty())
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        setUrl(fileDialog.selectedUrls().constFirst());
}

void CameraView::play()
{
    switch (m_mediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->pause();
        break;
    default:
        m_mediaPlayer->play();
        break;
    }
}

void CameraView::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        ui->buttPlayVideo->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        ui->buttPlayVideo->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void CameraView::positionChanged(qint64 position)
{
    ui->videoSlider->setValue(position);
}

void CameraView::durationChanged(qint64 duration)
{
    ui->videoSlider->setRange(0, duration);
}

void CameraView::setPosition(int position)
{
    m_mediaPlayer->setPosition(position);
}

void CameraView::handleError()
{
    ui->buttPlayVideo->setEnabled(false);
    const QString errorString = m_mediaPlayer->errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(m_mediaPlayer->error()));
    else
        message += errorString;
    ui->errorLabel->setText(message);
}
