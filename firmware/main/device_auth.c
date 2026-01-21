#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pk.h"
#include "mbedtls/x509_crt.h"


extern const unsigned char ca_pem_start[]           asm("_binary_ca_pem_start");
extern const unsigned char ca_pem_end[]             asm("_binary_ca_pem_end");
extern const unsigned char device_cert_pem_start[]  asm("_binary_device_crt_start");
extern const unsigned char device_cert_pem_end[]    asm("_binary_device_crt_end");
extern const unsigned char device_key_pem_start[]   asm("_binary_device_key_start");
extern const unsigned char device_key_pem_end[]     asm("_binary_device_key_end");

static const char *TAG = "DEVICE_AUTH";

esp_err_t device_auth_init(void)
{
    int ret;
    const char *pers = "esp32_device_auth";

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_x509_crt ca_cert;
    mbedtls_x509_crt device_cert;
    mbedtls_pk_context device_key;

    ESP_LOGI(TAG, "Initializing mTLS authentication");

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_x509_crt_init(&ca_cert);
    mbedtls_x509_crt_init(&device_cert);
    mbedtls_pk_init(&device_key);

    ret = mbedtls_ctr_drbg_seed(
        &ctr_drbg,
        mbedtls_entropy_func,
        &entropy,
        (const unsigned char *)pers,
        strlen(pers)
    );
    if (ret != 0) {
        ESP_LOGE(TAG, "RNG seed failed: -0x%x", -ret);
        goto fail;
    }

    ESP_LOGI(TAG, "Loading CA certificate");
    ret = mbedtls_x509_crt_parse(
        &ca_cert,
        ca_pem_start,
        ca_pem_end - ca_pem_start
    );
    if (ret != 0) {
        ESP_LOGE(TAG, "CA parse failed: -0x%x", -ret);
        goto fail;
    }

    ESP_LOGI(TAG, "Loading device certificate");
    ret = mbedtls_x509_crt_parse(
        &device_cert,
        device_cert_pem_start,
        device_cert_pem_end - device_cert_pem_start
    );
    if (ret != 0) {
        ESP_LOGE(TAG, "Device certificate parse failed: -0x%x", -ret);
        goto fail;
    }

    ESP_LOGI(TAG, "Loading device private key");
    ret = mbedtls_pk_parse_key(
        &device_key,
        device_key_pem_start,
        device_key_pem_end - device_key_pem_start,
        NULL,
        0,
        mbedtls_ctr_drbg_random,
        &ctr_drbg
    );
    if (ret != 0) {
        ESP_LOGE(TAG, "Private key parse failed: -0x%x", -ret);
        goto fail;
    }

    ESP_LOGI(TAG, "mTLS credentials verified successfully");
    mbedtls_pk_free(&device_key);
    mbedtls_x509_crt_free(&device_cert);
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return ESP_OK;

fail:
    mbedtls_pk_free(&device_key);
    mbedtls_x509_crt_free(&device_cert);
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return ESP_FAIL;
}
