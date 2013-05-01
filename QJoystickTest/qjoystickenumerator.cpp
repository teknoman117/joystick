#include "qjoystickenumerator.h"
#include "ui_qjoystickenumerator.h"

#include <dirent.h>

QJoystickEnumerator::QJoystickEnumerator(QString directory, QWidget *parent) : QDialog(parent), ui(new Ui::QJoystickEnumerator), mDirectory(directory) {
    ui->setupUi(this);
    if(!mDirectory.endsWith('/')) mDirectory += '/';
    refreshList();
    refreshTimer = new QTimer();
    refreshTimer->start(1000);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshList()));
}

QJoystickEnumerator::~QJoystickEnumerator() { delete ui; }

void QJoystickEnumerator::refreshList() {
    bool reselect = false;
    QModelIndex index;
    if(ui->joystickList->selectedItems().count() > 0) {
        index = ui->joystickList->selectionModel()->selectedIndexes().at(0);
        reselect = true;
    }
    ui->joystickList->clear();
    DIR* dir;
    struct dirent *dirp;
    dir = opendir(mDirectory.toAscii().data());
    while((dirp = readdir(dir)) != NULL) {
        if(QString(dirp->d_name).startsWith("js")) {
            QString path = mDirectory + dirp->d_name;
            JSInfo info = QJoystick::getJoystickInfo(path);
            QListWidgetItem *item = new QListWidgetItem(info.name, ui->joystickList);
            item->setToolTip(path);
        }
    }
    if(reselect) {
        ui->joystickList->selectionModel()->select(index, QItemSelectionModel::Select);
    }
}

QJoystick* QJoystickEnumerator::getJoystick() {
    if(ui->joystickList->selectedItems().count() < 1) return NULL;
    QListWidgetItem *item = ui->joystickList->selectedItems().at(0);
    return QJoystick::openJoystick(item->toolTip(), 1);
}

QJoystick* QJoystickEnumerator::enumerate(QString jsDir, QWidget *parent) {
    QJoystickEnumerator *enumerator = new QJoystickEnumerator(jsDir, parent);
    if(enumerator->exec() == QDialog::Accepted) {
        return enumerator->getJoystick();
    }
    return NULL;
}
