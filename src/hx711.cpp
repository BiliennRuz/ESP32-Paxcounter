#ifdef HAS_HX711 

#include "hx711.h"

// Local logging tag
static const char TAG[] = __FILE__;

hx711Status_t hx711_status = {200000, 0, 0};
Ticker hx711cycler;
HX711_ADC hx711(HX711_DOUT, HX711_SCK);

static boolean newDataReady = 0;

void setHX711IRQ() { xTaskNotify(irqHandlerTask, HX711_IRQ, eSetBits); }

int hx711_init(void) {
    int rc = 0;
	if ((HX711_DOUT != 0) and (HX711_SCK != 0))
	{
		hx711.begin();   //set pinMode, HX711 gain and power up the HX711
		if (hx711_setCalFactor(cfg.calFactor)) rc = true;
		if (hx711_setTareOffset(cfg.tareOffset)) rc = true;
  		
		  // check for new data/start next conversion:
		if (hx711.update())
		{
			rc = true;
			ESP_LOGD(TAG,"HX711 conversion is ready");
		} else {
			rc = false;
			ESP_LOGW(TAG,"HX711 conversion is not ready, check MCU>HX711 wiring and pin designations");
		}
	} else {
		ESP_LOGW(TAG,"PIN sensor HX711 not configured");
		rc = false;
	}
	if (rc)
    	hx711cycler.attach(HX711CYCLE, setHX711IRQ); // start cyclic data transmit

    return rc;
}

bool hx711_setCalFactor(float calibrationValue)
{
	bool rc = false;
	ESP_LOGI(TAG,"The calibration factor in memory is %0.2f", calibrationValue);
	if (calibrationValue == (float)0 || isnan(calibrationValue))
	{
		ESP_LOGW(TAG,"/!\\ The scale is not calibrated, carry out the calibration step");
		rc = false;
		return rc;
	}
	hx711.setCalFactor(calibrationValue); // set calibration factor o hx711
	// Check if the calibration factor is sent to the scale correctly
	if (hx711.getCalFactor() == calibrationValue)
	{
		ESP_LOGI(TAG,"And has been correctly sent to the scale");
		rc = true;
	}
	else
	{
		ESP_LOGW(TAG, "But has not been correctly sent to the scale");
		rc = false;
	}
	return rc;
}  // End int hx711_setCalFactor(float calibrationValue)

bool hx711_setTareOffset(long tareOffsetValue)
{
	bool rc = false;
	ESP_LOGI(TAG, "The tare offset of the scale in memory is %0.2f,", tareOffsetValue);
	if (tareOffsetValue <= (long)0)
	{
		ESP_LOGW(TAG, "/!\\ The scale has not been tared, carry out the taring step");
		rc = false;
		return rc;
	}
	hx711.setTareOffset(tareOffsetValue); // set tare to scale
	// Check if the tare offset is sent to the scale correctly
	if (hx711.getTareOffset() == tareOffsetValue)
	{
		ESP_LOGI(TAG, "And has been correctly sent to the scale");
		rc = true;
	}
	else
	{
		ESP_LOGW(TAG, "But has not been correctly sent to the scale");
		delay(5000);
		rc = false;
	}
	return rc;
} // End int hx711_setTareOffset(float tareOffsetValue)


// store current Loadcell sensor data in struct
void hx711_storedata(hx711Status_t *hx711_store) {
    ESP_LOGD(TAG, "Debug hx711 store data");
    if ((cfg.payloadmask & SCALE_DATA))
    {
        hx711_store->mass = hx711_measure();
        ESP_LOGD(TAG, "Loadcell Mass: %.2f g", hx711_status.mass);
//        hx711_getTareOffset();
//		hx711_store->tareOffset = hx711.readHumidity();
        ESP_LOGD(TAG, "Loadcell Tare offset: %.2f", hx711_status.tareOffset);
//        hx711_getCalibration();
//		hx711_store->calibrationFactor = 0; // IAQ feature not present
        ESP_LOGD(TAG, "Loadcell Calibration factor: %.2f", hx711_status.calibrationFactor);
    }
}

