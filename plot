#!/usr/bin/gnuplot

set terminal png size 400, 300

#gyroscope
set output 'assets/gyro1.png'
plot 'txt/gyro.txt' using 1:2 w l

set output 'assets/gyro2.png'
plot 'txt/gyro.txt' using 1:3 w l

set output 'assets/gyro3.png'
plot 'txt/gyro.txt' using 1:4 w l

#velocity
set output 'assets/vel1.png'
plot 'txt/vel.txt' using 1:2 w l

set output 'assets/vel2.png'
plot 'txt/vel.txt' using 1:3 w l

set output 'assets/vel3.png'
plot 'txt/vel.txt' using 1:4 w l

#accelometer
set output 'assets/accel1.png'
plot 'txt/accel.txt' using 1:2 w l

set output 'assets/accel2.png'
plot 'txt/accel.txt' using 1:3 w l

set output 'assets/accel3.png'
plot 'txt/accel.txt' using 1:4 w l

#position
set output 'assets/pos1.png'
plot 'txt/pos.txt' using 1:2 w l

set output 'assets/pos2.png'
plot 'txt/pos.txt' using 1:3 w l

set output 'assets/pos3.png'
plot 'txt/pos.txt' using 1:4 w l
