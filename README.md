# RP2040 Project Examples
A collection of simple projects that can be used  
to better understand the RP2040 SDK. Intended to serve as  
either the basis for larger projects, or act as small  
standalone tools.  
  

## Multicore i2c Peripheral Mode
A small multicore i2c build. In this configuration  
the RP2040 is intended to be a low speed peripheral controller  
for a larger system that needs to offload non-critical tasks.  
  
Examples may include:
- Driving servos, LEDs, or other PWM driven devices
- Long term data collection and monitoring
- Communications node between a faster processor and slower peripherals
  

## PicoSPI For iCE Tests
This program accompanies the small simple SPI peripheral program  
I've written for the iCE40 series FPGA.  
A serial interface is provided, intended to collect user input  
and present sent/read data.  
  
Serial Commands:
- `S` Enter hex string creation mode (up to 4 vals), press `Enter` to break early
- `T` Transmit full test buffer over SPI (Mode 0, MSB first), prints resultant read
- `t` Send byte 0 of the test buffer over SPI (Mode 0, MSB first), prints resultant read
- `P` Print current test buffer
  
Important Note: the RP2040 SDK LSB first parameter in the SPI format configuration appears  
to do nothing, every transmission is MSB first.