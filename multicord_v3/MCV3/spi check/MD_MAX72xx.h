/**



A 4-wire serial interface (SPI) allows the devices to be cascaded, with 
communications passed through the first device in the chain to all others. Individual 
elements may be addressed and updated without rewriting the entire display.


The AVR hardware SPI interface is fast but fixed to predetermined output pins. The more general 
software interface uses the Arduino shiftOut() library function, making it slower but allows the 
use of arbitrary digital pins to send the data to the device. Which mode is enabled depends 
on the class constructor used.

The Arduino interface is implemented with 3 digital outputs that are passed through to 
the class constructor. The digital outputs define the SPI interface as follows:
- DIN (MOSI) - the Data IN signal shifts data into the display module. Data is loaded into 
the device's internal 16-bit shift register on CLK's rising edge.
- CLK (SCK) - the CLocK signal that is used to time the data for the device. 
- LD (SS) - the interface is active when LoaD signal is LOW. Serial data are loaded into the 
device shift register while LOAD is LOW and latched in on the rising edge.


*/
#ifndef MD_MAX72xx_h
#define MD_MAX72xx_h

#include <Arduino.h>



#define	USE_LIBRARY_SPI	1




/**
 * Core object for the MD_MAX72XX library
 */
class MD_MAX72XX
{
public:




	
  /** 
   * Class Constructor - arbitrary digital interface.
   *
   * Instantiate a new instance of the class. The parameters passed are used to 
   * connect the software to the hardware. Multiple instances may co-exist 
   * but they should not share the same hardware CS pin (SPI interface).
   * 
   * \param dataPin		output on the Arduino where data gets shifted out.
   * \param clkPin		output for the clock signal.
   * \param csPin		output for selecting the device.
   * \param numDevices	number of devices connected. Default is 1 if not supplied. 
   *                    Memory for device buffers is dynamically allocated based 
   *                    on this parameter.
   */
  MD_MAX72XX(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices=1);

  /** 
   * Class Constructor - SPI hardware interface.
   *
   * Instantiate a new instance of the class. The parameters passed are used to 
   * connect the software to the hardware. Multiple instances may co-exist 
   * but they should not share the same hardware CS pin (SPI interface).
   * The dataPin and the clockPin are defined by the Arduino hardware definition
   * (SPI MOSI and SCK signals).
   * 
   * \param csPin		output for selecting the device.
   * \param numDevices	number of devices connected. Default is 1 if not supplied. 
   *                    Memory for device buffers is dynamically allocated based 
   *                    on this parameter.
   */
  MD_MAX72XX(uint8_t csPin, uint8_t numDevices=1);

  /** 
   * Initialize the object.
   *
   * Initialise the object data. This needs to be called during setup() to initialise new 
   * data for the class that cannot be done during the object creation.
   *
   * The LED hardware is initialized to the middle intensity value, all rows showing, 
   * and all LEDs cleared (off). Test, shutdown and decode modes are off. Display updates 
   * are on and wraparound is off.
   */
  void begin(void);

  /** 
   * Class Destructor.
   *
   * Released allocated memory and does the necessary to clean up once the object is
   * no longer required.
   */

  //--------------------------------------------------------------
  /** \name Methods for object and hardware control.
   * @{
   */
  /** 
   * Set the control status of the specified parameter for the specified device.
   * 
   * The device has a number of control parameters that can be set through this method. 
   * The type of control action required is passed through the mode parameter and 
   * should be one of the control actions defined by controlRequest_t. The value that 
   * needs to be supplied on the control action required is one of the defined 
   * actions in controlValue_t or a numeric parameter suitable for the control action.
   *
   * \param dev			address of the device to control [0..getDeviceCount()-1].
   * \param mode		one of the defined control requests.
   * \param value		parameter value or one of the control status defined.
   * \return false if parameter errors, true otherwise.
   */
  
  
  
  void setShiftDataInCallback(uint8_t (*cb)(uint8_t dev, transformType_t t)) { _cbShiftDataIn = cb; };

  /** 
   * Set the Shift Data Out callback function.
   *
   * The callback function is called from the library when a transform shift left 
   * or shift right operation is executed and the library is about to discard the data for 
   * the first element of the shift (ie, conceptually this is the data that 'falls' off 
   * the front end of the scrolling display). The callback function is invoked when
   * - WRAPAROUND is not active, as the data would automatically supplied to the tail end.
   * - the call to transform() is global (ie, not for an individual buffer).
   *  
   * The callback function is with supplied 3 parameters, with no return value required:
   * - the device number that is the source of the data [0..getDeviceCount()-1]
   * - one of the transformation types transformType_t that tells the callback 
   * function the type of shifting being executed
   * - the data for the column being shifted out
   * 
   * \param cb	the address of the function to be called from the library.
   * \return No return data
   */
  void setShiftDataOutCallback(void (*cb)(uint8_t dev, transformType_t t, uint8_t colData)) { _cbShiftDataOut = cb; };
  
private:


  // SPI interface data
  uint8_t	_dataPin;		  // DATA is shifted out of this pin ...
  uint8_t	_clkPin;		  // ... signaled by a CLOCK on this pin ...
  uint8_t	_csPin;			  // ... and LOADed when the chip select pin is driven HIGH to LOW
  bool		_hardwareSPI;	// true if SPI interface is the hardware interface
	

  uint8_t*	_spiData;		// data buffer for writing to SPI interface

  // User callback function for shifting operations
  uint8_t	(*_cbShiftDataIn)(uint8_t dev, transformType_t t);
  void		(*_cbShiftDataOut)(uint8_t dev, transformType_t t, uint8_t colData);
	


  // Private functions
  void spiSend(void);			    // do the actual physical communications task
  void spiClearBuffer(void);	// clear the SPI send buffer
  void controlHardware(uint8_t dev, controlRequest_t mode, int value);	// set hardware control commands
  void controlLibrary(controlRequest_t mode, int value);	// set internal control commands

  void flushBuffer(uint8_t buf);// determine what needs to be sent for one device and transmit
  void flushBufferAll();			  // determine what needs to be sent for all devices and transmit

  uint8_t bitReverse(uint8_t b);	// reverse the order of bits in the byte
  bool transformBuffer(uint8_t buf, transformType_t ttype);	// internal transform function

  bool copyRow(uint8_t buf, uint8_t rSrc, uint8_t rDest);	  // copy a row from Src to Dest
  bool copyColumn(uint8_t buf, uint8_t cSrc, uint8_t cDest);// copy a row from Src to Dest
};

#endif