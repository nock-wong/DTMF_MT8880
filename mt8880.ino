#include <dht.h>

dht DHT;

const byte WRITE = 0;
const byte READ = 1;

// DTMF character to value mapping
const byte ONE = 1;
const byte TWO = 2;
const byte THREE = 3; 
const byte FOUR = 4;
const byte FIVE = 5;
const byte SIX = 6;
const byte SEVEN = 7;
const byte EIGHT = 8;
const byte NINE = 9;
const byte ZERO = 10;
const byte STAR = 11;
const byte HASH = 12;
const byte A = 13;   
const byte B = 14;
const byte C = 15;
const byte D = 0;

// Pin definitions
const byte D_0 = 3;
const byte D_1 = 4;
const byte D_2 = 5;
const byte D_3 = 6;
const byte IRQ_NOT = 7;
const byte RS_0 = 8;
const byte R_W = 9;
const byte CS_NOT = 10;
const byte DHT_IN = 11;
const byte SW = 12;

void reset(void) {
  delay(100);
  statusRegisterRead();
  controlRegisterWrite(B0000);
  controlRegisterWrite(B0000);
  controlRegisterWrite(B1000);
  controlRegisterWrite(B0000);
  statusRegisterRead();
}

byte statusRegisterRead(void) {
  busMode(READ);
  digitalWrite(R_W, HIGH);
  digitalWrite(RS_0, HIGH);
  digitalWrite(CS_NOT, LOW);
  byte value = busRead();
  digitalWrite(CS_NOT, HIGH);
  return value;
}
void controlRegisterWrite(byte value) {
  busWrite(value);
  digitalWrite(RS_0, HIGH);
  digitalWrite(R_W, LOW);
  digitalWrite(CS_NOT, LOW); 
  digitalWrite(CS_NOT, HIGH);
  return;
}
void transmitRegisterWrite(byte value) {
  busWrite(value);
  digitalWrite(RS_0, LOW);
  digitalWrite(R_W, LOW);
  digitalWrite(CS_NOT, LOW);  
  digitalWrite(CS_NOT, HIGH);  
  return;
}
void busMode(byte mode) {
  if (mode == WRITE) {
    pinMode(D_0, OUTPUT);
    pinMode(D_1, OUTPUT);
    pinMode(D_2, OUTPUT);
    pinMode(D_3, OUTPUT);  
  }
  else if (mode == READ) {
    pinMode(D_0, INPUT);
    pinMode(D_1, INPUT);
    pinMode(D_2, INPUT);
    pinMode(D_3, INPUT);  
  }
  return;
}
void busWrite(byte value) {
  busMode(WRITE);
  digitalWrite(D_0, bitRead(value,0));
  digitalWrite(D_1, bitRead(value,1));
  digitalWrite(D_2, bitRead(value,2));
  digitalWrite(D_3, bitRead(value,3));
  return;
}
byte busRead(void) {
  busMode(READ);
  byte value = 0;
  bitWrite(value, 0, digitalRead(D_0));
  bitWrite(value, 1, digitalRead(D_1));
  bitWrite(value, 2, digitalRead(D_2));
  bitWrite(value, 3, digitalRead(D_3));
  return value;
}

void setup() {
  busMode(WRITE);
  pinMode(SW, INPUT);
  pinMode(IRQ_NOT, INPUT);
  pinMode(RS_0, OUTPUT);
  pinMode(CS_NOT, OUTPUT);
  pinMode(R_W, OUTPUT);
  reset();
  Serial.begin(115200);
}

void loop() {
  int DHT_chk;
  byte seq[2];
  if (digitalRead(SW) == HIGH) {
    Serial.println("Trigger:");
    DHT_chk = readDHT();
    
    Serial.print("Temperature: ");
    Serial.println(int(DHT.temperature), 1);
    intToTones(int(DHT.temperature), seq);
    playSeq(seq, 2);
    
//    Serial.print("Humidity: ");
//    Serial.println(int(DHT.humidity), 1); 
//    intToTones(int(DHT.humidity), seq);
//    playSeq(seq, 2);
    
    byte a[] = {HASH};
    playSeq(a, 1);
  }
}

int readDHT (void) {
  int chk = DHT.read22(DHT_IN);
  Serial.print("Read sensor status: ");
  switch (chk) {
    case DHTLIB_OK:
                Serial.println("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error,\t"); 
                break;
    default: 
                Serial.println("Unknown error,\t"); 
                break;   
  }
  return chk;
}

void intToTones(int value, byte* seq) {
  int ones = ZERO;
  int tens = ZERO;
  if (value != 0) {
    ones = value % 10;
    tens = (value/10) % 10;  
  }
  seq[1] = ones;
  seq[0] = tens;
}



void playSeq(byte* values, int len) {
  //byte values[] = {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO, STAR, HASH, A, B, C, D};
  //byte values[] = {ONE,EIGHT};
  controlRegisterWrite(B1101);
  controlRegisterWrite(B0000);
  for (int i = 0; i < len; i++) {
    Serial.println(values[i]);
    transmitRegisterWrite(values[i]);
    delay(1000);  
    while (digitalRead(IRQ_NOT) == HIGH) {
      delay(1); 
    }
  }
}

