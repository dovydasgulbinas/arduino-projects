
struct segmentPins { // LATCH PIN CAN BE REUSED since silmutanious shift do no occur
  const byte clk[5];
  const byte data[5];
  const byte lt[5];
} sp =
{ {13, A0, 10, 7, A2},
  {12, A1, 9, A0, A3},
  {11, A2, A3, A1, 6}
};

void init_segment_pins() {
  for (int i = 0; i < 5; i++) { //enable ouput pins make i <5 later
    pinMode(sp.clk[i], OUTPUT);
    pinMode(sp.data[i], OUTPUT);
    pinMode(sp.lt[i], OUTPUT);

  }
}

