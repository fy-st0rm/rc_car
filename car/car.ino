#include "servo.h"
#include "sensor.h"
#include "wheels.h"
#include "config.h"

#define MOVE_SPEED 115
#define TURN_SPEED 169

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
	set_wheel_speed(wheels, LEFT , MOVE_SPEED);
	set_wheel_speed(wheels, RIGHT, MOVE_SPEED);
	switch (dir) {
		case FORWARD:
			move_forward(wheels);
			break;
		case BACKWARD:
			move_backward(wheels);
			break;
		case TURN_LEFT:
			set_wheel_speed(wheels, LEFT , TURN_SPEED);
			set_wheel_speed(wheels, RIGHT, TURN_SPEED);
			turn_left(wheels);
			break;
		case TURN_RIGHT:
			set_wheel_speed(wheels, LEFT , TURN_SPEED);
			set_wheel_speed(wheels, RIGHT, TURN_SPEED);
			turn_right(wheels);
			break;
		default:
			brake(wheels);
			break;
	}
	delay(50);
}

void setup() {
	Serial.begin(115200);

	// Sending internet info to the camera
	String msg = WIFI_NAME + " " + WIFI_PASSWORD + " " + SERVER_IP + " " + SERVER_PORT;
	Serial.println(msg);

	// Initialize the wheels.
	wheels = construct_wheels(PIN_7, PIN_8, PIN_10, PIN_9, PIN_5, PIN_6);

	// Initializing the servo.
	int servo_pins[1] = { PIN_11 };
	servo = construct_servo(servo_pins, 1);
	rotate_servo(servo, SERVO_FRONT, 90);
	
	// Initializing the sensor
	int sensor_pins[2] = { PIN_3, PIN_4 };
	sensor = construct_sensor(sensor_pins, 2);

	while (Serial.available() == 0);
	while (Serial.available() > 0) {
		int data = Serial.read();
		if (data == 69) break;
	}
	Serial.println("Setup completed.");
}

void loop() {
	int dir = 0;
	if (Serial.available() > 0) {
		String inst = Serial.readStringUntil('\n');
		int idx = inst.indexOf(' ');
		int header = inst.substring(0, idx).toInt();
		int value  = inst.substring(idx+1).toInt();
		if (header == MOVE) {
			dir = value;
		} else if (header == ANGLE) {
			rotate_servo(servo, SERVO_FRONT, value);
		}
	}

	long cm = read_sensor(sensor, SENSOR_FRONT);
	if (cm <= 15) {
		Serial.println(cm);
	}
	walk_in_dir(dir);
}
