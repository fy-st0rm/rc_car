#ifndef __WHEELS_H__
#define __WHEELS_H__

#define FORWARD 1
#define BACKWARD 2
#define TURN_LEFT 3
#define TURN_RIGHT 4

/*
 * @brief Enum of direction.
 */
enum DIRS {
	LEFT,
	RIGHT
};

/*
 * @brief Enum of wheel type.
 */

enum WHEELS {
	LEFT_1,
	LEFT_2,
	RIGHT_1,
	RIGHT_2,
	WHEEL_SIZE
};

/*
 * @brief Structure of wheels_controls.
 * @member types: Array of wheel types.
 * @member speed_pins: Array of speed pins.
 */

typedef struct {
	int types[WHEEL_SIZE];
	int speed_pins[2];
} wheels_controls;

/*
 * @brief Function to construct wheels control structure.
 * @param left_1: PIN number for left wheels forward.
 * @param left_2: PIN number for left wheels backward.
 * @param right_1: PIN number for right wheels forward.
 * @param right_2: PIN number for right wheels backward.
 * @param left_speed_pin: PIN number for speed input for left.
 * @param right_speed_pin: PIN number for speed input for right.
 */

static wheels_controls construct_wheels(
	int left_1, int left_2,
	int right_1, int right_2,
	int left_speed_pin, int right_speed_pin
) {
	pinMode(left_1, OUTPUT);
	pinMode(left_2, OUTPUT);
	pinMode(right_1, OUTPUT);
	pinMode(right_2, OUTPUT);
	pinMode(left_speed_pin, OUTPUT);
	pinMode(right_speed_pin, OUTPUT);

	return (wheels_controls) {
		.types = {
			[LEFT_1]  = left_1,
			[LEFT_2]  = left_2,
			[RIGHT_1] = right_1,
			[RIGHT_2] = right_2
		},
		.speed_pins = {
			[LEFT]  = left_speed_pin,
			[RIGHT] = right_speed_pin
		}
	};
}

/*
 * @brief Function to activate assigned wheel.
 * @param wheels: Wheels control structure.
 * @param type: Type of wheel.
 */

static void activate_wheel(wheels_controls wheels, WHEELS type) {
	digitalWrite(wheels.types[type], HIGH);
}

/*
 * @brief Function to deactivate assigned wheel.
 * @param wheels: Wheels control structure.
 * @param type: Type of wheel.
 */

static void deactivate_wheel(wheels_controls wheels, WHEELS type) {
	digitalWrite(wheels.types[type], LOW);
}

/*
 * @brief Function to assign speed of wheels.
 * @param wheels: Wheels control structure.
 * @param dir: Side of speed.
 * @param speed: Magnitude of speed.
 */

static void set_wheel_speed(wheels_controls wheels, DIRS dir, int speed) {
	analogWrite(wheels.speed_pins[dir], speed);
}

/*
 * Movement Functions
 */

static void turn_left(wheels_controls wheels) {
	activate_wheel(wheels, RIGHT_1);
	deactivate_wheel(wheels, RIGHT_2);

	activate_wheel(wheels, LEFT_2);
	deactivate_wheel(wheels, LEFT_1);
}

static void turn_right(wheels_controls wheels) {
	activate_wheel(wheels, LEFT_1);
	deactivate_wheel(wheels, LEFT_2);

	activate_wheel(wheels, RIGHT_2);
	deactivate_wheel(wheels, RIGHT_1);
}

static void move_forward(wheels_controls wheels) {
	activate_wheel(wheels, RIGHT_1);
	deactivate_wheel(wheels, RIGHT_2);

	activate_wheel(wheels, LEFT_1);
	deactivate_wheel(wheels, LEFT_2);
}

static void move_backward(wheels_controls wheels) {
	activate_wheel(wheels, LEFT_2);
	deactivate_wheel(wheels, LEFT_1);

	activate_wheel(wheels, RIGHT_2);
	deactivate_wheel(wheels, RIGHT_1);
}

static void brake(wheels_controls wheels) {
	deactivate_wheel(wheels, LEFT_2);
	deactivate_wheel(wheels, LEFT_1);

	deactivate_wheel(wheels, RIGHT_2);
	deactivate_wheel(wheels, RIGHT_1);
}

#endif// __WHEELS_H__
