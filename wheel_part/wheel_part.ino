#include "wheels.h"

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
	PIN_12,
	PIN_13
};

static wheels_controls wheels;
static int dir = 1; // 0 = back, 1 = forward

void setup() {
	Serial.begin(9600);
	wheels = construct_wheels(PIN_7, PIN_8, PIN_10, PIN_9, PIN_5, PIN_6);
}

void walk_test() {
	set_wheel_speed(wheels, LEFT, 90);
	set_wheel_speed(wheels, RIGHT, 90);

	move_forward(wheels);
	delay(1000);
	turn_left(wheels);
	delay(500);
}

void walk_test_with_sensor() {
	if (Serial.available() > 0) {
		char buffer[100];
		Serial.readBytes(buffer, 11);

		long front = atoi(strtok(buffer, " "));
		long back = atoi(strtok(NULL, " "));

		if (front <= 15) dir = 0;
		if (back <= 15)  dir = 1;

	}
	set_wheel_speed(wheels, LEFT, 90);
	set_wheel_speed(wheels, RIGHT, 90);

	switch (dir) {
		case 0:
			move_backward(wheels);
			break;
		case 1:
			move_forward(wheels);
			break;
	}
	delay(1000);
}

void loop() {
	walk_test_with_sensor();
}
