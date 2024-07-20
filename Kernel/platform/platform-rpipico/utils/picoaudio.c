#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/audio.h>

int main(int argc, char **argv) {
  int fd = open("/dev/audio", O_RDWR, 0);
  if (fd == -1) {
    perror("Failed to open /dev/audio");
    exit(1);
  }

  // load the song (replace NULL with the song bytes)
  if (ioctl(fd, 0x0205, NULL) != 0) {
    perror("Failed to perform ioctl operation");
    close(fd);
    exit(1);
  }

  // play the song
  struct sound sound;

  if (ioctl(fd, AUDIOC_PLAY, &sound) != 0) {
    perror("Failed to perform play operation 2");
    close(fd);
    exit(1);
  }

  return 0;
}