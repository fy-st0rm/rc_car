#include "servo.h"
#include "sensor.h"
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

static servo_controls servo;
static sensor_controls sensor;
static wheels_controls wheels;

void walk_in_dir(int dir) {

	switch (dir) {
		set_wheel_speed(wheels, LEFT, 69);
		set_wheel_speed(wheels, RIGHT, 69);
		case FORWARD:
			move_forward(wheels);
			break;
		case BACKWARD:
			move_backward(wheels);
			break;
		case TURN_LEFT:
			set_wheel_speed(wheels, LEFT, 150);
			set_wheel_speed(wheels, RIGHT, 150);
			turn_left(wheels);
			break;
		case TURN_RIGHT:
			set_wheel_speed(wheels, LEFT, 150);
			set_wheel_speed(wheels, RIGHT, 150);
			turn_right(wheels);
			break;
		default:
			brake(wheels);
			break;
	}
	delay(500);
}

void setup() {
	Serial.begin(115200);

	// Initialize the components.
	wheels = construct_wheels(PIN_7, PIN_8, PIN_10, PIN_9, PIN_5, PIN_6);
	//servo  = construct_servo(PIN_6, PIN_5, PIN_2, PIN_4);
	//sensor = construct_sensor(PIN_7, PIN_8, PIN_9, PIN_10, PIN_11, PIN_12, PIN_13, PIN_13);

	while (Serial.available() > 0) {
		int data = Serial.read();
		if (data == 69) break;
	}
	Serial.println("Setup completed.");
}

void loop() {
	int dir = 0;
	while (Serial.available() > 0) {
		dir = Serial.read();
		Serial.print("Received: ");
		Serial.print(dir);
		Serial.print("\n");
	}
	walk_in_dir(dir);
}
