#ifndef QJOYSTICKENUMERATOR_H
#define QJOYSTICKENUMERATOR_H

#include <QDialog>
#include <QTimer>
#include <QListIterator>
#include <QModelIndexList>
#include <QListWidgetItem>

#include "qjoystick.h"

namespace Ui {
    class QJoystickEnumerator;
}

class QJoystickEnumerator : public QDialog
{
    Q_OBJECT
    QTimer *refreshTimer;
    QString mDirectory;
    QModelIndexList selectedIndexes;
public:
    explicit QJoystickEnumerator(QString directory, QWidget *parent = 0);
    ~QJoystickEnumerator();
    QJoystick* getJoystick();

    static QJoystick* enumerate(QString jsDir, QWidget *parent = 0);

private:
    Ui::QJoystickEnumerator *ui;

private slots:
    void refreshList();

};

#endif // QJOYSTICKENUMERATOR_H
