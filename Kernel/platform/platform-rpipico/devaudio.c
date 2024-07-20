#include <kernel.h>
#include <audio.h>
#include <printf.h>
#include "./audio/buzzer.h"
#include "./audio/mario_tune.h"
#include "picosdk.h"

/* Simple two channel beeper style ton generator with no volume control */
const struct audio audio_info = {AUDIO_BEEPER, 2, 0};


#define PMW_PIN 18
#define HALT_BUTTON_PIN 20

void play(ezBuzzer* buz, const int* melody, const int* noteDurations, int length) {
  playMelody(buz, melody, noteDurations, length);
  kprintf("after playMelody 7\n");

  while (true) {
    loop(buz);

    if (gpio_get(HALT_BUTTON_PIN) == 0) {
      stop(buz);
      break;
    }
    if (getState(buz) == BUZZER_IDLE) {
      break;
    }
  }
}



// Interfaces that needs to be implemented

int devaudio_play(void) {
  kprintf("playing\n");

  const uint LED_PIN = 6;
  gpio_set_function(18, GPIO_FUNC_PWM);
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_init(HALT_BUTTON_PIN);
  gpio_set_dir(HALT_BUTTON_PIN, GPIO_IN);
  gpio_put(LED_PIN, 1);

  ezBuzzer buz = {0};
  buz._buzzerPin = GPIO_FUNC_PWM;
  buz._buzzerState = BUZZER_IDLE;
  buz._delayTime = 0;
  buz._beepTime = 0;
  buz._startTime = 0;
  buz._melody = 0;
  buz._noteDurations = 0;
  buz._melodyLength = 0;
  buz._melodyIndex = 0;
  buz._notePauseTime = 0;

  
  int length = sizeof(smbIntroNotes) / sizeof(int);
  play(&buz, smbIntroNotes, smbIntroDurations,length);

  length = sizeof(smbANotes) / sizeof(int);
  play(&buz, smbANotes, smbADurations, length);
  
  length = sizeof(smbBNotes) / sizeof(int);
  play(&buz, smbBNotes, smbBDurations, length);
  
  // length = sizeof(smbCNotes) / sizeof(int);
  // play(&buz, smbCNotes, smbCDurations, length);
  
  // length = sizeof(smbDNotes) / sizeof(int);
  // play(&buz, smbDNotes, smbDDurations, length);

  gpio_put(LED_PIN, 0);
  return 0;
}

void devaudio_stop(uint8_t channel) {
  //kprintf("stopped\n");
}

int devaudio_wait(uint8_t channel) {
  //kprintf("waiting\n");
  return 0;
}


int devaudio_ioctl(uarg_t op, void *val) {
  //kprintf("ioctl\n");
  // used(op);
  // used(val);
  // return -EINVAL;

  return 0;
}

void devaudio_tick(void) {

}