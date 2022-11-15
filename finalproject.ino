//Bridget LaBonney
//CPE 301
//Final Project

void ClockReport() {
//This gets the current real time and reports it to the Serial monitor
}

void WaterMonitorEnable(bool x) {
  if(x) {}; //endable water monitor
  else //disable water monitor
}

void IDLE() {
  //timestamp
  //turn on greenLED
  //if stop button is pressed, DISABLED()
}

void ERROR() {
  //timestamp
  //turn motor off
  //reset button -> idle
  //red led on, turn all others off
}

void stateReport() {
  //stop button -> turn fan motor off and go to DISABLED()
  //humidity and temp control report on
  //respond to vent position.
}

void RUNNING() {
  //timestamp
 //turn motor on
 //humidity/temp monitoring
 // -> idle if below water threshold
 // -> ERROR() if water is too low
//blue LED on, turn all others off
}

void DISABLED() {
  //timestamp
  //turn yellow LED on
  WaterMonitorEnable(false);
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
