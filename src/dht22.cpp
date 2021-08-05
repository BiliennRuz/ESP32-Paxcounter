#if (HAS_DHT)

#include "dht22.h"

// Local logging tag
static const char TAG[] = __FILE__;

dhtStatus_t dht_status = {0, 32.26, 40.28};

DHT dht22(DHT_PIN, DHT_TYPE);

// store current DHT sensor data in struct
void dht_storedata(dhtStatus_t *dht_store) {
    ESP_LOGD(TAG, "Debug DHT store data");
    if ((cfg.payloadmask & SENSOR2_DATA))// &&
       // (I2C_MUTEX_LOCK()))
    { // block i2c bus access
        dht_store->temperature = dht22.readTemperature();
        ESP_LOGD(TAG, "DHT22 Temperature: %%.2f", dht_status.temperature);
        dht_store->humidity = dht22.readHumidity();
        ESP_LOGD(TAG, "DHT22 Humidity: %%.2f", dht_status.humidity);
        dht_store->iaq = 0; // IAQ feature not present

    }
}

void setDHTIRQ() { xTaskNotify(irqHandlerTask, DHT_IRQ, eSetBits); }

int dht_init(void) {
    
    dht22.begin();
    
    return true;
}

#endif // HAS_DHT