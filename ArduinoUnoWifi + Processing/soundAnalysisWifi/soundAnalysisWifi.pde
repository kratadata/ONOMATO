import ddf.minim.analysis.*;
import ddf.minim.*;
import mqtt.*;


MQTTClient client;
Minim minim;
AudioSource in;
AudioPlayer player;
FFT fft;

float wAverage = 0;
boolean fading = false;

void setup() {
  size(512, 300);
  textSize(16);

  client = new MQTTClient(this);
  client.connect("mqtt://311b2d0d:a1617db7c1ade904@broker.shiftr.io", "processing");
  frameRate(20);

  minim = new Minim(this);
  player = minim.loadFile("beat.mp3");
  player.loop();
  in = minim.getLineIn();
  fft = new FFT(in.bufferSize(), in.sampleRate() );
}

void draw() {

  stroke(255);
  background(0);
  float currentVolume = player.getGain();
  for (int i = 0; i < in.bufferSize() - 1; i++)
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
  text("Level " + sendToArduino, 20, 20);
  client.publish("/soundVolume", ""+sendToArduino);

  if (sendToArduino > 5) {
    if (!fading) {
      player.play();
      player.shiftGain(currentVolume, 20, 1000); 
      fading = true;
    }
  } else {
    player.shiftGain(currentVolume, -40, 1000);
    fading = false;
  }
}

void stop()
{
  in.close();
  minim.stop();
  super.stop();
}

void clientConnected() {
  println("client connected");
  // client.subscribe("/hello");
}

void messageReceived(String topic, byte[] payload) {
  println("new message: " + topic + " - " + new String(payload));
}

void connectionLost() {
  println("connection lost");
}
