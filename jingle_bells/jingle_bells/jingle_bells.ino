#include <Arduino.h>

int bounce_time = 100;
int button_pin = 11;
bool is_playing = false;
bool last_state = false;
bool new_state = false;

#define buttonPin_0 (11)
#define buttonLine (12)

struct buttonStruct{
	uint8_t cState : 1;
	uint8_t pState : 1;
	uint8_t buttPin : 4;
} button[1];

int speakerPin = 13;
int length = 26;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int beats[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};

int tempo = 300;
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin_0, OUTPUT)
  	pinMode(buttonLine, INPUT)
digitalWrite(buttonPin_0, HIGH);
button[0] = {0,0,buttonPin_0};

  is_playing = false;


}
void loop() {
  new_state = digitalWrite(12, HIGH);
  digitalWrite(12, LOW);

  if((millis()%bounce_time == 0)&&(new_state)){
    if(is_playing==true){
      is_playing = false;
    }
    else{
      is_playing = true;
    }


  }

  if(is_playing == true){
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2);
  }
}


} // loop
