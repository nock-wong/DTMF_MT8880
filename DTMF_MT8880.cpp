/*
	DTMF_MT8880.h - Library for MT8880 DTMF transciever chip.
	Created by Nick Wong, April 24, 2013
	Windchimes
*/

#include "Arduino.h"
#include "DTMF_MT8880.h"


// DTMF character to value mapping
const byte DTMF_MT8880::ONE = 1;
const byte DTMF_MT8880::TWO = 2;
const byte DTMF_MT8880::THREE = 3; 
const byte DTMF_MT8880::FOUR = 4;
const byte DTMF_MT8880::FIVE = 5;
const byte DTMF_MT8880::SIX = 6;
const byte DTMF_MT8880::SEVEN = 7;
const byte DTMF_MT8880::EIGHT = 8;
const byte DTMF_MT8880::NINE = 9;
const byte DTMF_MT8880::ZERO = 10;
const byte DTMF_MT8880::STAR = 11;
const byte DTMF_MT8880::HASH = 12;
const byte DTMF_MT8880::A = 13;   
const byte DTMF_MT8880::B = 14;
const byte DTMF_MT8880::C = 15;
const byte DTMF_MT8880::D = 0;


// Mode constants
const byte _MODE_WRITE = 0;
const byte _MODE_READ = 1;

DTMF_MT8880::DTMF_MT8880(byte d0_pin, byte d1_pin, byte d2_pin, byte d3_pin, byte irq_not_pin, byte rs0_pin, byte rw_pin, byte cs_not_pin)
{
	_d0_pin = d0_pin;
  _d1_pin = d1_pin; 
  _d2_pin = d2_pin; 
  _d3_pin = d3_pin;
  _irq_not_pin = irq_not_pin; 
  _rs0_pin = rs0_pin; 
  _rw_pin = rw_pin; 
  _cs_not_pin = cs_not_pin;
  busMode(_MODE_WRITE);
	pinMode(_irq_not_pin, INPUT);
  pinMode(_rs0_pin, OUTPUT);
  pinMode(_cs_not_pin, OUTPUT);
  pinMode(_rw_pin, OUTPUT);
  return;
}

void DTMF_MT8880::reset(void)
{
  readStatusRegister();
  writeToControlRegister(B0000);
  writeToControlRegister(B0000);
  writeToControlRegister(B1000);
  writeToControlRegister(B0000);
  readStatusRegister();
  return;
}

void DTMF_MT8880::playSequence(byte* values, int len, int pause) {
  writeToControlRegister(B1101);
  writeToControlRegister(B0000);
  for (int i = 0; i < len; i++) {
    writeToTransmitRegister(values[i]);
    delay(pause);  
  }
}

void DTMF_MT8880::writeToBus(byte value) {
  busMode(_MODE_WRITE);
  digitalWrite(_d0_pin, bitRead(value,0));
  digitalWrite(_d1_pin, bitRead(value,1));
  digitalWrite(_d2_pin, bitRead(value,2));
  digitalWrite(_d3_pin, bitRead(value,3));
  return;
}

void DTMF_MT8880::busMode(byte mode)
{
  if (mode == _MODE_WRITE) {
    pinMode(_d0_pin, OUTPUT);
    pinMode(_d1_pin, OUTPUT);
    pinMode(_d2_pin, OUTPUT);
    pinMode(_d3_pin, OUTPUT);  
  }
  else if (mode == _MODE_READ) {
    pinMode(_d0_pin, INPUT);
    pinMode(_d1_pin, INPUT);
    pinMode(_d2_pin, INPUT);
    pinMode(_d3_pin, INPUT);  
  }
  return;
}

byte DTMF_MT8880::readBus(void) 
{
  byte value = 0;
  busMode(_MODE_READ);
  bitWrite(value, 0, digitalRead(_d0_pin));
  bitWrite(value, 1, digitalRead(_d1_pin));
  bitWrite(value, 2, digitalRead(_d2_pin));
  bitWrite(value, 3, digitalRead(_d3_pin));
  return value;
}

void DTMF_MT8880::writeToControlRegister(byte value) {
  writeToBus(value);
  digitalWrite(_cs_not_pin, LOW); 
  digitalWrite(_rs0_pin, HIGH);
  digitalWrite(_rw_pin, LOW);
  digitalWrite(_cs_not_pin, HIGH);
  return;
}

void DTMF_MT8880::writeToTransmitRegister(byte value) {
  writeToBus(value);
  digitalWrite(_rs0_pin, LOW);
  digitalWrite(_rw_pin, LOW);
  digitalWrite(_cs_not_pin, LOW);  
  digitalWrite(_cs_not_pin, HIGH);  
  return;
}

byte DTMF_MT8880::readReceiveRegister(void) {
  busMode(_MODE_READ);
  digitalWrite(_rw_pin, HIGH);
  digitalWrite(_rs0_pin, LOW);
  digitalWrite(_cs_not_pin, LOW);
  byte value;
  value = readBus();
  return value;
}

byte DTMF_MT8880::readStatusRegister(void) 
{
  busMode(_MODE_READ);
  digitalWrite(_cs_not_pin, LOW);
  digitalWrite(_rs0_pin, HIGH);
  digitalWrite(_rw_pin, HIGH);
  byte value = readBus();
  digitalWrite(_cs_not_pin, HIGH);
  return value;
}


