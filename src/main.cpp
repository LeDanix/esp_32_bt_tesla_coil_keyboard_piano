#include "BluetoothA2DPSource.h"
#include <HardwareSerial.h>
#include "driver/uart.h"
#include "soc/rtc_wdt.h"

#define DEBUG_UART
#define PI 3.1415926535897932384626433832795
#define UART_BAUDRATE 115200

#ifdef DEBUG_UART
static void bt_app_uart(void *pvParameters);
#endif
double frec_map(char key_pressed);

BluetoothA2DPSource a2dp_source;
#ifdef DEBUG_UART
//UART Handler
static QueueHandle_t uart0_queue;

/* Configure parameters of UART */
#define COUNTER_DIV 32  /* Max divider */
#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};
#endif

double frec = 130.81;
//const char* device_name = "Mi True Wireless EBs Basic 2";
const char* device_name = "MH-M18";

// The supported audio codec in ESP32 A2DP is SBC. SBC audio stream is encoded
// from PCM data normally formatted as 44.1kHz sampling rate, two-channel 16-bit sample data
int32_t get_data_channels(Frame *frame, int32_t channel_len) {
    static double m_time = 0.0;
    double m_amplitude = 10000.0;  // -32,768 to 32,767
    double m_deltaTime = 1.0 / 44100.0;
    double m_phase = 0.0;
    double double_Pi = PI * 2.0;
    // fill the channel data
    for (int sample = 0; sample < channel_len; ++sample) {
        double angle = double_Pi * frec * m_time + m_phase;
        frame[sample].channel1 = m_amplitude * sin(angle);
        frame[sample].channel2 = frame[sample].channel1;
        m_time += m_deltaTime;
    }

    return channel_len;
}


void setup() {
  //a2dp_source.set_auto_reconnect(false);
  a2dp_source.start(device_name, get_data_channels);  
  a2dp_source.set_volume(100);
#ifdef DEBUG_UART
  //Install UART driver, and get the queue.
  uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
  uart_param_config(EX_UART_NUM, &uart_config);
  //Set UART pins (using UART0 default pins ie no changes.)
  uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  xTaskCreate(bt_app_uart, "bt_app_uart", 8192, NULL, 15, NULL);

  //Set Watchdog
  rtc_wdt_set_length_of_reset_signal(RTC_WDT_SYS_RESET_SIG, RTC_WDT_LENGTH_3_2us);
  rtc_wdt_set_stage(RTC_WDT_STAGE0, RTC_WDT_STAGE_ACTION_RESET_SYSTEM);
  rtc_wdt_set_time(RTC_WDT_STAGE0, 250);
  vTaskDelay(10);  // to prevent watchdog
#endif
}

void loop() {
  //TODO Delete Watchdog
  rtc_wdt_feed();
  vTaskDelay(10);  // to prevent watchdog
}

double frec_map(char key_pressed){
  switch(key_pressed){
    case 'a':
			ESP_LOGI(NOTE, " DO");
      return 261.62;
    case 'w':
			ESP_LOGI(NOTE, " DO#");
      return 277.18;
    case 's':
			ESP_LOGI(NOTE, " RE");
      return 293.66;
		case 'e':
			ESP_LOGI(NOTE, " RE#");
      return 311.127;
    case 'd':
			ESP_LOGI(NOTE, " MI");
      return 329.62;
    case 'f':
			ESP_LOGI(NOTE, " FA");
      return 349.22;
		case 'r':
			ESP_LOGI(NOTE, " FA#");
      return 370.00;
    case 'j':
			ESP_LOGI(NOTE, " SOL");
      return 391.99;
		case 'i':
			ESP_LOGI(NOTE, " SOL#");
      return 415.30;
    case 'k':
			ESP_LOGI(NOTE, " LA");
      return 440.00;
		case 'o':
			ESP_LOGI(NOTE, " LA#");
      return 440.00;
    case 'l':
			ESP_LOGI(NOTE, " SI");
      return 493.88;
		case 'A':
			ESP_LOGI(NOTE, " DO");
      return 261.62;
    case 'W':
			ESP_LOGI(NOTE, " DO#");
      return 277.18;
    case 'S':
			ESP_LOGI(NOTE, " RE");
      return 293.66;
		case 'E':
			ESP_LOGI(NOTE, " RE#");
      return 311.127;
    case 'D':
			ESP_LOGI(NOTE, " MI");
      return 329.62;
    case 'F':
			ESP_LOGI(NOTE, " FA");
      return 349.22;
		case 'R':
			ESP_LOGI(NOTE, " FA#");
      return 370.00;
    case 'J':
			ESP_LOGI(NOTE, " SOL");
      return 391.99;
		case 'I':
			ESP_LOGI(NOTE, " SOL#");
      return 415.30;
    case 'K':
			ESP_LOGI(NOTE, " LA");
      return 440.00;
		case 'O':
			ESP_LOGI(NOTE, " LA#");
      return 440.00;
    case 'L':
			ESP_LOGI(NOTE, " SI");
      return 493.88;
		default:
			return 0.0;
  }
}

#ifdef DEBUG_UART
static void bt_app_uart(void *pvParameters)
{
	/* UART Task 
		Control the serial data which provide the serial port COM.
	*/
    uart_event_t uart_event;
    uint8_t uart_data;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE); //Allocates 1024 positions of memory
    for(;;) {
        //Waiting for UART event.
        if (xQueueReceive(uart0_queue, (void * )&uart_event, (TickType_t)portMAX_DELAY)){ //Wait for a event on the queue
            bzero(dtmp, RD_BUF_SIZE); //Delete
            switch(uart_event.type) {
                case UART_DATA:  //Event of UART receving data
                    ESP_LOGI(BT_AV_TAG, "hw uart data");
                    uart_read_bytes(EX_UART_NUM, dtmp, uart_event.size, portMAX_DELAY);  //Read events
                    //uart_write_bytes(EX_UART_NUM, (const char*) dtmp, uart_event.size); //Echo
                    frec = frec_map((int)(dtmp[0]));
                    break;
                case UART_FIFO_OVF:  //Event FIFO overflow detected
                    ESP_LOGI(BT_AV_TAG, "hw fifo overflow");
                    // If fifo overflow happened, directly flush the rx buffer here 
                    // in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                case UART_BUFFER_FULL:  //Event UART ring buffer full
                    ESP_LOGI(BT_AV_TAG, "ring buffer full");
                    // If buffer full happened, directly flush the rx buffer here 
                    // in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                case UART_BREAK:  //Event UART RX break detected
                    ESP_LOGI(BT_AV_TAG, "uart rx break");
                    break;
                case UART_PARITY_ERR:  //Event UART parity check error
                    ESP_LOGI(BT_AV_TAG, "uart parity error");
                    break;
                case UART_FRAME_ERR:  //Event UART frame error
                    ESP_LOGI(BT_AV_TAG, "uart frame error");
                    break;
                default:  //Others
                    ESP_LOGI(BT_AV_TAG, "uart event type: %d", uart_event.type);
                    break;
            }
        }
        vTaskDelay(10); //Avoid problems
    }
    free(dtmp); //Release reserved memory locations
    dtmp = NULL;
    vTaskDelete(NULL);
}
#endif