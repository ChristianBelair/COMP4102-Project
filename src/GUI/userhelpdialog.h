#ifndef USERHELPDIALOG_H
#define USERHELPDIALOG_H
#include <QDialog>
#include <iostream>
#include "ui_userhelpdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CameraView; }
QT_END_NAMESPACE

class UserHelpDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit UserHelpDialog(QWidget *parent = nullptr);

    private:
        Ui_UserHelpDialog *ui;
};

#endif // USERHELPDIALOG_H
