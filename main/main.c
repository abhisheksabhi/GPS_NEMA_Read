#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include <stdio.h>

#define GPS_UART UART_NUM_1
#define GPS_RX GPIO_NUM_16
#define GPS_TX GPIO_NUM_17
#define GPS_BAUD_RATE 9600
#define BUF_SIZE 1024

#define LED GPIO_NUM_0
#define BUF_SIZE 512

void app_main(void) {
  uint8_t data[BUF_SIZE];
  // Configure UART parameters
  uart_config_t uart_config = {.baud_rate = GPS_BAUD_RATE,
                               .data_bits = UART_DATA_8_BITS,
                               .parity = UART_PARITY_DISABLE,
                               .stop_bits = UART_STOP_BITS_1,
                               .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

  // Install UART driver (UART number, RX buffer, TX buffer, event queue, queue
  // handle, interrupt flags)
  uart_driver_install(GPS_UART, BUF_SIZE * 2, 0, 0, NULL, 0);

  // Set UART parameters
  uart_param_config(GPS_UART, &uart_config);

  // Set UART pins (TX, RX, RTS, CTS)
  uart_set_pin(GPS_UART, GPS_TX, GPS_RX, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);

  // uart_enable_rx_intr(GPS_UART);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  printf("GPS UART initialized\n");
  while (1) {
    int len =
        uart_read_bytes(GPS_UART, data, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    if (len > 0) {

      data[len] = '\0';
      printf("%s", data);
      gpio_set_level(LED, 1);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      gpio_set_level(LED, 0);
    }
  }
}