#include "startmenudialog.h"
#include "ui_startmenudialog.h"

#include "renderwindow.h"
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QTextEdit>
#include <QDebug>

startMenuDialog::startMenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startMenuDialog)
{
    ui->setupUi(this);
}

startMenuDialog::~startMenuDialog()
{
    delete ui;
}

void startMenuDialog::on_pushButton_clicked()
{
    endGame->close();
    close();
}
