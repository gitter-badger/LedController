#pragma once

#if (ARDUINO >= 100)
#include <Arduino.h>
#define PRINTLN(x) Serial.println(x)
#else
#if __has_include("ArduinoFake.h")
#include "ArduinoFake.h"
#include <iostream>
#define PRINTLN(x) std::cout << x << std::endl
#else
#include <WProgram.h>
#define PRINTLN(x) Serial.println(x)
#endif
#endif

/**
 * @brief This class is ued to handle the configuration of the LedController
 *
 */
class controller_configuration {
public:
  /**
   * @brief The pin for the data transfer (MOSI on board and DIN on Matrix)
   * @warning if this is 0 and useHardwareSpi is false the initilization will
   * fail
   */
  unsigned int SPI_MOSI = 0;

  /**
   * @brief The pin for the chip select signal (CS).
   * @warning this has to be set if it is 0 the initlization will fail.
   */
  unsigned int SPI_CS = 0;

  /**
   * @brief The pin for the clock signal (CLK)
   * @warning if this is 0 and useHardwareSpi is false the initilization will
   * fail
   */
  unsigned int SPI_CLK = 0;

  /**
   * @brief The total number of connected Segments.
   * @warning while in theory it can be any number be careful not to run out of
   * memory.
   *
   */
  unsigned int SegmentCount = 4;

  /**
   * @brief true if you want to use hardware SPI (view
   * https://www.arduino.cc/en/Reference/SPI for pin config). While this is a
   * lot faster you cannot use every pin for the MOSI and CLK signal. SPI_MOSI
   * and SPI_CLK will be set automatically if this is true.
   */
  bool useHardwareSpi = false;

  /**
   * @brief The current brightness level of the leds.
   * @warning The value needs to be between 0 and 15 (inclusive bounds).
   */
  unsigned int IntensityLevel = 1;

  /**
   * @brief The number of rows the Matrix should have.
   * @warning Currently not in use, will be used in v2.0.0
   * @note The matrix has to be a rectangle if this is not the case the number
   * of rows will be set to 1.
   *
   */
  unsigned int rows = 1;

  /**
   * @brief This Arrays specifies which Pin to use for each row if nullptr
   * SPI_CS will be used.
   * @note If this array is not a nullptr it is assumed it is the same length as
   * the number of rows.
   * @warning Currently not in use, will be used in v2.0.0
   *
   */
  unsigned int *row_SPI_CS = nullptr;

  /**
   * @brief Only send data if something changed if true.
   * If this is true, new data will only be sent if the data given differes from the data internally stored.
   */
  bool onlySendOnChange = true;

  /**
   * @brief The speed which with the hardware spi should transfer the data to the matrix
   * 
   */
  unsigned int spiTransferSpeed = 8000000;

  /**
   * @brief check of this configuration is valid
   * 
   * @return true the configuration is valid
   * @return false the configuration is not valid
   */
  bool isValid() const{
    return isValidConfig(*this);
  }

  /**
   * @brief This function returns the row a given segment is in.
   * 
   * @param segmentNumber The index of the segment which you want the row of.
   * @return unsigned int The row the given segment is in.
   */
  unsigned int getRow(unsigned int segmentNumber) const{
    unsigned int row = 0;
    if (rows != 0 && SegmentCount != 0){
      row = segmentNumber / (SegmentCount / rows);
    }
    
    if(row >= rows){
      row = 0;
    }

    return row;
  }

  /**
   * @brief This function returns a copy of this configuration
   * 
   * @return controller_configuration a copy of the configuration
   */
  controller_configuration copy() const{
    controller_configuration conf;
    conf.IntensityLevel = this->IntensityLevel;
    conf.onlySendOnChange = this->onlySendOnChange;
    conf.row_SPI_CS = new unsigned int [this->rows];
    for(unsigned int i = 0; i < this->rows;i++){
      conf.row_SPI_CS[i] = this->row_SPI_CS[i];
    }
    conf.rows = this->rows;
    conf.SegmentCount = this->SegmentCount;
    conf.SPI_CLK = this->SPI_CLK;
    conf.SPI_CS = this->SPI_CS;
    conf.SPI_MOSI = this->SPI_MOSI;
    conf.useHardwareSpi = this->useHardwareSpi;
    return conf;
  }

  /**
   * @brief check if a given configuragtion is valid
   * 
   * @param conf the configuration that should be checked
   * @return true the configuration is valid
   * @return false the configuration is not valid 
   */
  static bool isValidConfig(const controller_configuration &conf) {
    //check if the dimenstions are valid
    if (conf.SegmentCount % conf.rows != 0){
      return false;
    }

    // checking the clk amd mosi pins
    if (!conf.useHardwareSpi) {
      if (conf.SPI_CLK == 0) {
        PRINTLN(
            "No CLK Pin given. Specify one or set useHardwareSpi to true");
        return false;
      }

      if (conf.SPI_MOSI == 0) {
        PRINTLN(
            "No MOSI Pin given. Specify one or set useHardwareSpi to true");
        return false;
      }
    }

    // checking the cs pin(s)
    if (conf.SPI_CS == 0 && conf.row_SPI_CS == nullptr) {
      PRINTLN("No CS Pin given");
      return false;
    }

    if (conf.row_SPI_CS != nullptr &&
        sizeof(conf.row_SPI_CS) != sizeof(unsigned int) * conf.rows) {
      PRINTLN("Wrong row_SPI_CS size, it does not match conf.rows");

      if (conf.SPI_CS != 0) {
        PRINTLN("Falling back to SPI_CS for every row (assuming all "
                       "segments are connected in series)");
      } else {
        PRINTLN("Falling back to SPI_CS not possible because it is 0");
        return false;
      }
    }
    return true;
  }
};

#ifdef PROGMEM
const static byte charTable[] PROGMEM = {
#else
const static byte charTable[] = {
#endif
  B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111,
  B01110000, B01111111, B01111011, B01110111, B00011111, B00001101, B00111101,
  B01001111, B01000111, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B10110000, B00100010,
  B00111111, B01011011, B10100101, B00110001, B00000010, B01001010, B01101000,
  B01000010, B00000111, B00000100, B00000001, B10000000, B00100101, B01111110,
  B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000,
  B01111111, B01111011, B01001000, B01011000, B01000011, B00001001, B01100001,
  B11100101, B01111101, B01110111, B00011111, B01001110, B00111101, B01001111,
  B01000111, B01011110, B00110111, B00000110, B00111100, B01010111, B00001110,
  B01010100, B01110110, B01111110, B01100111, B01101011, B01100110, B01011011,
  B00001111, B00111110, B00111110, B00101010, B00110111, B00111011, B01101101,
  B01001110, B00010011, B01111000, B01100010, B00001000, B00100000, B01111101,
  B00011111, B00001101, B00111101, B01101111, B01000111, B01111011, B00010111,
  B00000100, B00011000, B01010111, B00000110, B00010100, B00010101, B00011101,
  B01100111, B01110011, B00000101, B01011011, B00001111, B00011100, B00011100,
  B00010100, B00110111, B00111011, B01101101, B00110001, B00000110, B00000111,
  B01000000, B00000000
};
