#include "userhelpdialog.h"

UserHelpDialog::UserHelpDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui_UserHelpDialog)
{
    ui->setupUi(this);
    connect(ui->buttClose, &QPushButton::pressed, this, &UserHelpDialog::close);
}
