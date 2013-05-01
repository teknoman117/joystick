#ifndef QJOYSTICK_H
#define QJOYSTICK_H

#include <QObject>
#include <QTimer>

#include <linux/joystick.h>

typedef struct {
    QString name;
    int buttons;
    int axes;
} JSInfo;


class QJoystick : public QObject
{
    Q_OBJECT
    QTimer* updateTimer;
    short   threshold;
    /* System variables for Joystick */
    QString name;
    int     jd;
    bool*   buttons;
    short*  axes;
    int     buttonc;
    int     axesc;
    struct  js_event event;

    /* To fix the problem of a non existant joystick, constructor is private */
    QJoystick(int fd);

public:
    ~QJoystick();

    static QJoystick* openJoystick(QString, int);
    static JSInfo     getJoystickInfo(QString);

    void   setThreshold(short threshold); // This defines the center of a joystick for edgy joysticks (Like the gamestop Xbox Pad)
    JSInfo getInfo();
    void   setUpdateInterval(int msec);
    bool   getButton(int num);
    short  getAxis(int num);

signals:
    void axisChanged(int,short);
    void buttonChanged(int,bool);
    void updated();

private slots:
    void updateValues();

};

#endif // QJOYSTICK_H
