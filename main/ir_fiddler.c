#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/rmt_rx.h>
#include <driver/rmt_types.h>
#include "esp_log.h"
#include "sdkconfig.h"

#define IR_RECEIVE_GPIO 26

static void configure_ir(rmt_channel_handle_t rx);
static bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data);
static void print_nec_frame(rmt_symbol_word_t *symbols, size_t num_symbols);

static const char *TAG = "ir_fiddler";

void app_main(void)
{

}

static void configure_ir(rmt_channel_handle_t rx) {
  rmt_rx_channel_config_t rx_chan_config = {
    .clk_src = RMT_CLK_SRC_DEFAULT,   // select source clock
    .resolution_hz = 1 * 1000 * 1000, // 1 MHz tick resolution, i.e., 1 tick = 1 µs
    .mem_block_symbols = 64,          // memory block size, 64 * 4 = 256 Bytes
    .gpio_num = IR_RECEIVE_GPIO,              // GPIO number
    .flags.invert_in = false,         // do not invert input signal
    .flags.with_dma = false,          // do not need DMA backend
  };
  ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_chan_config, &rx));
}

static bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data) {
  BaseType_t high_task_wakeup = pdFALSE;
  QueueHandle_t receive_queue = (QueueHandle_t) user_data;
  // send the received RMT symbols to the parser task
  xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
  // return whether any task is woken up
  return high_task_wakeup == pdTRUE;
}

static void print_nec_frame(rmt_symbol_word_t *symbols, size_t num_symbols) {
  ESP_LOGI(TAG, "Recieved %lu symbols with unsigned value", symbols->val);
  size_t tt = 0;
  uint8_t address = 0, command = 0;
  for (size_t i = 1, shift = 0; i <= 8; i++, shift++) {
    tt = symbols[i].duration0 + symbols[i].duration1;
    // ESP_LOGI(TAG, "Symbol %d\n", i + 1);
    // ESP_LOGI(TAG, "Pulse: %d, Level: %d", symbols[i].duration0, symbols[i].level0);
    // ESP_LOGI(TAG, "Space: %d, Level: %d", symbols[i].duration1, symbols[i].level1);
    ESP_LOGI(TAG, "Total transimission time: %d\n", symbols[i].duration0 + symbols[i].duration1);
    if (tt > 2200) {
      address = (1 << shift) | address;
    }
    else if (tt < 1200) {
      address = (0 << shift) | address;
    }
  }
  ESP_LOGI(TAG,"Got address %d", address);

  for (size_t i = 24, shift = 0; i <= 31; i++, shift++) {
    tt = symbols[i].duration0 + symbols[i].duration1;
    // ESP_LOGI(TAG, "Symbol %d\n", i + 1);
    // ESP_LOGI(TAG, "Pulse: %d, Level: %d", symbols[i].duration0, symbols[i].level0);
    // ESP_LOGI(TAG, "Space: %d, Level: %d", symbols[i].duration1, symbols[i].level1);
    ESP_LOGI(TAG, "Total transimission time: %d\n", symbols[i].duration0 + symbols[i].duration1);
    if (tt > 2200) {
      command = (1 << shift) | command;
    }
    else if (tt < 1200) {
      command = (0 << shift) | command;
    }
  }
  ESP_LOGI(TAG,"Got command %d", command);
}