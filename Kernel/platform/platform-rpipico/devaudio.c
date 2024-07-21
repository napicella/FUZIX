#include <kernel.h>
#include <audio.h>
#include <printf.h>
#include "picosdk.h"

/* Simple two channel beeper style ton generator with no volume control */
const struct audio audio_info = {AUDIO_BEEPER, 2, 0};


#define PMW_PIN 18
#define HALT_BUTTON_PIN 20

static int count = 0;
static bool playing = false;

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
  kprintf("tone pin %d and freq %d\n", pin, frequency);
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

// Interfaces that needs to be implemented

int devaudio_play(void) {
  tone(18, sound.freq, 50);
  return 0;
}

void devaudio_stop(uint8_t channel) {
  noTone(18);
}

int devaudio_wait(uint8_t channel) {
  used(op);
  used(val);
  return -EINVAL;
}


int devaudio_ioctl(uarg_t op, void *val) {
  gpio_set_function(18, GPIO_FUNC_PWM);
  return 0;
}

void devaudio_tick(void) {
  // count++;
  // if (sound.time > count) {
  //   noTone(18);
  //   count = 0;
  // }
}