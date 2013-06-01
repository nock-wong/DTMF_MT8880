#include <DTMF_MT8880.h>
#include <Anemometer.h>

#define DTMF_D0_PIN 3
#define DTMF_D1_PIN 4
#define DTMF_D2_PIN 5
#define DTMF_D3_PIN 6
#define DTMF_IRQ_NOT_PIN 7
#define DTMF_RS0_PIN 8
#define DTMF_R_W_PIN 9
#define DTMF_CS_NOT_PIN 10

//#define ANEM_PIN 3
//#define ANEM_INT 1

DTMF_MT8880 dtmf(DTMF_D0_PIN, DTMF_D1_PIN, DTMF_D2_PIN, DTMF_D3_PIN, 
DTMF_IRQ_NOT_PIN, DTMF_RS0_PIN, DTMF_R_W_PIN, DTMF_CS_NOT_PIN);

//Anemometer anemometer(ANEM_PIN, ANEM_INT);

byte tone_test[] = {dtmf.ONE,dtmf.TWO,dtmf.THREE,dtmf.FOUR,dtmf.FIVE,
                    dtmf.SIX,dtmf.SEVEN,dtmf.EIGHT,dtmf.NINE,dtmf.ZERO,
                    dtmf.STAR,dtmf.HASH,dtmf.A,dtmf.B,dtmf.C,dtmf.D};
const int test_delay = 175;
int test_length = sizeof(tone_test);

void setup()
{
  dtmf.reset();
  //  attachInterrupt(ANEM_INT, anemometer.ticker, RISING);
}

void loop()
{
  dtmf.playSequence(tone_test, test_length, test_delay);
  delay(5000);
}

