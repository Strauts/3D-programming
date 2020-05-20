#ifndef STARTMENUDIALOG_H
#define STARTMENUDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class startMenuDialog;
}

class startMenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit startMenuDialog(QWidget *parent = nullptr);
    ~startMenuDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::startMenuDialog *ui;
    MainWindow* endGame;
};

#endif // STARTMENUDIALOG_H
