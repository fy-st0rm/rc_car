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

static void __init_sensors_pin(sensor_controls controls, int size) {
	for (int i = 0; i < size; i++) {
		sensor_pins pin = controls.types[i];
		pinMode(pin.ping_pin, OUTPUT);
		pinMode(pin.echo_pin, INPUT);
	}
}

/*
 * @brief Function to contruct sensor.
 * @param pins: Array of pins.
 * @param pin_amt: Number of pins to be attached.
 */

static sensor_controls construct_sensor(int* pins, int pin_amt) {
	sensor_controls controls;

	for (int j = 0, i = 0; i < pin_amt; i += 2) {
		controls.types[j++] = (sensor_pins) { pins[i], pins[i+1] };
	}
	__init_sensors_pin(controls, pin_amt);

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
