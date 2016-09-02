#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <core.h>
#include <cube.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void serialPortButtonPushed();
private:
    Ui::MainWindow *ui;
    Core *core;
    Cube *cube;
};

#endif // MAINWINDOW_H
