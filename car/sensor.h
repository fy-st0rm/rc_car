#ifndef __MY_SENSOR_H__
#define __MY_SENSOR_H__

/*
 * @brief Enum of sensor types.
 */

enum SENSOR {
	SENSOR_FRONT,
	SENSOR_BACK,
	SENSOR_LEFT,
	SENSOR_RIGHT,
	SENSOR_SIZE
};

/*
 * @brief Function to convert miliseconds to centimeter."
 * @param ms: Miliseconds to be converted to centimeter.
 */

static long __ms_to_cm(long ms) {
	return ms / 29 / 2;
}

/*
 * @brief Structure of single sensor pins.
 */

typedef struct {
	int ping_pin, echo_pin;
} sensor_pins;

/*
 * @brief Structure of all the sensors.
 */

typedef struct {
	sensor_pins types[SENSOR_SIZE];
} sensor_controls;

/*
 * @brief Function to initialize sensor pins.
 * @param controls: sensor_controls structure.
 */

static void __init_sensors_pin(sensor_controls controls) {
	for (int i = 0; i < SENSOR_SIZE; i++) {
		sensor_pins pin = controls.types[i];
		pinMode(pin.ping_pin, OUTPUT);
		pinMode(pin.echo_pin, INPUT);
	}
}

/*
 * @brief Function to contruct sensor.
 * @param sensor_front_ping: Pin no of front sensor ping.
 * @param sensor_front_echo: Pin no of front sensor echo.
 * @param sensor_back_ping: Pin no of back sensor ping.
 * @param sensor_back_echo: Pin no of back sensor echo.
 * @param sensor_left_ping: Pin no of left sensor ping.
 * @param sensor_left_echo: Pin no of left sensor echo.
 * @param sensor_right_ping: Pin no of right sensor ping.
 * @param sensor_right_echo: Pin no of right sensor echo.
 */

static sensor_controls construct_sensor(
		int sensor_front_ping, int sensor_front_echo,
		int sensor_back_ping, int sensor_back_echo,
		int sensor_left_ping, int sensor_left_echo,
		int sensor_right_ping, int sensor_right_echo
) {
	sensor_controls controls = {
		.types = {
			[SENSOR_FRONT] = (sensor_pins) { sensor_front_ping, sensor_front_echo },
			[SENSOR_BACK ] = (sensor_pins) { sensor_back_ping, sensor_back_echo },
			[SENSOR_LEFT ] = (sensor_pins) { sensor_left_ping, sensor_left_echo },
			[SENSOR_RIGHT] = (sensor_pins) { sensor_right_ping, sensor_right_echo }
		}
	};
	__init_sensors_pin(controls);

	return controls;
}

/*
 * @brief Function to read the distance value from sensor.
 * @param control: sensor_controls struct.
 * @param type: sensor type.
 */

static long read_sensor(sensor_controls control, SENSOR type) {
	sensor_pins pin = control.types[type];

	pinMode(pin.ping_pin, OUTPUT);
	digitalWrite(pin.ping_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(pin.ping_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(pin.ping_pin, LOW);

	pinMode(pin.echo_pin, INPUT);
	long duration = pulseIn(pin.echo_pin, HIGH);
	long cm = __ms_to_cm(duration);

	return cm;
}

#endif // __SENSOR_H__
