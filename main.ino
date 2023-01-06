#include "wheels.h"
#include "servo.h"

enum PINS {
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12
};

static wheels_controls wheels;

void setup() {
	Serial.begin(9600);
	wheels = construct_wheels(PIN_2, PIN_3, PIN_4, PIN_5, PIN_10, PIN_11);
}

void loop() {
	set_wheel_speed(wheels, LEFT, 90);
	set_wheel_speed(wheels, RIGHT, 90);

	turn_right(wheels);
}
