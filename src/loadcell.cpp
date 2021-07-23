#include "loadcell.h"

#ifdef HAS_HX711
    HX711_ADC LoadCell(HX711_DOUT, HX711_SCK);

    #include <EEPROM.h>         // Library EEPROM management (save calibration factor HX711)
    const int calVal_eepromAdress = 0;
    const int tareVal_eepromAdress = 8;

    float calibrationValue = 1;     // calibration value
    long tareOffsetValue = 10;       // tare offset value
    float valueMass = 0;            // Curent Mass value in g
    float saveValueMass;        // Last mass saved in g to check the swarming
    
	float valueMassAbsolute = 10;    // Absolute mass on the sensor in kg
    float valueMassMaxLimit = 10;    // Limit mass max on the sensor in kg
    float valueMassMaxAlarm = 1;    // Alarm mass max on the sensor in kg
    String msgMass;             // Message mass to send in g
    float valueMassSwarming(2); // Mass alert Swarming in kg

    static boolean newDataReady = 0;
    String statusDevice(""); 

    bool tareRequestHX711(0);
    bool calibrationRequestHX711(false);


void Setup_EEPROM()
{
	ESP_LOGI(TAG,"-------------- EEPROM ---------------");
	if (EEPROM.begin(EEPROM_SIZE)) {
		ESP_LOGI(TAG, "EEPROM initialize with %d", EEPROM_SIZE);
	} else {
		ESP_LOGE(TAG, "Failed to initialise EEPROM, check your board compatibility with %d",  EEPROM_SIZE); delay(1000000);
	}
}


void Setup_HX711()
{
	ESP_LOGI(TAG,"------------ HX711 sensor ----------");
	if ((HX711_DOUT != 0) and (HX711_SCK != 0)) {
		LoadCell.begin();   //set pinMode, HX711 gain and power up the HX711
		#if defined(ESP8266)|| defined(ESP32)
			EEPROM.begin(EEPROM_SIZE); // to fetch the calibration value from eeprom
		#endif
		getCalibrationValueHX711();
		getTareOffsetValueHX711();
		ESP_LOGI(TAG,"Startup load cell is complete");
	} else {
		ESP_LOGW(TAG,"PIN sensor HX711 not configured");
	}	
}


void Measure_HX711()
{
	if ((HX711_DOUT != 0) and (HX711_SCK != 0))
	{
		unsigned long tKX711Meas = millis(); // initialise time mass measurement
		//LoadCell.begin();   //set pinMode, HX711 gain and power up the HX711
		//LoadCell.powerUp(); //power up the HX711

		int sampleNumber = 0;
		while (sampleNumber <= (SAMPLES + IGN_HIGH_SAMPLE + IGN_HIGH_SAMPLE))
		{
			// check for new data/start next conversion:
			if (LoadCell.update()) newDataReady = true;

			// get smoothed value from the dataset:
			if (newDataReady) {
				valueMass = LoadCell.getData();
				ESP_LOGV(TAG, "Sample %d: mass: %0.2f g", sampleNumber, valueMass);
				newDataReady = 0;
				sampleNumber++; // increment the number of sample
			}
		}

		// Display value
		if (!isnan(valueMass))
		{
			ESP_LOGI(TAG, "Mass: %0.2f g", valueMass);
//			display2LinesOLED("Mass", String(valueMass), "g");
			msgMass = String(valueMass,1);
		}
		else
		{
//			display2LinesOLED("Mass", "-", "g");
		}

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

		// Measurement time
		tKX711Meas = millis() - tKX711Meas; // number of milliseconds passed for the mass measurement
		ESP_LOGD(TAG, "KX711 measurement time: %d ms", tKX711Meas);

		// receive command from web server to initiate tare operation:
		if (tareRequestHX711 == true)
		{
			LoadCell.tareNoDelay(); // tare asyncrone mode
			Serial.println("/!\\ tare request");
			tareRequestHX711 = false;
		}
		// check if last tare operation is complete:
		if (LoadCell.getTareStatus() == true)
		{
			tareOffsetValue = LoadCell.getTareOffset(); // recupération de la valeur de tare
			EEPROM.put(tareVal_eepromAdress, tareOffsetValue); // memorisation dans l'EEPROM
			#if defined(ESP8266)|| defined(ESP32)
				EEPROM.commit();
			#endif
			Serial.print("Tare complete; new offset: "); Serial.println(tareOffsetValue);
		}
		
		if (calibrationRequestHX711 == true)
		{
			calibrate_HX711(); //calibrate
			Serial.println("/!\\ Nouvelle calibation demandée");
		}
	}
}

int getCalibrationValueHX711()
	{
	EEPROM.get(calVal_eepromAdress, calibrationValue);
	if (calibrationValue == 0)
	{
		Serial.println("La balance n'a pas été calibrée");
	}
	else
	{
		Serial.print("La valeur de calibration de la balance est "); Serial.print(calibrationValue);
		LoadCell.setCalFactor(calibrationValue); // set du facteur de calibration
		if (LoadCell.getCalFactor() == calibrationValue)
		{
			Serial.println(" et a été corectement envoyé a la balance");
		}
		else
		{
			Serial.println(" mais n'a été corectement envoyé a la balance"); delay(5000);
		}
	}
	return calibrationValue;
}  // End int getCalibrationValueHX711()

int getTareOffsetValueHX711()
{
	EEPROM.get(tareVal_eepromAdress, tareOffsetValue);
	if (tareOffsetValue == -1) {
		Serial.println("La balance n'a pas été tarée");
	}
	else
	{
		Serial.print("L'offset de tare de la balance est "); Serial.print(tareOffsetValue);
		LoadCell.setTareOffset(tareOffsetValue); // set tare
		if (LoadCell.getTareOffset() == tareOffsetValue) {
			Serial.println(" et a été corectement envoyé a la balance");
		}
		else
		{
			Serial.println(" mais n'a été corectement envoyé a la balance"); delay(5000);
		}
	}
	return tareOffsetValue;
} // End int getTareOffsetValueHX711()

bool tareHX711()
{
	newDataReady = false;
	boolean flagTareComplete = false;
	Serial.println("Start Tare");
	/*
	LoadCell.update();
		Serial.print(LoadCell.getTareStatus());
	LoadCell.tareNoDelay();
	Serial.print(LoadCell.getTareStatus());
	while (flagTareComplete == false)
	{
		Serial.print(LoadCell.getTareStatus()); Serial.print("."); delay(1000);
		if (LoadCell.getTareStatus() == true)
		{
			Serial.println("Tare complete");
			flagTareComplete = true;
		}
	}
	*/
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
		LoadCell.update();
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

	LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
	float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

	Serial.print("New calibration value has been set to: ");
	Serial.print(newCalibrationValue);
	Serial.println(", use this as calibration value (calFactor) in your project sketch.");
	Serial.print("Save this value to EEPROM adress ");
	Serial.print(calVal_eepromAdress);
	Serial.println("? y/n");

	_resume = false;
	while (_resume == false)
	{
		if (Serial.available() > 0)
		{
			char inByte = Serial.read();
			if (inByte == 'y') {
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

#endif

