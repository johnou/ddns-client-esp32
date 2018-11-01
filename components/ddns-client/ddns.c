/*
 * main.c
 *
 *  Created on: Oct 30, 2018
 *      Author: johno
 */
#include "oap_common.h"
#include "cJSON.h"
#include "oap_storage.h"
#include "ddns.h"
#include "esp_http_client.h"

static const char *TAG = "ddns";

bool initialized;
char ddns_url[192];

esp_err_t ddns_configure(cJSON* ddns) {
	if (!ddns) {
		ESP_LOGI(TAG, "not configured...");
		return ESP_FAIL;
	}
	cJSON* field;
	if ((field = cJSON_GetObjectItem(ddns, "url")) && field->valuestring) {
		strcpy(ddns_url, field->valuestring);
	} else {
		ESP_LOGW(TAG, "url not configured...");
		return ESP_FAIL;
	}
	if (strlen(ddns_url) == 0) {
		ESP_LOGW(TAG, "url not configured...");
		return ESP_FAIL;
	}

	ESP_LOGI(TAG, "url configured with '%s'", ddns_url);
	initialized = true;
	return ESP_OK;
}

void ddns_update() {
	if (!initialized) {
		//ESP_LOGW(TAG, "url not configured...");
		return;
	}
	//ESP_LOGI(TAG, "Performing update with DDNS url '%s'", ddns_url);
	esp_http_client_config_t config;
	memset(&config, 0, sizeof(config));
	config.url = ddns_url;
	config.timeout_ms = 10000;
	esp_http_client_handle_t client = esp_http_client_init(&config);

	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		//ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
		//esp_http_client_get_status_code(client),
		//esp_http_client_get_content_length(client));
	} else {
		ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
	}

	esp_http_client_cleanup(client);
}
