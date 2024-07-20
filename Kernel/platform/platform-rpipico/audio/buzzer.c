#include <kernel.h>
#include "buzzer.h"
#include <printf.h>
// #include <stdlib.h>

// #include "hardware/pwm.h"
#include "../picosdk.h"


ezBuzzer* newEzBuzzer(int pin) {
  ezBuzzer* buz = (ezBuzzer*)malloc(sizeof(ezBuzzer));
  buz->_buzzerPin = pin;
  buz->_buzzerState = BUZZER_IDLE;
  buz->_delayTime = 0;
  buz->_beepTime = 0;
  buz->_startTime = 0;
  buz->_melody = 0;
  buz->_noteDurations = 0;
  buz->_melodyLength = 0;
  buz->_melodyIndex = 0;
  buz->_notePauseTime = 0;

  gpio_set_function(buz->_buzzerPin, GPIO_FUNC_PWM);
  return buz;
}

// d - the duty cycle (0 -> 100). For a square wave, select 50.
uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d) {
  // Pi pico has a 12-bit fractional divider, where 12 bits can represent values
  // from 0 to 4095 (2^12 = 4096). This gives a high resolution for dividing the
  // base clock.
  //
  // The PWM signal period is determined by how many clock cycles fit into one
  // period of the PWM signal. Without a divider, the period would be very short
  // if the base clock frequency is high (e.g., 125 MHz). This short period
  // limits the number of discrete steps that can be used within one PWM cycle.
  uint32_t clock = 125000000;
  uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
  if (divider16 / 16 == 0) {
    divider16 = 16;
  }

  uint32_t wrap = clock * 16 / divider16 / f - 1;

  // divider16 / 16  - integer part
  // divider16 & 0xF - fractional part (last 4 bits)
  pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
  pwm_set_wrap(slice_num, wrap);
  pwm_set_chan_level(slice_num, chan, wrap * d / 100);

  return wrap;
}

// Function to set the PWM frequency
void tone(uint pin, uint32_t frequency, int duration) {
  kprintf("tone\n");
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_set_freq_duty(slice_num, pwm_gpio_to_channel(pin), frequency, duration);
  pwm_set_enabled(slice_num, true);
}

// Function to disable the PWM
void noTone(uint pin) {
  kprintf("no-tone\n");
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_set_enabled(slice_num, false);
}

int millis() { return to_ms_since_boot(get_absolute_time()); }

void stop(ezBuzzer* buzzer) {
  noTone(buzzer->_buzzerPin);
  buzzer->_buzzerState = BUZZER_IDLE;
}

void beep(ezBuzzer* buzzer, unsigned long beepTime, unsigned long delay) {
  buzzer->_delayTime = delay;
  buzzer->_beepTime = beepTime;
  buzzer->_buzzerState = BUZZER_BEEP_DELAY;
  buzzer->_startTime = millis();
}

void playMelody(ezBuzzer* buzzer, const int* melody, const int* noteDurations, int length) {
  buzzer->_buzzerPin = 18;
  buzzer->_notePauseTime = 0;
  buzzer->_melody = melody;
  buzzer->_noteDurations = noteDurations;
  buzzer->_melodyLength = length;
  buzzer->_melodyIndex = 0;
  buzzer->_notePauseTime = 0;
  buzzer->_buzzerState = BUZZER_MELODY;
  buzzer->_startTime = millis();
}

int getState(ezBuzzer* buzzer) { 
  return buzzer->_buzzerState;
 };

void loop(ezBuzzer* buzzer) {
  switch (buzzer->_buzzerState) {
    case BUZZER_IDLE:
      break;

    case BUZZER_MELODY:
      if (buzzer->_melodyIndex < buzzer->_melodyLength) {
        if (!buzzer->_notePauseTime) {
          kprintf("melody\n");
          kprintf("%d\n", buzzer->_noteDurations[buzzer->_melodyIndex]);
          kprintf("%d\n", buzzer->_melody[buzzer->_melodyIndex]);
          // if _notePauseTime is not set calculate it.
          // To calculate the note duration, take one second divided by the note
          // type.
          // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          // 1000 defines the tempo - smaller and it gets faster
          int duration = 1000 / buzzer->_noteDurations[buzzer->_melodyIndex];

          // TODO: I added note 0 if
          if (buzzer->_melody[buzzer->_melodyIndex] != NOTE_0) {
            // Duty cycle set to 50%
            kprintf("before melody tone with pin\n");
            tone(18, buzzer->_melody[buzzer->_melodyIndex],
               50);
          }
          kprintf("after tone melody\n");
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          buzzer->_notePauseTime = (duration * 130) / 100;
          kprintf("after setting duration\n");
          buzzer->_startTime = millis();
          kprintf("after setting start time\n");
        }

        if ((unsigned long)(millis() - buzzer->_startTime) >=
            buzzer->_notePauseTime) {
          
          // stop the tone playing:
          noTone(18);
          buzzer->_notePauseTime = 0;
          // play next node
          buzzer->_melodyIndex++;
        }
      } else {
        // no more notes
        noTone(buzzer->_buzzerPin);
        buzzer->_buzzerState = BUZZER_IDLE;
      }

    default:
      break;
  }
}