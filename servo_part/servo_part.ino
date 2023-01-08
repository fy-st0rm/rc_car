#include "servo.h"
#include "sensor.h"

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

void setup() {
	Serial.begin(9600);
	servo = construct_servo(PIN_4, PIN_5, PIN_2, PIN_3);
	sensor = construct_sensor(PIN_6, PIN_7, PIN_8, PIN_9, PIN_10, PIN_11, PIN_12, PIN_13);
}

void servo_test() {
	rotate_servo(servo, SERVO_FRONT, 180);
	rotate_servo(servo, SERVO_BACK, 180);
	delay(1000);
	rotate_servo(servo, SERVO_FRONT, 0);
	rotate_servo(servo, SERVO_BACK, 0);
	delay(1000);
}

void sensor_test() {
	long cm_front = read_sensor(sensor, SENSOR_FRONT);
	long cm_back  = read_sensor(sensor, SENSOR_BACK);
	char buffer[100];
	sprintf(buffer, "%04ld %04ld\0", cm_front, cm_back);
	Serial.write(buffer, 11);
}

void loop() {
	sensor_test();
	delay(1000);
}
