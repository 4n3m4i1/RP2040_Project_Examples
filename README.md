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


## MIDI Keyboard
I needed a MIDI keyboard with an out and didn't have one, so here it is.  
Supports dynamic channel allocation such that up to a specified `NUM_VOICES`  
keys can be pressed at a time, but any presses over this value will not be registered  
as a valid input. Of course the release of any pin will release the associated channel,  
and the next press of any button will claim that channel.  
  
Support for Modulation, Volume, some arbitrary data, and octave switching coming soon,  
most support is baked in but not broken out yet.  
  
(REMEMBER TO PUT THE BOARD PICTURE IN THE FOLDER)
