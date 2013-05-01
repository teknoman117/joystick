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

#include "joystick.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <linux/input.h>

Joystick* js_open (const char* joydev) {
    Joystick* js = malloc( sizeof(Joystick) );                      /* Allocate memory for the joystick structure */
    js->js_fd = open(joydev, O_RDONLY);                             /* Attempt to open the joystick dev file */
    if(js->js_fd < 0) {                                             /* if an error occured, report it and take action */
        fprintf( stderr, "Cannot open port %s\n", joydev);          /* Report the error */
        free(js);                                                   /* Free the memory used by the joystick structure */
        return NULL;                                                /* Return a NULL pointer */
    }
    js->device = strdup(joydev);                                    /* Get the path of the dev file and store it */
    ioctl(js->js_fd, JSIOCGAXES, &(js->axis_count));                /* Get the number of axes in the joystick */
    ioctl(js->js_fd, JSIOCGBUTTONS, &(js->button_count));           /* Get the number of buttons in the joystick */
    ioctl(js->js_fd, JSIOCGNAME(80), &(js->name));                  /* Get the name of the joystick */
    js->axes = calloc(js->axis_count, sizeof(int));                 /* Allocate memory for the axes array */
    js->buttons = calloc(js->button_count, sizeof(char));           /* Allocate memory for the buttons array */
    fcntl(js->js_fd, F_SETFL, O_NONBLOCK);                          /* Disable blocking the joystick port */
    return js;                                                      /* Return the joystick pointer */
}

char getJSButton(Joystick* js, int button) {                        /* Get the value of a button on the joystick */
    read(js->js_fd, &(js->event), sizeof(struct js_event));         /* Read the current status of the joystick */
    switch ((js->event.type) & ~JS_EVENT_INIT)  {                   /* Check if an event has occured */
        case JS_EVENT_AXIS:                                         /* If an axis event */
            (js->axes)[js->event.number] = js->event.value;         /* Update the proper axis */
            break;
        case JS_EVENT_BUTTON:                                       /* If a button event */
            (js->buttons)[js->event.number] = js->event.value;      /* Update the proper button */
            break;
    }
    if(button <= (js->button_count)) {                              /* If the specified button exists, return the value */
         return (js->buttons)[button];
    }
    return -1;                                                      /* Otherwise return -1 to show the button does not exist */
}

int getJSAxis(Joystick* js, int axis) {                             /* Get the value of an axis on the joystick */
    read(js->js_fd, &(js->event), sizeof(struct js_event));         /* Read the current status of the joystick */
    switch ((js->event.type) & ~JS_EVENT_INIT)  {                   /* Check if an event has occured */
        case JS_EVENT_AXIS:                                         /* If an axis event */
            (js->axes)[js->event.number] = js->event.value;         /* Update the proper axis */
            break;
        case JS_EVENT_BUTTON:                                       /* If a button event */
            (js->buttons)[js->event.number] = js->event.value;      /* Update the proper button */
            break;
    }
    if(axis <= (js->axis_count)) {                                 /* If the axis exists, return its value */
        return (js->axes)[axis];
    }
    return -65535;                                                  /* Otherwise return a -65535 to show the axis does not exist */
}

void js_close(Joystick* js) {                                       /* Close the joystick and free the memory used by the structure */
    if( js == NULL ) return;
    close(js->js_fd);                                               /* Close the port the joystick uses */
    free(js->device);                                               /* Free the device string */
    free(js->axes);                                                 /* Free the axes array */
    free(js->buttons);                                              /* Free the buttons array */
    free(js);                                                       /* Free the rest of the memory */
}
