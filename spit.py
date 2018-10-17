import spidev

spi = spidev.SpiDev()
spi.open(0,1)
spi.max_speed_hz = 10000
while True:
    spi.writebytes([0x0A, 0x00])
