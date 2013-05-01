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

#include <stdio.h>
#include "joystick.h"

int main ( int argc, char** argv ) {
    if( argc < 2 ) {
        fprintf( stderr, "Please specify a joystick device file\n" );
        return 1;
    }
    Joystick* joy = js_open( argv[1] );
    if( joy == NULL ) {
        fprintf( stderr, "Opening of %s failed\n", argv[1] );
        return 1;
    }
    printf( "Joystick Name: %s\n", joy->name );
    printf( "Axes count: %d\n", joy->axis_count );
    printf( "Button count: %d\n", joy->button_count );
    js_close( joy );
    return 0;
}
