
void shiftToSec(byte sec[], byte clk, byte data, byte lt) { //shift goes by last in first out (by out i mean last shifted byte will be visible on the first 595 )
  digitalWrite(lt, LOW);
  for (int i = 4; i >= 0; i--) {
    shiftOut(data, clk, MSBFIRST, sec[i]);
    //delay(10); //----
  }
  digitalWrite(lt, HIGH);
}

void broadcast_value(byte value) { //sends identical data to all of the segments
  //byte printArray[5] = {0};
  for (int i = 0; i < 5; i++) {
    _printArray[i] = value;
  }
  shiftToSec(_printArray, CELL_CLK, CELL_DATA, CELL_LT);
}

void suppress_dots() {
  for (int i = 0; i < 5; i++) {
    _printArray[i] &= B01111111;
    _printArray[i] += 128;
  }
}

void show_dot(byte index) { // will show a dot by a given index
  _printArray[index] &= B01111111; //0 will light up the dot since the 7seg displays are common cathode
}

void suppress_digits(byte index) { //will hide digits but will not interfere with dots and leds that are on
  _printArray[index] &= B11110000;
  _printArray[index] += 10; //value 10 will display nothing on the displlays
}

void p_show_score(byte ct_score, byte t_score) {//The score arragement XX_YY, where XX is ct score, _ is blank segment, YY is t score
  _printArray[2] = 138; // show blank in the middle //also suppress digits method can be used
  process_int(ct_score, 2, 3, true);
  process_int(t_score, 2, 0, true);
  suppress_dots();
  shiftToSec(_printArray, CELL_CLK, CELL_DATA, CELL_LT);
}

int num_of_digits(unsigned int n) { //pretty stupid method but fastest that works with no external libs
  if (n < 10)
    return 1;
  else if (n < 100)
    return 2;
  else if (n < 1000)
    return 3;
  else if (n < 10000)
    return 4;
  else
    return 5;
}

void process_int(int n, byte reserved_slots, byte shift_to_left, boolean show_leading_zeroes) {
  int n_digits = num_of_digits(n);
  int shifted_index = 4 - shift_to_left;

  for (int j = shifted_index ; j > shifted_index - reserved_slots; j-- ) { //digits are calculated in LSD order therefore inverted loop must be used
    _printArray[j] &= B11110000; //this part will prevent variable overflow
    _printArray[j] += n % 10; // pass the values to the global array
    n /= 10;
  } //for

  if (n_digits < reserved_slots) {
    for (byte i = shifted_index + 1 - reserved_slots; i < shifted_index + 1 - n_digits; i++) {
      if (show_leading_zeroes) {
        bitWrite(_printArray[i], 0, 0); //fill the reserved slots space with leading zeroes
      }
      else {
        suppress_digits(i);
      }
    }
  }
}

void print_number(int n, byte reserved_slots, byte shift_to_left, boolean show_leading_zeroes) {
  process_int(n, reserved_slots, shift_to_left, show_leading_zeroes);
#if DEBUG
  printSector("Array as digits: ", 0, _printArray);
#endif
  shiftToSec(_printArray, CELL_CLK, CELL_DATA, CELL_LT);
}


void print_time(int seconds) {
  suppress_digits(4);
  suppress_digits(0);
  process_int(seconds / 60, 1, 3, false); //render minutes
  process_int(seconds % 60, 2, 1, true); //render seconds
  show_dot(1);
  shiftToSec(_printArray, CELL_CLK, CELL_DATA, CELL_LT);
}

void print_real_time(byte hh, byte mm, boolean show_the_dot) {
  suppress_dots();
  suppress_digits(4); // dont show the 5th digit
  process_int(hh, 2, 3, true); //render hours
  process_int(mm, 2, 1, true); //render minutes
  if (show_the_dot){ //else the dot wont be displayed
     show_dot(1);   
  }
  shiftToSec(_printArray, CELL_CLK, CELL_DATA, CELL_LT);
}






