# Example of an i2C peripheral device that utilizes both cores
Will RX up to 16 bytes (full RX FIFO), then read all bytes out via serial.  
Once this occurs the first core is notified via mailbox and the on board  
LED (pico board, GPIO 25) will toggle.
