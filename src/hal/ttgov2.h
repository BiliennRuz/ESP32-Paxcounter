// clang-format off
// upload_speed 921600
// board ttgo-lora32-v1

#ifndef _TTGOV2_H
#define _TTGOV2_H

#include <stdint.h>

// Hardware related definitions for TTGO V2 Board

#define HAS_LORA 1       // comment out if device shall not send data via LoRa
#define CFG_sx1276_radio 1 // HPD13A LoRa SoC

#define HAS_DISPLAY 1
//#define MY_DISPLAY_FLIP  1 // uncomment this for rotated display
#define HAS_LED NOT_A_PIN // on-board LED is wired to SCL (used by display) therefore totally useless

// disable brownout detection (needed on TTGOv2 for battery powered operation)
#define DISABLE_BROWNOUT 1 // comment out if you want to keep brownout feature

/*
// enable only if you want to store a local paxcount table on the device
#define HAS_SDCARD  0      // this board has an SD-card-reader/writer
// Pins for SD-card
#define SDCARD_CS    (13)
#define SDCARD_MOSI  (15)
#define SDCARD_MISO  (2)
#define SDCARD_SCLK  (14)
*/

// Pins for I2C interface of OLED Display
#define MY_DISPLAY_SDA (21)
#define MY_DISPLAY_SCL (22)
#define MY_DISPLAY_RST NOT_A_PIN
#define OLED_ADDR    0x3C

// Pins for LORA chip SPI interface come from board file, we need some
// additional definitions for LMIC
/*
#define LORA_RST  LMIC_UNUSED_PIN
#define LORA_IO1  (33)
#define LORA_IO2  LMIC_UNUSED_PIN

#define LORA_SCK  (5) 
#define LORA_CS   (18)
#define LORA_MISO (19)
#define LORA_MOSI (27)
#define LORA_RST  (23)
#define LORA_IRQ  (26)
#define LORA_IO1  (33)
#define LORA_IO2  (32)
*/

// Battery settings
#define BAT_MEASURE_ADC ADC1_GPIO35_CHANNEL // battery probe GPIO pin -> ADC1_CHANNEL_7
#define BAT_VOLTAGE_DIVIDER 0.66667 // voltage divider 100k/100k on board

// GPS settings
#define HAS_GPS 1 // use on board GPS
#define GPS_SERIAL 9600, SERIAL_8N1, GPIO_NUM_0, GPIO_NUM_2 // UBlox NEO 6M GPIO_NUM_12, GPIO_NUM_15 
//#define GPS_INT GPIO_NUM_34 // 30ns accurary timepulse, to be external wired on pcb: NEO 6M Pin#3 -> GPIO34

// DHT sensor settings
#define HAS_DHT 1   // DHT sensor not used = 0, used = 1
#define DHT_PIN 13     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHT_TYPE    DHT11     // DHT 11
#define DHT_TYPE    DHT22       // DHT 22 (AM2302)
//#define DHT_TYPE    DHT21     // DHT 21 (AM2301)
// See guide for details on sensor wiring and usage:  https://learn.adafruit.com/dht/overview


// LodCell HX711 settings
#define HAS_HX711 1 // Enable Loadcell HX711 sensors in general
#define HX711_DOUT  (4) 
#define HX711_SCK  (15) 


#endif
