#include "clock.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_sleep.h"
#include "freertos/idf_additions.h"
#include "freertos/semphr.h"
#include "mqtt_con.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "portmacro.h"
#include "wifi_con.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ON 1
#define OFF 0

SemaphoreHandle_t ConexaoWifiSemaphore;
SemaphoreHandle_t ConexaoMQTTSemaphore;
static esp_netif_t *sta_netif = NULL;
static const char *TAG = "EAP";
void await_wifi_connect(void *params) {

  while (true) {
    if (xSemaphoreTake(ConexaoWifiSemaphore, portMAX_DELAY)) {
      ESP_LOGI("Main Task", "Obtem horario");
      timesync();
      mqtt_start();
    }
  }
}

void gettime(char *date, int buffersize) {

  time_t now;
  struct tm timeinfo;

  time(&now);

  localtime_r(&now, &timeinfo);

  strftime(date, buffersize, "%Y-%m-%dT%H:%M:%S", &timeinfo);
}

void trataCom(void *parmas) {

  char mensagem[71];
  char date[64];
  if (xSemaphoreTake(ConexaoMQTTSemaphore, portMAX_DELAY)) {

    while (true) {
      gettime(date, sizeof(date));
      sprintf(mensagem, "%s:%d", date,ON);
      mqtt_send("MAQ-01/state", mensagem);
      vTaskDelay(600 / portTICK_PERIOD_MS);
      gettime(date, sizeof(date));
      sprintf(mensagem, "%s:%d", date,OFF);
      mqtt_send("MAQ-01/state", mensagem);
      vTaskDelay(600 / portTICK_PERIOD_MS);
      gettime(date, sizeof(date));
      sprintf(mensagem, "%s:tr_fio", date);
      mqtt_send("MAQ-01/occourrence", mensagem);
      vTaskDelay(600 / portTICK_PERIOD_MS);
      gettime(date, sizeof(date));
      sprintf(mensagem, "%s:%d", date,ON);
      mqtt_send("MAQ-01/state", mensagem);
      vTaskDelay(600 / portTICK_PERIOD_MS);
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
  ConexaoMQTTSemaphore = xSemaphoreCreateBinary();
  // initialise_wifi();
  wifi_start();
  xTaskCreate(&await_wifi_connect, "aguardando conexão", 4096, NULL, 1, NULL);
  xTaskCreate(&trataCom, "Comunicacao com broker", 4096, NULL, 1, NULL);
}
