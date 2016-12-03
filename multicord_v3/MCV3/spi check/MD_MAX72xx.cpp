

#include <Arduino.h>

#include <SPI.h>




MD_MAX72XX::MD_MAX72XX(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices):
_dataPin(dataPin), _clkPin(clkPin), _csPin(csPin), _maxDevices(numDevices),
_updateEnabled(true)
{
	_hardwareSPI = false;
#if USE_LOCAL_FONT && USE_FONT_INDEX
	_fontIndex = NULL;
#endif
}

MD_MAX72XX::MD_MAX72XX(uint8_t csPin, uint8_t numDevices):
_dataPin(0), _clkPin(0), _csPin(csPin), _maxDevices(numDevices),
_updateEnabled(true)
{
	_hardwareSPI = true;
#if USE_LOCAL_FONT && USE_FONT_INDEX
	_fontIndex = NULL;
#endif
}

void MD_MAX72XX::begin(void)
{
  // initialize the AVR hardware
  if (_hardwareSPI)
  {
    PRINTS("\nHardware SPI");
    // Set direction register for SCK and MOSI pin.
	  // MISO pin automatically overrides to INPUT.
	  // SS pin is still used and needs to be made HIGH
	  digitalWrite(SS, HIGH);
	  pinMode(SS, OUTPUT);
	  pinMode(MOSI, OUTPUT);
	  pinMode(SCK, OUTPUT);

#if USE_LIBRARY_SPI
    PRINTS("\nLibrary SPI");
    SPI.begin();
#else
    PRINTS("\nNative SPI");
    // Warning: if the SS ever becomes a LOW INPUT then SPI
	  // automatically switches to Slave, so the data direction of
	  // the SS pin MUST be kept as OUTPUT.
	  SPCR |= _BV(MSTR);
	  SPCR |= _BV(SPE);

	  // Set SPI to MSB first
    SPCR &= ~(_BV(DORD));
#endif // USE_LIBRARY_SPI
  }
  else
  {
    pinMode(_dataPin, OUTPUT);
  	pinMode(_clkPin, OUTPUT);
  }

  // initialise our preferred CS pin (could be same as SS)
  digitalWrite(_csPin, HIGH);
  pinMode(_csPin, OUTPUT);

  // object memory and internals
  setShiftDataInCallback(NULL);
  setShiftDataOutCallback(NULL);

  _matrix = (deviceInfo_t *)malloc(sizeof(deviceInfo_t) * _maxDevices);
  _spiData = (uint8_t *)malloc(SPI_DATA_SIZE);

#if USE_LOCAL_FONT
#if USE_INDEX_FONT
  _fontIndex = (uint16_t *)malloc(sizeof(uint16_t) * FONT_INDEX_SIZE);
#endif
  setFont(NULL);
#endif // INCLUDE_LOCAL_FONT

  // clear internal memory map for this device
  for (uint8_t d = FIRST_BUFFER; d <= LAST_BUFFER; d++)
  {
    _matrix[d].changed = ALL_CLEAR;
    for (uint8_t i = 0; i < ROW_SIZE; i++)
    {
      _matrix[d].dig[i] = 0;
    }
  }
			
  // Initialize the display devices. On initial power-up
  // - all control registers are reset, 
  // - scan limit is set to one digit (row/col or LED),
  // - Decoding mode is off, 
  // - intensity is set to the minimum, 
  // - the display is blanked, and 
  // - the MAX7219/MAX7221 is shut down.
  // The devices need to be set to our library defaults prior using the 
  // display modules.
  control(TEST, OFF);				// no test
  control(SCANLIMIT, ROW_SIZE-1);	// scan limit is set to max on startup
  control(INTENSITY, MAX_INTENSITY/2);	// set intensity to a reasonable value
  control(DECODE, OFF);				// make sure that no decoding happens (warm boot potential issue)
  clear();
  control(SHUTDOWN, OFF);			// take the modules out of shutdown mode
}

MD_MAX72XX::~MD_MAX72XX(void)
{
    SPI.end();
}

void MD_MAX72XX::spiSend() 
{
  // enable the devices to receive data
  digitalWrite(_csPin, LOW);

  // shift out the data 
  if (_hardwareSPI)
  {
    for (int i = SPI_DATA_SIZE-1; i >= 0; i--)
	  {

      SPI.transfer(_spiData[i]);

    }
  }

  digitalWrite(_csPin, HIGH);
}    
