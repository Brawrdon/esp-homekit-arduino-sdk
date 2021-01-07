#include <Arduino.h>
#include <Wifi.h>
#include <ESP32HomeKit.h>

const char *ssid = "yourNetworkName";
const char *password = "yourNetworkPassword";

/* Mandatory identify routine for the accessory.
 * In a real accessory, something like LED blink should be implemented
 * got visual identification
 */
static int identify(hap_acc_t *ha)
{
	ESP_LOGI(TAG, "Accessory identified");
	return HAP_SUCCESS;
}

/* A dummy callback for handling a read on the "Direction" characteristic of Fan.
 * In an actual accessory, this should read from hardware.
 * Read routines are generally not required as the value is available with th HAP core
 * when it is updated from write routines. For external triggers (like fan switched on/off
 * using physical button), accessories should explicitly call hap_char_update_val()
 * instead of waiting for a read request.
 */
static int fan_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
	if (hap_req_get_ctrl_id(read_priv))
	{
		ESP_LOGI(TAG, "Received read from %s", hap_req_get_ctrl_id(read_priv));
	}
	if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_ROTATION_DIRECTION))
	{
		/* Read the current value, toggle it and set the new value.
        * A separate variable should be used for the new value, as the hap_char_get_val()
        * API returns a const pointer
        */
		const hap_val_t *cur_val = hap_char_get_val(hc);

		hap_val_t new_val;
		if (cur_val->i == 1)
		{
			new_val.i = 0;
		}
		else
		{
			new_val.i = 1;
		}
		hap_char_update_val(hc, &new_val);
		*status_code = HAP_STATUS_SUCCESS;
	}
	return HAP_SUCCESS;
}

/* A dummy callback for handling a write on the "On" characteristic of Fan.
 * In an actual accessory, this should control the hardware
 */
static int fan_write(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv)
{
	if (hap_req_get_ctrl_id(write_priv))
	{
		ESP_LOGI(TAG, "Received write from %s", hap_req_get_ctrl_id(write_priv));
	}

	ESP_LOGI(TAG, "Fan Write called with %d chars", count);
	int i, ret = HAP_SUCCESS;
	hap_write_data_t *write;
	for (i = 0; i < count; i++)
	{
		write = &write_data[i];
		if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON))
		{
			ESP_LOGI(TAG, "Received Write. Fan %s", write->val.b ? "On" : "Off");

			/* TODO: Control Actual Hardware */
			hap_char_update_val(write->hc, &(write->val));
			*(write->status) = HAP_STATUS_SUCCESS;
		}
		else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ROTATION_DIRECTION))
		{
			if (write->val.i > 1)
			{
				*(write->status) = HAP_STATUS_VAL_INVALID;
				ret = HAP_FAIL;
			}
			else
			{
				ESP_LOGI(TAG, "Received Write. Fan %s", write->val.i ? "AntiClockwise" : "Clockwise");
				hap_char_update_val(write->hc, &(write->val));
				*(write->status) = HAP_STATUS_SUCCESS;
			}
		}
		else
		{
			*(write->status) = HAP_STATUS_RES_ABSENT;
		}
	}
	return ret;
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

	hap_acc_t *accessory;
	hap_serv_t *service;

	/* Configure HomeKit core to make the Accessory name (and thus the WAC SSID) unique,
     * instead of the default configuration wherein only the WAC SSID is made unique.
     */
	hap_cfg_t hap_cfg;
	hap_get_config(&hap_cfg);
	hap_cfg.unique_param = UNIQUE_NAME;
	hap_set_config(&hap_cfg);

	/* Initialize the HAP core */
	hap_init(HAP_TRANSPORT_WIFI);

	/* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
	hap_acc_cfg_t cfg = {
		.name = "Esp-Fan",
		.model = "Espressif",
		.manufacturer = "EspFan01",
		.serial_num = "001122334455",
		.fw_rev = "0.0.1",
		.hw_rev = NULL,
		.pv = "1.1.0",
		.cid = HAP_CID_FAN,
		.identify_routine = identify,
	};

	/* Create accessory object */
	accessory = hap_acc_create(&cfg);

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
	/* Unique four character Setup Id. Default: ES32 */
	hap_set_setup_id("ES32");

	/* After all the initializations are done, start the HAP core */
	hap_start();
}

void loop()
{
	/* Main loop code */
}