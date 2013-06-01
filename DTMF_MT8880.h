/*
	DTMF_MT8880.h - Library for MT8880 DTMF transciever chip.
	Created by Nick Wong, April 24, 2013
	Windchimes
*/

#ifndef DTMF_MT8880_h
#define DTMF_MT8880_h

#include "Arduino.h"

class DTMF_MT8880
{
	public:
		DTMF_MT8880(byte d0_pin, byte d1_pin, byte d2_pin, byte d3_pin, 
			byte irq_not_pin, byte rs0_pin, byte rw_pin, byte cs_not_pin);
		void reset (void);
		void playSequence (byte* values, int len, int pause);
		static const byte ONE;
		static const byte TWO;
		static const byte THREE; 
		static const byte FOUR;
		static const byte FIVE;
		static const byte SIX;
		static const byte SEVEN;
		static const byte EIGHT;
		static const byte NINE;
		static const byte ZERO;
		static const byte STAR;
		static const byte HASH;
		static const byte A;   
		static const byte B;
		static const byte C;
		static const byte D;

	private:
		byte _d0_pin;
		byte _d1_pin; 
		byte _d2_pin; 
		byte _d3_pin;
		byte _irq_not_pin; 
		byte _rs0_pin; 
		byte _rw_pin; 
		byte _cs_not_pin;
		void busMode (byte mode);
		byte readBus (void);
		void writeToBus (byte value);
		void writeToControlRegister(byte value);
		void writeToTransmitRegister(byte value);
		byte readReceiveRegister(void);
		byte readStatusRegister (void);
};

#endif