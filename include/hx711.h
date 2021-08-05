#ifndef _LOADCELL_H
#define _LOADCELL_H

/* in global.h
typedef struct {
  float mass;               // Curent Mass value in g
  float tareOffset;         // tare offset value
  float calibrationFactor;  // calibration factor value
} hx711Status_t;
*/

#ifdef HAS_HX711

    #ifndef HX711_DOUT
        #defined HX711_DOUT           0
    #endif
    #ifndef HX711_SCK
        #defined HX711_SCK            0
    #endif
    #ifndef HX711_CALIB_FACTOR
        #define HX711_CALIB_FACTOR          0                   // Calibration factor
    #endif
    #ifndef HX711_TARE_OFFSET 
        #define HX711_TARE_OFFSET           0                   // Tare offset
    #endif
    #ifndef HX711_INITIAL_MASS
        #define HX711_INITIAL_MASS    2600 // Loadcell structure mass in grams
    #endif
    #ifndef HX711_MAX_MASS_LIMIT
        #define HX711_MAX_MASS_LIMIT  200000 // Max mass lodcell sensor in grams
    #endif
    #ifndef HX711_MAX_MASS_ALARM
        #define HX711_MAX_MASS_ALARM  200000 * 0.8 // Alarm max mass lodcell sensor in grams
    #endif

    #include <HX711_ADC.h>      // Library HX711 (cell force) management https://github.com/olkal/HX711_ADC
    #include "globals.h"
    #include "irqhandler.h"
    #include "configmanager.h"

    #define HX711CYCLE               10       // hx711 sensor read cycle in seconds [default = 1 secs]

    extern HX711_ADC hx711;
    extern hx711Status_t hx711_status; // Make struct for storing loadcell sensor data globally available

    void setHX711IRQ();
    int hx711_init(void);
    bool hx711_setCalFactor(float calibrationValue);
    bool hx711_setTareOffset(long tareOffsetValue);
    void hx711_storedata(hx711Status_t *hx711_store);
    float hx711_measure();


    //int hx711_getCalibration();
    //int hx711_getTareOffset();



 /*   extern float calibrationValue;     // calibration value
    extern long tareOffsetValue;       // tare offset value
    extern float valueMass;            // Curent Mass value in g
    extern float saveValueMass;        // Last mass saved in g
    extern float valueMassAbsolute;    // Absolute mass on the sensor in kg
    extern float valueMassMaxLimit;    // Limit mass max on the sensor in kg
    extern float valueMassMaxAlarm;    // Alarm mass max on the sensor in kg
    extern String msgMass;             // Message mass to send in g
    extern float valueMassSwarming; // Mass alert Swarming in kg
	extern bool tareRequestHX711;
    extern bool calibrationRequestHX711;
    extern String statusDevice; 

    void calibrate_HX711();
    */

#endif // end HAS_HX711

#endif // end _LOADCELL_H