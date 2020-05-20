#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

class QSurface;
class QTextEdit;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool running = true;
    void updateText();
    void updateText2();

public slots:
    void init();

private slots:
    void renderWindowReady();
    void renderWindowError(const QString &msg);

    void on_actionWireframe_toggled(bool arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    void addRenderWindow();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H
