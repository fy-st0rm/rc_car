#include "camera.h"
#include "config.h"

#define FLASH 3
#define FLASH_PIN 4

static int flash_state = 0;

static WiFiClient client;

/*
 * @brief Function to intialize wifi.
 */

void init_wifi(String wifi_name, String wifi_password) {
	WiFi.begin(wifi_name.c_str(), wifi_password.c_str());

	Serial.print("Trying to connect to wifi: ");
	Serial.println(wifi_name);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println(WiFi.status());
	Serial.println("WiFi connected");
	Serial.print("Camera IP: ");
	Serial.print(WiFi.localIP());
	Serial.println("");

	delay(1000);
}

void connect_to_server(String server_ip, int server_port) {
	Serial.print("Trying to connect to server: ");
	Serial.print(server_ip);
	Serial.print(":");
	Serial.println(server_port);

	if (!client.connect(server_ip.c_str(), server_port)) {
		Serial.println("Failed to connect to server.");
	} else {
		Serial.println("Connected to the server.");
	}
	delay(5000);
}

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);

	delay(1000);
	String data[4];
	int i = 0;

	while (Serial.available() == 0);
	while (Serial.available() > 0) {
		String msg = Serial.readStringUntil('\n');
		while (msg.length() > 0) {
			int idx = msg.indexOf(' ');
			if (idx == -1) {
				data[i++] = msg;
				break;
			} else {
				data[i++] = msg.substring(0, idx);
				msg = msg.substring(idx+1);
			}
		}
	}

	// Initialize the components.
	init_esp_camera();
	init_wifi(data[0], data[1]);
	connect_to_server(data[2], data[3].toInt());

	// Initializing flashlight
	pinMode(FLASH_PIN, OUTPUT);

	Serial.write(69);
}

void loop() {
	send_image_to_sv(&client);

	// Reading data from server and sending to arduino using serials.
	while (client.available()) {
		String inst = client.readStringUntil('\r');
		int idx = inst.indexOf(' ');
		int header = inst.substring(0, idx).toInt();
		int value  = inst.substring(idx+1, inst.length()-1).toInt();

		// Toggle flash light
		if (header == FLASH) {
			if (flash_state == 0) {
				digitalWrite(FLASH_PIN, HIGH);
				flash_state = 1;
			} else {
				digitalWrite(FLASH_PIN, LOW);
				flash_state = 0;
			}
		} else {
			Serial.println(inst);
		}
	}
}
