 void initialize()
 {
  // initialize Serial console
  Serial.begin(115200);
  Serial.println("GPS test code");
  Serial.println();

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's
  GPS.begin(9600);

  // turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate to 1 Hz
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  // activate TIMER0 interrupt which goes off every 1 millisecond
  enableInterrupt();
  }
