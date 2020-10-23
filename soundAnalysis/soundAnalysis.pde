import ddf.minim.analysis.*;
import ddf.minim.*;
import processing.serial.*;
import processing.net.*;

//int webServer = 5555; 
//Client arduinoClient;

Minim minim;
AudioSource in;
AudioPlayer player;
FFT fft;
Serial port;
float wAverage = 0;
boolean fading = false;

void setup() {
  size(512, 300);
  textSize(16);
  
  port = new Serial(this,"/dev/cu.usbmodem14202", 115200);
  
  minim = new Minim(this);
  player = minim.loadFile("groove.mp3");
  player.loop();
  in = minim.getLineIn();
  fft = new FFT(in.bufferSize(), in.sampleRate() );
}
  
void draw() {

  stroke(255);
  background(0);
  float currentVolume = player.getGain();
  //for (int j = 0; j < in.bufferSize(); j++) {
  //float rightVal = in.right.get(j);
  //float leftVal = in.left.get(j);
  //float avgVal = (leftVal + rightVal)/2;
  //float dB = in.getGain();
  //  pushMatrix();
  //  translate(width/2, height/2);
  //  rotate(radians(map(j, 0, fft.specSize()/8, 0, 360)));
  //  strokeWeight(2);
  //  stroke(255, 255, 255);
  //  point(0, 200 + avgVal*400);
  //  popMatrix();
    
  //}
 for(int i = 0; i < in.bufferSize() - 1; i++)
  {
    float x1 = map( i, 0, in.bufferSize(), 0, width );
    float x2 = map( i+1, 0, in.bufferSize(), 0, width );
    line( x1, 50 + in.left.get(i)*50, x2, 50 + in.left.get(i+1)*50 );
    line( x1, 150 + in.right.get(i)*50, x2, 150 + in.right.get(i+1)*50 );
  }
  
  noStroke();
  fill( 255, 255 );
  float avgVal = (in.left.level() + in.right.level())/2;
  avgVal = avgVal*100;
  
  wAverage = wAverage * 0.8;
  wAverage += avgVal * 0.2;
 
  rect(0, 50, wAverage*10, 100 );
  fill(0);  
  noStroke();
  rect(0, 0, width, 50);
  fill(255); 
  
   int sendToArduino = int(round(wAverage));
    text("Level " + sendToArduino, 20,20);
    port.write("" + sendToArduino);
    port.write("\n");
    
  /*if(arduinoClient.available() > 0){
    text("Level " + sendToArduino, 20,20);
    port.write("" + sendToArduino);
    port.write("\n");
  }*/
  
  if (sendToArduino > 10) {
    if (!fading) {
      player.rewind();
      player.play();
      player.shiftGain(currentVolume,13,1000); 
      fading = true;
    }
  } else {
    if (fading) {
       player.shiftGain(currentVolume,-80,1000);
       fading = false;
     }
  }
}
void stop()
{
  in.close();
  minim.stop();
  super.stop();
}
