/* This library is to control a multi axis joystick with buttons from a C/C++ program
 *
 * Author: Nathaniel Lewis (linux.robotdude@gmail.com)
 *
 * Anyone is free to use this library for any purpose.  If you use it or modify it,
 * just be sure to include me as the original author of this library
 *
 * This software is open source, for the licensing info view the GPLv3 license in the
 * LICENSE file
 *
 */

 #ifndef __JOYSTICK_H__
 #define __JOYSTICK_H__ 1

#include <linux/joystick.h>

#ifdef __cplusplus              /* If the language is C++, use the Joystick class */

class Joystick {
protected:
    char *device;               /* Holds the path to the device file */
    int js_fd;                  /* Holds the joystick file descriptor */
    int axis_count;             /* Holds the number of axes the joystick has */
    int button_count;           /* Holds the number of buttons the joystick has */
    char name[80];              /* Holds the name of the joystick */
    int *axes;                  /* A pointer to the values of the axes of the joystick */
    char *buttons;              /* A pointer to the values of the buttons of the joystick */
    struct js_event event;      /* The joystick event structure */
public:
    int connected;              /* Holds status of the connection state to the joystick */
    Joystick();                 /* A constructor that does nothing, open must then be called before any other function int the class */
    Joystick( const char* joydev ); /* The constructor for the object, must have the joystick port */
    ~Joystick();                /* The deconstructor for the object */
    int init( const char* joydev ); /* Opens a joystick following a blank constructor or after a close */
    void stop();                /* A function that calls the deconstructor to close an open joystick */
    char getButton( int button ); /* Returns the value of the specified button */
    int getAxis( int axis );    /* Returns the value of the specified axis */
    char* getName();            /* Returns the name of the name of the joystick */
    char* getDevice();          /* Returns the path of the device file the joystick is attached to */
    int numAxes();              /* Returns the number of axes the joystick has */
    int numButtons();           /* Returns the number of buttons the joystick has */
};

#else                           /* If the language is C, use the c implementation */

typedef struct {                /* A Structure to house the variable required in maintaining a joystick */
    char *device;               /* Holds the path to the device file */
    int js_fd;                  /* Holds the joystick file descriptor */
    int axis_count;             /* Holds the number of axes the joystick has */
    int button_count;           /* Holds the number of buttons the joystick has */
    char name[80];              /* Holds the name of the joystick */
    int *axes;                  /* A pointer to the values of the axes of the joystick */
    char *buttons;              /* A pointer to the values of the buttons of the joystick */
    struct js_event event;      /* The joystick event structure */
} Joystick ;

Joystick* js_open ( const char* joydev );      /* Function to open the joystick from a device file */

char getJSButton( Joystick* js, int button );  /* Returns the value of a button of the joystick */

int getJSAxis( Joystick* js, int axis );       /* Returns the value of an axis of the joystick */

void js_close( Joystick* js );                 /* Closes a joystick instance */

#endif

#endif
