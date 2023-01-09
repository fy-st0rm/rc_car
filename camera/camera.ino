#include "camera.h"
#include "config.h"

static WiFiClient client;

/*
 * @brief Function to intialize wifi.
 */

void init_wifi() {
	WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

	Serial.print("Trying to connect to wifi: ");
	Serial.println(WIFI_NAME);

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

	Serial.print("Trying to connect to server: ");
	Serial.print(SERVER_IP);
	Serial.print(":");
	Serial.println(SERVER_PORT);

	delay(1000);
	if (!client.connect(SERVER_IP, SERVER_PORT)) {
		Serial.println("Failed to connect to server.");
	} else {
		Serial.println("Connected to the server.");
	}
	delay(1000);
}

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);

	// Initialize the components.
	init_esp_camera();
	init_wifi();

	Serial.write(69);
}

void loop() {
	send_image_to_sv(&client);

	delay(100);

	// Reading data from server and sending to arduino using serials.
	while (client.available()) {
		String reply = client.readStringUntil('\r');
		int dir = reply.toInt();
		Serial.write(dir);
	}
}
