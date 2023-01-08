#ifndef __SERVO_H__
#define __SERVO_H__

#include <Servo.h>

/*
 * @brief Enum of servo types
 */

enum SERVO {
	SERVO_LEFT,
	SERVO_RIGHT,
	SERVO_FRONT,
	SERVO_BACK,
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
 * @param servo_left: Left servo pin.
 * @param servo_right: Right servo pin.
 * @param servo_front: Front servo pin.
 * @param servo_back: Back servo pin.
 */

static servo_controls construct_servo(int servo_left, int servo_right, int servo_front, int servo_back) {
	servo_controls control;
	control.servos[SERVO_LEFT].attach(servo_left);
	control.servos[SERVO_RIGHT].attach(servo_right);
	control.servos[SERVO_FRONT].attach(servo_front);
	control.servos[SERVO_BACK].attach(servo_back);
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
