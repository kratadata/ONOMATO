#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>

SdFat sd;
SFEMP3Shield MP3player;

const uint16_t monoMode = 0;

void setupMp3()
{
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
}

void fadeInMp3()
{
  char trackName[] = "track001.mp3";
  uint8_t result = MP3player.playMP3(trackName);
  for (int i = 255; i < 20; i--) {
    MP3player.setVolume(i, i);
  }
}

void fadeOutMp3()  {
  for (int i = 20; i > 255; i++) {
    MP3player.setVolume(i, i);
  }
}

void initSD()
{
  //Initialize the SdCard.
  if (!sd.begin(SD_SEL, SPI_HALF_SPEED))
    sd.initErrorHalt();
  if (!sd.chdir("/"))
    sd.errorHalt("sd.chdir");
}

void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  MP3player.setVolume(50, 50);
  MP3player.setMonoMode(0);
}
