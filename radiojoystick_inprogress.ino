// **********************************************************************************
// 
// joystick RF control
//                                                       
// **********************************************************************************

#include <RFM69.h>              // https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>          // https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>                // Included with Arduino IDE


// Node and network config
#define NODEID        1    // The ID of this node (must be different for every node on network)
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
#define yPin  A1
#define xPin  A2
#define btPin  6

struct joystick{
  uint8_t btnval; //value of buttone
  uint8_t xmostsig; //bits 9-10 for xvalue
  uint8_t xleastsig; //bits 1-8 for xvalue
  uint8_t ymostsig; //bits 9-10 for yvalue
  uint8_t yleastsig; //bits 1-8 for yvalue
};

//#define ENCRYPTKEY "thisis16bytes111"
RFM69 radio(RF69_SPI_CS, RF69_IRQ_PIN, false);

// Setup
void setup() {
  Serial.begin(115200);
  pinMode(btPin,INPUT); //set btpin as INPUT
  pinMode(xPin,INPUT);
  pinMode(yPin,INPUT);
  digitalWrite(btPin, HIGH); //and HIGH
  Serial.begin(9600); //initialize serial
  // Reset the radio
  resetRadio();
  // Initialize the radio
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.promiscuous(true);
  #ifdef IS_RFM69HW_HCW
    radio.setHighPower(); //must include this only for RFM69HW/HCW!
  #endif
//  radio.encrypt(ENCRYPTKEY);

//joystick setup
  pinMode(btPin,INPUT); //set btpin as INPUT
  digitalWrite(btPin, HIGH); //and HIGH
}


// Main loop
unsigned long previousMillis = 0;
const long sendInterval = 3000;
void loop() {

    // Receive
    if (radio.receiveDone()) {
      if (Serial) Serial.println("Message received");
      if (radio.ACKRequested()) { radio.sendACK(radio.SENDERID); }
      delay(100);
    }

    
    // Send     
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= sendInterval) {
      previousMillis = currentMillis;
  
      if (Serial) Serial.println("Sending");
      char payload[] = "hello from test node";
//      uint8_t sendbuff[1];
//      int len = 62;
//      byte sendbuff[len];
//      for(byte i =0; i<len; i++){
//        sendbuff[i]=i;
//          }
      struct joystick tosend = joystick();
      uint8_t *ptr = &(tosend.btnval);  
      Serial.println("Before loop");  
      for(int i=0; i<5; i++){
        Serial.println(ptr[i],HEX);
      }
      Serial.println("Post loop");
      if (radio.sendWithRetry(2, ptr, sizeof(struct joystick), 3, 200)) {
        if (Serial) Serial.println("ACK received");
      } else {
        if (Serial) Serial.println("No ACK");
      }
    }
}

// Reset the Radio
void resetRadio() {
  if (Serial) Serial.print("Resetting radio...");
  pinMode(RF69_RESET, OUTPUT);
  digitalWrite(RF69_RESET, HIGH);
  delay(20);
  digitalWrite(RF69_RESET, LOW);
  delay(500);
}
struct joystick joystick() {
  uint16_t xval = uint16_t(analogRead(xPin));
//  Serial.print("xval: ");
//  Serial.println(xval);
  uint16_t yval = uint16_t(analogRead(yPin));
//  Serial.print("yval: ");
//  Serial.println(yval);
  uint8_t btnval = digitalRead(btPin);
//  Serial.print("btn: ");
//  Serial.println(btnval);
  uint8_t msg[] = {btnval, xval>>8, xval, yval>>8, yval};
  struct joystick values;
  values.btnval=btnval;
  values.xmostsig = xval>>8;
  values.xleastsig = xval;
  values.ymostsig = yval>>8;
  values.yleastsig = yval;
  return values;
}
