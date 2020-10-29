#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>

SdFat sd;
SFEMP3Shield MP3player;

const uint16_t monoMode = 0;
char trackName[] = "track001.mp3";

void setupMp3()
{
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
  uint8_t result = MP3player.playMP3(trackName);
}

void fadeInMp3()
{
  for (int i = 0; i < 255; i += 10) {
    uint8_t volume = 255 - i;
    MP3player.setVolume(volume, volume);
    delay(100);
  }
}

void fadeOutMp3()  {
  for (int i = 0; i < 255; i += 10) {
    uint8_t volume = i;
    MP3player.setVolume(volume, volume);
    delay(100);
  }
}

void resumeThisTrack()  {
  MP3player.resumeMusic(0);
}

void stopThisTrack()  {
  MP3player.pauseMusic();
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
  MP3player.setVolume(0, 0);
  MP3player.setMonoMode(0);
}
