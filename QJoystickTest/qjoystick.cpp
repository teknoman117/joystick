#include "qjoystick.h"

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>

#include <linux/input.h>
#include <sys/ioctl.h>

short sabs(short s) {
    if(s < 0) return s * -1;
    return s;
}

QJoystick::QJoystick(int fd) : QObject(0) {
    /* Initalize Things */
    this->axesc = 0;
    this->buttonc = 0;
    this->buttons = NULL;
    this->axes = NULL;
    /* Open the joystick settings */
    threshold = 0;
    jd = fd;                           // Store the file descriptor
    char *buf = static_cast<char *>(malloc(sizeof(char) * 80)); // Create a small character buffer
    ioctl(jd, JSIOCGAXES, &axesc);      // Get the axis count
    ioctl(jd, JSIOCGBUTTONS, &buttonc); // Get the button count
    ioctl(jd, JSIOCGNAME(80), buf);     // Get the name
    name = QString(buf);                // store the name
    axes = static_cast<short *>(malloc(axesc * sizeof(short)));    // Create a buffer for axis values
    buttons = static_cast<bool *>(malloc(buttonc * sizeof(bool))); // Create a buffer for button values
    fcntl(jd, F_SETFL, O_NONBLOCK);     // set the port mode
    free(buf);
    /* Create the update timer */
    updateTimer = new QTimer();
    updateTimer->start(1);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
}

QJoystick::~QJoystick() {
    updateTimer->stop();
    delete updateTimer;
    free(axes);
    free(buttons);
    close(jd);
}

void QJoystick::setUpdateInterval(int msec) {updateTimer->start(msec);}

void QJoystick::setThreshold(short threshold) {this->threshold = threshold;}

JSInfo QJoystick::getInfo() {
    JSInfo info;
    info.axes = axesc;
    info.buttons = buttonc;
    info.name = name;
    return info;
}

void QJoystick::updateValues() {
    short axisvalue = 0;
    for(int i = 0; i < 10; i++) {
        read(jd, &event, sizeof(struct js_event));   // Get event structure
        switch (event.type & ~JS_EVENT_INIT)  {
            case JS_EVENT_AXIS:
                axisvalue = event.value;
                if((axisvalue < sabs(threshold)) && (axisvalue > (sabs(threshold) * -1))) axisvalue = 0;
                if(axes[event.number] != axisvalue) emit axisChanged(event.number, axisvalue);
                axes[event.number] = axisvalue;
                break;
            case JS_EVENT_BUTTON:
                if(buttons[event.number] != (event.value == 1)) emit buttonChanged(event.number, (event.value == 1));
                buttons[event.number] = (event.value == 1);
                break;
        }
    }
    emit updated();
}

short QJoystick::getAxis(int num) {
    if(num < axesc) return axes[num];
    return 0;
}

bool QJoystick::getButton(int num) {
    if(num < buttonc) return buttons[num];
    return 0;
}

/* Static Functions */

QJoystick* QJoystick::openJoystick(QString path, int msec) {
    char* p = (char*) path.toAscii().data(); // Get the ascii version of the path
    int fd = open(p, O_RDONLY);              // Open the dev port
    if(fd < 0) return NULL;                  // couldn't open joystick
    QJoystick *js = new QJoystick(fd);       // Create the object
    js->setUpdateInterval(msec);             // Set the update interval
    return js;
}

JSInfo QJoystick::getJoystickInfo(QString path) {
    int fd = open(path.toAscii().data(), O_RDONLY); // Open the dev port
    JSInfo info;
    if(fd < 0) return info;                  // couldn't open joystick
    char *buf = static_cast<char *>(malloc(sizeof(char) * 80)); // Create a small character buffer
    ioctl(fd, JSIOCGAXES, &info.axes);      // Get the axis count
    ioctl(fd, JSIOCGBUTTONS, &info.buttons);// Get the button count
    ioctl(fd, JSIOCGNAME(80), buf);          // Get the name
    info.name = QString(buf);               // store the name
    close(fd);
    free(buf);
    return info;
}


