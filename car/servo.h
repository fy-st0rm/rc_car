#ifndef __SERVO_H__
#define __SERVO_H__

#include <Servo.h>

/*
 * @brief Enum of servo types
 */

enum SERVO {
	SERVO_FRONT,
	SERVO_BACK,
	SERVO_LEFT,
	SERVO_RIGHT,
	SERVO_SIZE
};

/*
 * @brief Struture to hold all the servos.
 */

typedef struct {
	Servo servos[SERVO_SIZE];
} servo_controls;

/*
 * @brief Function to construct servo.
 * @param pins: Array of pins.
 * @param pin_amt: Number of pins to be attached.
 */

static servo_controls construct_servo(int* pins, int pin_amt) {
	servo_controls control;
	for (int i = 0; i < pin_amt; i++) {
		control.servos[i].attach(pins[i]);
	}
	return control;
}

/*
 * @brief Function to rotate a servo.
 * @param control: Servo controls struct.
 * @param type: Type of the servo.
 * @param angle: Angle to be rotated.
 */

static void rotate_servo(servo_controls control, SERVO type, int angle) {
	control.servos[type].write(angle);
}

#endif // __SERVO_H__
