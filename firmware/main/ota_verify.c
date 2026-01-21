#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "mbedtls/pk.h"
#include "mbedtls/md.h"

static const char *TAG = "OTA_VERIFY";
extern const unsigned char firmware_pubkey_pem_start[] asm("_binary_root_pubkey_pem_start");
extern const unsigned char firmware_pubkey_pem_end[]   asm("_binary_root_pubkey_pem_end");
extern const unsigned char firmware_sig_start[] asm("_binary_sig_bin_start");
extern const unsigned char firmware_sig_end[]   asm("_binary_sig_bin_end");
extern const unsigned char firmware_hash_start[] asm("_binary_fw_hash_bin_start");
extern const unsigned char firmware_hash_end[]   asm("_binary_fw_hash_bin_end");

esp_err_t ota_verify_firmware(void)
{
    int ret;

    mbedtls_pk_context pubkey;
    mbedtls_pk_init(&pubkey);

    ESP_LOGI(TAG, "Loading firmware verification public key");

    ret = mbedtls_pk_parse_public_key(
        &pubkey,
        firmware_pubkey_pem_start,
        firmware_pubkey_pem_end - firmware_pubkey_pem_start
    );
    if (ret != 0) {
        ESP_LOGE(TAG, "Public key parse failed: -0x%x", -ret);
        goto fail;
    }

    ESP_LOGI(TAG, "Verifying firmware signature");

    ret = mbedtls_pk_verify(
        &pubkey,
        MBEDTLS_MD_SHA256,
        firmware_hash_start,
        firmware_hash_end - firmware_hash_start,
        firmware_sig_start,
        firmware_sig_end - firmware_sig_start
    );
    if (ret != 0) {
        ESP_LOGE(TAG, "Firmware signature verification FAILED: -0x%x", -ret);
        goto fail;
    }

    ESP_LOGI(TAG, "Firmware integrity verified successfully");

    mbedtls_pk_free(&pubkey);
    return ESP_OK;

fail:
    mbedtls_pk_free(&pubkey);
    return ESP_FAIL;
}
