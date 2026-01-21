#include "esp_log.h"
#include "esp_system.h"
#include "device_auth.h"
#include "ota_verify.h"

static const char *TAG = "APP_MAIN";
void app_main(void)
{
    ESP_LOGI(TAG, "Secure IoT device booting");

    if (device_auth_init() != ESP_OK) {
        ESP_LOGE(TAG, "Device authentication failed");
        esp_restart();
    }
    if (ota_verify_firmware() != ESP_OK) {
    ESP_LOGE(TAG, "Firmware verification failed, rebooting");
    esp_restart();
}


    ESP_LOGI(TAG, "Device authenticated successfully");
    ESP_LOGI(TAG, "System initialization complete");
}
