/*
 * main.c
 *
 *  Created on: Oct 30, 2018
 *      Author: johno
 */
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "bootwifi.h"
#include "ddns.h"
#include "oap_common.h"
#include "oap_storage.h"
#include "server_cpanel.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char *TAG = "app";

static void update_loop() {
	for(;;) {
		ddns_update();
		vTaskDelay(600000 / portTICK_PERIOD_MS);
	}
}

void app_main() {
	ESP_LOGI(TAG, "Starting application... firmware %s", oap_version_str());

	storage_init();

	wifi_configure(storage_get_config("wifi"), cpanel_wifi_handler);
	wifi_boot();

	delay(5000);
	ddns_configure(storage_get_config("ddns"));
	update_loop();
}
