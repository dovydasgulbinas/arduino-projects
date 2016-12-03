
void shiftToSec(byte sec[], byte clk, byte data, byte lt) { //shift goes by last in first out (by out i mean last shifted byte will be visible on the first 595 )

  digitalWrite(lt, LOW);
  for (int i = 4; i >= 0; i--) {
    shiftOut(data, clk, MSBFIRST, sec[i]);
    //delay(10); //----
  }
  digitalWrite(lt, HIGH);
}

void broadcast_value(byte value) { //sends identical data to all of the segments
  byte printArray[5] = {0};
  for (int i = 0; i < 5; i++) {
    printArray[5] = value;
  }
  shiftToSec(printArray, CELL_CLK, CELL_DATA, CELL_LT);
}

void suppress_dots() {
  for (int i = 0; i < 5; i++) {
    _printArray[i] &= B01111111;
    _printArray[i] += 128;
  }
}

void suppress_digits(byte index) { //will hide digits but will not interfere with dots and leds that are on
  _printArray[index] &= B11110000;
  _printArray[index] += 10; //value 10 will display nothing on the displlays
}

void p_show_score(byte ct_score, byte t_score) {
  _printArray[0] = 128;
  _printArray[1] = ct_score;
  _printArray[2] = 138;
  _printArray[3] = 128;
  _printArray[4] = t_score;
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

void printNumber(int n, byte reserved_slots, byte shift_to_left, boolean show_leading_zeroes) {
  int n_digits = num_of_digits(n);
  int shifted_index = 4 - shift_to_left;

  for (int j = shifted_index ; j > shifted_index - reserved_slots; j-- ) { //digits are calculated in LSD order therefore inverted loop must be used
    _printArray[j] &= B11110000; //this part will prevent variable overflow
    _printArray[j] += n % 10; // pass the values to the global array
    n /= 10;
  } //for

  if (n_digits < reserved_slots) {
    Serial.println("TRUTH!");
    for (byte i = shifted_index + 1 - reserved_slots; i < shifted_index + 1 - n_digits; i++) {
      if (show_leading_zeroes) {
        bitWrite(_printArray[i], 0, 0); //fill the reserved slots space with leading zeroes
      }
      else {
        suppress_digits(i);
      }
    }
  }
  printSector("Array as digits: ", 0, _printArray);
  shiftToSec(_printArray, CELL_CLK, CELL_DATA, CELL_LT);
}







