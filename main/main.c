#include "clock.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/idf_additions.h"
#include "freertos/semphr.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "portmacro.h"
#include "wifi_con.h"
#include "mqtt_con.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
SemaphoreHandle_t ConexaoWifiSemaphore;
//SemaphoreHandle_t ConexaoMQTTSemaphore;

void await_wifi_connect(void *params) {

  while (true) {
    if (xSemaphoreTake(ConexaoWifiSemaphore, portMAX_DELAY)) {
      ESP_LOGI("Main Task", "Obtem horario");
      gettimedate();
      //mqtt_start(); 
    }
  }
}

void app_main(void) {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  ESP_ERROR_CHECK(ret);
  ConexaoWifiSemaphore = xSemaphoreCreateBinary();
  //ConexaoMQTTSemaphore = xSemaphoreCreateBinary();
  wifi_start();
  xTaskCreate(&await_wifi_connect, "aguardando conexão", 4096, NULL, 1, NULL);
}
