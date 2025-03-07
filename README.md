# IMP
Inertial Measurement Processor is a small utility for processing the data from IMUs. 
This project was created for processing the data from
[RP2040 MCU with BMI160 IMU altimeter](https://github.com/sykacek/rp2040-bmi160-gyroscope), this is only supported project at the time.

## How to build

Below there are specific instructions for [RP2040 altimeter](https://github.com/sykacek/rp2040-bmi160-gyroscope)
target.
The project consists of 3 components:

1. Memory extractor
2. Data Processing Unit
3. Plotting script

### Memory extractor

First, we need to get the data from the MCU. This is done using `picotool`. For building picotool please
refer to [this page](https://github.com/raspberrypi/picotool). To get the whole memory image run
```sh
	picotool save -a test.bin
```

That will copy the whole memory into the file `test.bin`, which will be processed afterwards.

### Data Processing Unit

Build process is managed via `make`, so make sure you have installed `make` and `gcc`.
For example, for Debian based distributions (Ubuntu, Kali etc.) run

```sh
	sudo apt install make gcc
```

The binary can be then build simply using
```sh
	make
```

### Plotting script

Plotting is managed via `gnuplot`. To install gnuplot on Debian based ditributions run
```sh
	sudo apt install gnuplot
```

To generate graphs simply run
```sh
	./gnuplot
```

