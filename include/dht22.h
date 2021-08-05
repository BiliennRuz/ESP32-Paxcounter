#ifndef _DHT22_H
#define _DHT22_H

// DHT sensor settings
#ifndef HAS_DHT
    #define HAS_DHT 0   // DHT sensor not used = 0, used = 1
#endif
#ifndef DHT_PIN
    #define DHT_PIN 13     // Digital pin connected to the DHT sensor 
    // Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
    // Pin 15 can work but DHT must be disconnected during program upload.
#endif
#ifndef DHT_TYPE
    // Uncomment the type of sensor in use:
    //#define DHT_TYPE    DHT11     // DHT 11
    #define DHT_TYPE    DHT22       // DHT 22 (AM2302)
    //#define DHT_TYPE    DHT21     // DHT 21 (AM2301)
    // See guide for details on sensor wiring and usage:  https://learn.adafruit.com/dht/overview
#endif

/* in global.h
typedef struct {
  float iaq;             // IAQ signal
  float temperature;     // temperature signal
  float humidity;        // humidity signal
} dhtStatus_t;
*/

#include <DHT.h>
#include "globals.h"
#include "irqhandler.h"
#include "configmanager.h"

extern DHT dht22; // Make DHT instance globally availabe

extern dhtStatus_t
    dht_status; // Make struct for storing DHT sensor data globally available

void dht_storedata(dhtStatus_t *dht_store);
void setDHTIRQ();
int dht_init(void);

#endif