#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QLabel>

#include "qjoystickenumerator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QJoystick *joystick;
    QLabel    *infoLabel;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void exitApp();
    void openJoystick();
    void editThreshold();

    void updateAxis(int,short);
    void updateButton(int,bool);

};

#endif // MAINWINDOW_H
