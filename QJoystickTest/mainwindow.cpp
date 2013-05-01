#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    infoLabel = new QLabel();
    ui->statusBar->addWidget(infoLabel);
    joystick = NULL;
    connect(ui->actionOpenJoystick, SIGNAL(triggered()), this, SLOT(openJoystick()));
    connect(ui->actionChangeThreshold, SIGNAL(triggered()), this, SLOT(editThreshold()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(exitApp()));
}

MainWindow::~MainWindow() {delete ui;}

void MainWindow::openJoystick() {
    QJoystick *nJoystick = QJoystickEnumerator::enumerate("/dev/input", this);
    if(nJoystick != NULL) {
        if(joystick != NULL) delete joystick;
        JSInfo info = nJoystick->getInfo();
        infoLabel->setText(info.name + " - Axes: " + QString::number(info.axes) + "; Buttons: " + QString::number(info.buttons));
        ui->DataTable->clear();
        ui->DataTable->setColumnCount(2);
        ui->DataTable->setColumnWidth(1, 200);
        ui->DataTable->setRowCount((info.buttons > info.axes) ? info.buttons : info.axes);
        for(int i=0; i<info.axes; i++) ui->DataTable->setItem(i, 0, new QTableWidgetItem(QString("Axis ") + QString::number(i + 1) + ":"));
        for(int i=0; i<info.buttons; i++) ui->DataTable->setItem(i, 1, new QTableWidgetItem(QString("Button ") + QString::number(i + 1) + ":"));
        joystick = nJoystick;
        connect(joystick, SIGNAL(axisChanged(int,short)), this, SLOT(updateAxis(int,short)));
        connect(joystick, SIGNAL(buttonChanged(int,bool)), this, SLOT(updateButton(int,bool)));
    }
}

void MainWindow::editThreshold() {
    if(joystick) joystick->setThreshold(QInputDialog::getInt(this, "Enter Threshold Value", "Threshold"));
}

void MainWindow::exitApp() {
    exit(0);
}

void MainWindow::updateAxis(int n, short v) {
    ui->DataTable->item(n, 0)->setText(QString("Axis ") + QString::number(n + 1) + ": " + QString::number(v));
}

void MainWindow::updateButton(int n, bool v) {
    ui->DataTable->item(n, 1)->setText(QString("Button ") + QString::number(n + 1) + ": " + ((v) ? QString("Pressed") : QString("")));
}
