function tareButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "tare", true);
    xhttp.send();
}

function calibrateButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "calibrate", true);
    xhttp.send();
}

readData('readDeviceName', readDeviceName);
setInterval(readData('readStatus', readStatus), 1000);
setInterval(readData('readTemperature', readTemperature), 1001);
setInterval(readData('readHumidity', readHumidity), 1002);
setInterval(readData('readMass', readMass), 1003);
setInterval(readData('readLoadBattery', readLoadBattery), 1004);
setInterval(readData('readBatteryLife', readBatteryLife), 1005);
readData('readCalibrationValue', readCalibrationValue);
setInterval(readData('readMassAbsolute', readMassAbsolute),1006);
readData('readMassMaxAlarm', readMassMaxAlarm);
readData('readMassMaxLimit', readMassMaxLimit);
readData('readMassSwarming', readMassSwarming);

function readData(url, cFunction) {
  var xhttp;
  xhttp=new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      cFunction(this);
    }
  };
  xhttp.open("GET", url, true);
  xhttp.send();
}

function readStatus(xhttp) {
    document.getElementById("status").innerHTML = xhttp.responseText;
  }

function readTemperature(xhttp) {
  document.getElementById("temperature").innerHTML = xhttp.responseText;
}

function readHumidity(xhttp) {
    document.getElementById("humidity").innerHTML = xhttp.responseText;
}

function readMass(xhttp) {
    document.getElementById("mass").innerHTML = xhttp.responseText;
}

function readLoadBattery(xhttp) {
    document.getElementById("loadBattery").innerHTML = xhttp.responseText;
}

function readBatteryLife(xhttp) {
    document.getElementById("batteryLife").innerHTML = xhttp.responseText;
}

function readDeviceName(xhttp) {
    document.getElementById("deviceName").innerHTML = xhttp.responseText;
}

function readCalibrationValue(xhttp) {
    document.getElementById("calibration").innerHTML = xhttp.responseText;
}

function readMassAbsolute(xhttp) {
    document.getElementById("massAbsolute").innerHTML = xhttp.responseText;
}

function readMassMaxAlarm(xhttp) {
    document.getElementById("massMaxAlarm").innerHTML = xhttp.responseText;
}

function readMassMaxLimit(xhttp) {
    document.getElementById("massMaxLimit").innerHTML = xhttp.responseText;
}

function readMassSwarming(xhttp) {
    document.getElementById("massSwarming").innerHTML = xhttp.responseText;
}

function readProjectName(xhttp) {
    document.getElementById("projectName").innerHTML = xhttp.responseText;
}

function readVersionFirmware(xhttp) {
    document.getElementById("versionFirmware").innerHTML = xhttp.responseText;
}

function onloadIndex() {
    readData('readDeviceName', readDeviceName);
    readData('readStatus', readStatus);
    readData('readTemperature', readTemperature);
    readData('readHumidity', readHumidity);
    readData('readMass', readMass);
    readData('readLoadBattery', readLoadBattery);
    readData('readBatteryLife', readBatteryLife);
}

function onloadBalance() {
    readData('readDeviceName', readDeviceName);
    readData('readMass', readMass);
    readData('readCalibrationValue', readCalibrationValue);
    readData('readMassAbsolute', readMassAbsolute);
    readData('readMassMaxAlarm', readMassMaxAlarm);
    readData('readMassMaxLimit', readMassMaxLimit);
    readData('readMassSwarming', readMassSwarming);    
}

function onloadAbout() {
    readData('readDeviceName', readDeviceName);
    readData('readProjectName', readProjectName);
    readData('readVersionFirmware', readVersionFirmware);
}