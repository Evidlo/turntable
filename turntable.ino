#include "avr/interrupt.h"

#define trimpot PIN_PA7 // speed trim
#define button33 PIN_PA2 // 33 speed button
#define button45 PIN_PA3 // 45 speed button
#define button78 PIN_PA4 // 78 speed button
#define pwm PIN_PB2 // output
#define test PIN_PA6

#define speed33 118
#define speed45 160
#define speed78 251


void setup() {
    pinMode(trimpot, INPUT);
    pinMode(button33, INPUT);
    pinMode(button45, INPUT);
    pinMode(button78, INPUT);
    pinMode(pwm, OUTPUT);
    pinMode(test, OUTPUT);

    // enable pin change interrupts on button pins
    GIMSK = 1<<PCIE0;
    // FIXME - why doesn't this work?
    /* PCMSK0 = (1<<PCINT4) || (1<<PCINT3) || (1<<PCINT2); */
    PCMSK0 = 0b00011100;
    sei();

    analogWrite(pwm, speed33);
    analogWrite(test, 100);
}

int trimmed_speed(int speed) {
    // adjust speed by plus/minus 10 percent from trimpot
    int32_t trimpot_value = analogRead(trimpot);
    trimpot_value += analogRead(trimpot);
    trimpot_value += analogRead(trimpot);
    trimpot_value += analogRead(trimpot);
    trimpot_value += analogRead(trimpot);
    trimpot_value /= 5;
    return speed + speed * (trimpot_value - 512) / (512 * 10);
}

ISR(PCINT0_vect) {
    if (digitalRead(button33) == LOW) {
      analogWrite(pwm, trimmed_speed(speed33));
    }
    if (digitalRead(button45) == LOW) {
      analogWrite(pwm, trimmed_speed(speed45));
    }
    if (digitalRead(button78) == LOW) {
      analogWrite(pwm, trimmed_speed(speed78));
    }
}

void loop() {}
