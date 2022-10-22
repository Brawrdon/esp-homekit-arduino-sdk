#include <Arduino.h>
#include <WiFi.h>
#include <ESP32HomeKit.h>

const char *ssid = "yourNetworkName";
const char *password = "yourNetworkPassword";


int Indentification(AccessoryHandle* accessoryHandle) {
	Serial.println("Establishing connection to WiFi..");
}

void setup()
{
	Serial.begin(115200);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Establishing connection to WiFi..");
	}

	Serial.println("Connected to network.");

	HAPAccessory hapAccessory("ESP32 Fan");
	
	hapAccessory.InitialiseHAP("EspFan01", "Espressif", "001122334455", "0.0.1", "1.0.0", Indentification, HAP_CID_FAN);

	/* Add a dummy Product Data */
	uint8_t product_data[] = {'E', 'S', 'P', '3', '2', 'K', 'I', 'T'};
	hap_acc_add_product_data(accessory, product_data, sizeof(product_data));

	/* Create the Fan Service. Include the "name" since this is a user visible service  */
	service = hap_serv_fan_create(false);
	hap_serv_add_char(service, hap_char_name_create("My Fan"));
	hap_serv_add_char(service, hap_char_rotation_direction_create(0));

	/* Set the write callback for the service */
	hap_serv_set_write_cb(service, fan_write);

	/* Set the read callback for the service (optional) */
	hap_serv_set_read_cb(service, fan_read);

	/* Add the Fan Service to the Accessory Object */
	hap_acc_add_serv(accessory, service);

	/* Add the Accessory to the HomeKit Database */
	hap_add_accessory(accessory);

	/* Query the controller count (just for information) */
	ESP_LOGI(TAG, "Accessory is paired with %d controllers",
			 hap_get_paired_controller_count());

	/* TODO: Do the actual hardware initialization here */

	/* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
	hap_set_setup_code("111-22-333");

	/* After all the initializations are done, start the HAP core */
	hap_start();
}

void loop()
{
	/* Main loop code */
}
