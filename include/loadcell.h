#ifndef _LOADCELL_H
#define _LOADCELL_H

#ifndef HX711_INITIAL_MASS
#define HX711_INITIAL_MASS    2600 // Loadcell structure mass in grams
#endif
#ifndef HX711_MAX_MASS_LIMIT
#define HX711_MAX_MASS_LIMIT  200000 // Max mass lodcell sensor in grams
#endif
#ifndef HX711_MAX_MASS_ALARM
#define HX711_MAX_MASS_ALARM  200000 * 0.8 // Alarm max mass lodcell sensor in grams
#endif
#ifndef EEPROM_SIZE
#define EEPROM_SIZE  512 // Eprom size to define in hal file
#endif

#ifdef HAS_HX711
    #include <HX711_ADC.h>      // Library HX711 (cell force) management https://github.com/olkal/HX711_ADC
    extern HX711_ADC LoadCell;

    extern float calibrationValue;     // calibration value
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
    int getCalibrationValueHX711();
    int getTareOffsetValueHX711();

    void Setup_EEPROM();
    void Setup_HX711();
    void Measure_HX711();

#endif // end HAS_HX711

#endif // end _LOADCELL_H