#include <VirtualWire.h>
#include <OneWire.h>

#include <MAX6675.h>

//Thermoo Couple temperature sensor
#define tcCS_PIN 5             // CS pin on MAX6675
#define  tcSO_PIN 6              // SO pin of MAX6675
#define  tcSCK_PIN 7             // SCK pin of MAX6675
#define tcUnits 1            // Units to readout temp (0 = raw, 1 = C, 2 = F)
boolean bTCinitOK=false;
MAX6675 temp(tcCS_PIN,tcSO_PIN,tcSCK_PIN,tcUnits);

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
int TX_ID = 3; // Transmitter ID address
boolean bDebugMode=true;

typedef struct roverRemoteData// Data Structure 
{
int    TX_ID;      // Initialize a storage place for the outgoing TX ID
float    Sensor1Data;// Initialize a storage place for the first integar that you wish to Send 
float    Sensor2Data;// Initialize a storage place for the Second integar that you wish to Send
float    Sensor3Data;// Initialize a storage place for the Third integar that you wish to Send
float    Sensor4Data;// Initialize a storage place for the Forth integar that you wish to Send


};
char* msg;

void setup() {
  if(bDebugMode) Serial.begin(9600);
    if (temp.read_temp()>0){
    bTCinitOK=true;
    if (bDebugMode)Serial.println("Thermo Couple test successful");
  }
  else{
    bTCinitOK=false;
    if (bDebugMode)Serial.println("Thermo Couple test FAILED");
  }
  sensors.begin();
  
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(12);
  vw_setup(4000);
}

void loop() {
  pinMode(13,OUTPUT);
  if(bDebugMode) Serial.println("Statrt loop");
  struct roverRemoteData payload;// In this section is where you would load the data that needs to be sent.
  if(bDebugMode) Serial.println("RF reset");
  sensors.requestTemperatures();
  //Serial.println("DS Reset");

  float temperature = sensors.getTempCByIndex(0);
  if(bDebugMode) Serial.print("DS Temperature: ");
  if(bDebugMode) Serial.println(temperature);

  payload.TX_ID = TX_ID; // Set the Radio Address 
  payload.Sensor1Data =temp.read_temp();// analogRead(Sensor1Pin);
  payload.Sensor2Data =sensors.getTempCByIndex(0);// circle #2
  payload.Sensor3Data =sensors.getTempCByIndex(1);// Boiler room
  payload.Sensor4Data =sensors.getTempCByIndex(2);// Basement

  //dtostrf(temperature, 6, 2, msg);
digitalWrite(13,1);
  vw_send((uint8_t *)&payload, sizeof(payload)); // Send the data 
  vw_wait_tx();
digitalWrite(13,0);

  delay(3000);

}
