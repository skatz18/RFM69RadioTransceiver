// **********************************************************************************
// 
// Receive RFM69 Radio.
//                                                       
// **********************************************************************************

#include <RFM69.h>              // https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>          // https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>                // Included with Arduino IDE
//#include <ArduinoJson.h>        // https://arduinojson.org/d
//#include <Adafruit_SleepyDog.h> // https://github.com/adafruit/Adafruit_SleepyDog


// Node and network config
#define NODEID        3    // The ID of this node (must be different for every node on network)
#define NETWORKID     100  // The network ID

// The transmision frequency of the baord. Change as needed.
#define FREQUENCY      RF69_915MHZ

// Uncomment if this board is the RFM69HW/HCW not the RFM69W/CW
#define IS_RFM69HW_HCW

// Serial board rate - just used to print debug messages
#define SERIAL_BAUD   115200

// Board and radio specific config - You should not need to edit
#define RF69_SPI_CS   11
#define RF69_RESET    A5
#define RF69_IRQ_PIN  3
#define RF69_IRQ_NUM digitalPinToInterrupt(RF69_IRQ_PIN) 

#define ENCRYPTKEY "thisis16bytes111"
RFM69 radio(RF69_SPI_CS, RF69_IRQ_PIN, false, RF69_IRQ_NUM);

// Setup
void setup() {
  Serial.begin(115200);
  // Reset the radio
  resetRadio();
  // Initialize the radio
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.promiscuous(true);
  #ifdef IS_RFM69HW_HCW
    radio.setHighPower(); //must include this only for RFM69HW/HCW!
  #endif
  //radio.encrypt(ENCRYPTKEY);
}


void loop(){
  if (radio.receiveDone()){
    Serial.print((char*)radio.DATA);
    
  }
  if (radio.ACKRequested())
      {
        radio.sendACK();
        Serial.print(" - ACK sent");
      }
  radio.receiveDone();
}

void resetRadio() {
  if (Serial) Serial.print("Resetting radio...");
  pinMode(RF69_RESET, OUTPUT);
  digitalWrite(RF69_RESET, HIGH);
  delay(20);
  digitalWrite(RF69_RESET, LOW);
  delay(500);
}
