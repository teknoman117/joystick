/*   Copyright (C) 2009  Nathaniel <linux.robotdude@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "joystick.h"

int main ( int argc, char** argv ) {
    if( argc < 2 ) {
        fprintf( stderr, "Error: No Device Specified!\n");
        return 1;
    }
    Joystick joy(argv[1]);
    if(!joy.connected) {
        fprintf( stderr, "Error: Joystick did not open!\n");
        return 1;
    }
    printf( "Name: %s\n", joy.getName() );
    printf( "Axes: %d\n", joy.numAxes() );
    printf( "Buttons: %d\n", joy.numButtons() );
    joy.stop();
    return 0;
}
