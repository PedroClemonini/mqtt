#include "clock.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
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
SemaphoreHandle_t ConexaoMQTTSemaphore;
static esp_netif_t *sta_netif = NULL;
static const char *TAG = "EAP";
void await_wifi_connect(void *params) {

  while (true) {
    if (xSemaphoreTake(ConexaoWifiSemaphore, portMAX_DELAY)) {
      ESP_LOGI("Main Task", "Obtem horario");
      char *date = gettimedate();
      free(date);
      mqtt_start(); 
    }
  }
}
  
void trataCom(void *parmas) {

  char mensagem[70];
  if (xSemaphoreTake(ConexaoMQTTSemaphore, portMAX_DELAY)) {
    while (true) {
    
      char *date = gettimedate();
      sprintf(mensagem, "%s:%d",date,1);
      mqtt_send("MAQ-01/state", mensagem);
      free(date);
      vTaskDelay(60000 / portTICK_PERIOD_MS);

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
  //initialise_wifi(); 
  wifi_start();
  xTaskCreate(&await_wifi_connect, "aguardando conexão", 4096, NULL, 1, NULL);
  xTaskCreate(&trataCom,"Comunicacao com broker",4096,NULL, 1,NULL);
}