float hx711_measure()
{
	unsigned long tHX711Meas = millis(); // initialise time mass measurement

	int sampleNumber = 0;
	float mass = 0;
	while (sampleNumber <= (SAMPLES + IGN_HIGH_SAMPLE + IGN_HIGH_SAMPLE))
	{
		// check for new data/start next conversion:
		if (hx711.update()) newDataReady = true;

		// get smoothed value from the dataset:
		if (newDataReady) {
			mass = hx711.getData();
			ESP_LOGV(TAG, "Sample %d: mass: %0.2f g", sampleNumber, valueMass);
			newDataReady = 0;
			sampleNumber++; // increment the number of sample
		}
	}

	// Measurement time
	tHX711Meas = millis() - tHX711Meas; // number of milliseconds passed for the mass measurement
	ESP_LOGD(TAG, "KX711 measurement time: %d ms", tHX711Meas);

	return mass;
}




  
/*

 //   float calibrationValue = 0;     // calibration value
  //  long tareOffsetValue = 10;       // tare offset value
    float valueMass = 0;            // Curent Mass value in g
    float saveValueMass;        // Last mass saved in g to check the swarming
    
	float valueMassAbsolute = 10;    // Absolute mass on the sensor in kg
    float valueMassMaxLimit = 10;    // Limit mass max on the sensor in kg
    float valueMassMaxAlarm = 1;    // Alarm mass max on the sensor in kg
    String msgMass;             // Message mass to send in g
    float valueMassSwarming(2); // Mass alert Swarming in kg

    String statusDevice(""); 

    bool tareRequestHX711(0);
    bool calibrationRequestHX711(false);




void HX711_alarm()
{
		// Alarme mass
		if (!isnan(valueMass))
		{               
			valueMassAbsolute = (HX711_INITIAL_MASS + valueMass) / 1000;
			ESP_LOGD(TAG, "Mass absolute: %0.2f Kg", valueMassAbsolute);
			valueMassMaxAlarm = HX711_MAX_MASS_ALARM / 1000;
			ESP_LOGD(TAG, "Mass max alarm: %0.2f Kg", valueMassMaxAlarm);
			valueMassMaxLimit = HX711_MAX_MASS_LIMIT / 1000;
			ESP_LOGD(TAG, "Mass max limit: %0.2f Kg", valueMassMaxLimit);

			ESP_LOGD(TAG, "Mass detection for swarming bee: %0.2f Kg", valueMassSwarming);

			if (valueMassAbsolute > valueMassMaxAlarm)
			{
				statusDevice = "La balance a atteint " + String((valueMassAbsolute * 100) / valueMassMaxLimit, 0) + "% de son poid maximum";
			}
			if ((saveValueMass - valueMass) > (valueMassSwarming * 1000))
			{
				statusDevice = statusDevice + "Un esseimage a été détecté";
				ESP_LOGI(TAG, "Un esseimage a été détecté");
			}
			saveValueMass = valueMass;
		}
}

void HX711_Tare()
{

	// receive command from web server to initiate tare operation:
	if (tareRequestHX711 == true)
	{
		hx711.tareNoDelay(); // tare asyncrone mode
		Serial.println("/!\\ tare request");
		tareRequestHX711 = false;
	}
	// check if last tare operation is complete:
	if (hx711.getTareStatus() == true)
	{
		tareOffsetValue = hx711.getTareOffset(); // recupération de la valeur de tare
		//EEPROM.put(tareVal_eepromAdress, tareOffsetValue); // memorisation dans l'EEPROM
		//#if defined(ESP8266)|| defined(ESP32)
		//	EEPROM.commit();
		//#endif
		Serial.print("Tare complete; new offset: "); Serial.println(tareOffsetValue);
	}
	
	if (calibrationRequestHX711 == true)
	{
		calibrate_HX711(); //calibrate
		Serial.println("/!\\ Nouvelle calibation demandée");
	}
}



bool tareHX711()
{
	newDataReady = false;
	boolean flagTareComplete = false;
	Serial.println("Start Tare");
	
	hx711.update();
		Serial.print(hx711.getTareStatus());
	hx711.tareNoDelay();
	Serial.print(hx711.getTareStatus());
	while (flagTareComplete == false)
	{
		Serial.print(hx711.getTareStatus()); Serial.print("."); delay(1000);
		if (hx711.getTareStatus() == true)
		{
			Serial.println("Tare complete");
			flagTareComplete = true;
		}
	}
	
	return flagTareComplete;
}  // End bool tareHX711()


void calibrate_HX711()
{
	Serial.println("***");
	Serial.println("Start calibration:");
	Serial.println("Place the load cell an a level stable surface.");
	Serial.println("Remove any load applied to the load cell.");
	Serial.println("Send 't' from serial monitor to set the tare offset.");
	
	boolean _resume = false;
	tareHX711();

	Serial.println("Now, place your known mass on the loadcell.");
	Serial.println("Then send the mass of this mass in grams (i.e. 1000.0) from serial monitor.");

	float known_mass = 0;
	_resume = false;
	while (_resume == false)
	{
		hx711.update();
		if (Serial.available() > 0)
		{
			known_mass = Serial.parseFloat();
			if (known_mass != 0)
			{
				Serial.print("Known mass is: ");
				Serial.print(known_mass);
				Serial.println(" g");
				_resume = true;
			}
		}
	}

	hx711.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
	float newCalibrationValue = hx711.getNewCalibration(known_mass); //get the new calibration value

	Serial.print("New calibration value has been set to: ");
	Serial.print(newCalibrationValue);
	Serial.println(", use this as calibration value (calFactor) in your project sketch.");
	Serial.print("Save this value to EEPROM adress ");
//	Serial.print(calVal_eepromAdress);
	Serial.println("? y/n");

	_resume = false;
	while (_resume == false)
	{
		if (Serial.available() > 0)
		{
			char inByte = Serial.read();
			if (inByte == 'y')
			{
				/*
				#if defined(ESP8266)|| defined(ESP32)
					EEPROM.begin(512);
				#endif
				EEPROM.put(calVal_eepromAdress, newCalibrationValue);
				#if defined(ESP8266)|| defined(ESP32)
					EEPROM.commit();
				#endif
				EEPROM.get(calVal_eepromAdress, newCalibrationValue);
				Serial.print("Value ");
				Serial.print(newCalibrationValue);
				Serial.print(" saved to EEPROM address: ");
				Serial.println(calVal_eepromAdress);
				_resume = true;
			}
			else if (inByte == 'n')
			{
				Serial.println("Value not saved to EEPROM");
				_resume = true;
			}

		}
	}

	Serial.println("End calibration");
	Serial.println("***");
	Serial.println("To re-calibrate, send 'r' from serial monitor.");
	//Serial.println("For manual edit of the calibration value, send 'c' from serial monitor.");
	Serial.println("***");
}  // End void calibrate_HX711()

*/

#endif // HAS_HX711 
