
void shiftToSec(byte sec[], byte clk, byte data, byte lt) { //shift goes by last in first out (by out i mean last shifted byte will be visible on the first 595 )

  digitalWrite(lt, LOW);
  for (int i = 4; i >= 0; i--) {
    shiftOut(data, clk, MSBFIRST, sec[i]);
    //delay(10); //----
  }
  digitalWrite(lt, HIGH);
}

int num_of_digits(int n) { //pretty stupid method but fastest that works with no external libs
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

void printNumber(int n, boolean suppress_leading_zeroes) {
  byte printArray[5] = {10, 10, 10, 10, 10}; //Values >10 will show up as a blank segment
  int n_digits = num_of_digits(n);
  int leading_zeroes = 5 - n_digits;
  if (!suppress_leading_zeroes) { //if user asks to show leading zeros then show them
    for (int i = 0; i < leading_zeroes; i++) //there are 5 digits in total on the screen
      printArray[i] = 0;
  }// if

  for (int j = 4; j >= leading_zeroes; j-- ) { //digits are calculated in LSD order therefore inverted loop must be used
    printArray[j] = n % 10;
    n /= 10;
  } //for
  //printSector("Array as digits: ", 0, printArray);
  //Serial.println(n_digits);
  shiftToSec(printArray, CELL_CLK, CELL_DATA, CELL_LT);
}




