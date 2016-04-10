
# `pulseIn()` vs ISR

The issue with `pulseIn()` is that sometimes it misses the start of the pulse, which makes the resultant output very twitchy:

`pulseIn()`
```
...
rudder, pulse: 1523	angle: 94
rudder, pulse: 1517	angle: 93
rudder, pulse: 1517	angle: 93
rudder, pulse: 1523	angle: 94
rudder, pulse: 1517	angle: 93
rudder, pulse: 1502	angle: 90
rudder, pulse: 1484	angle: 87
rudder, pulse: 1523	angle: 94
rudder, pulse: 1517	angle: 93
rudder, pulse: 1523	angle: 94
rudder, pulse: 1517	angle: 93
rudder, pulse: 1525	angle: 94
rudder, pulse: 1517	angle: 93
rudder, pulse: 1523	angle: 94
rudder, pulse: 1525	angle: 94
rudder, pulse: 1496	angle: 89
rudder, pulse: 1492	angle: 88
rudder, pulse: 1495	angle: 89
rudder, pulse: 1523	angle: 94
rudder, pulse: 1517	angle: 93
...
```

An interrupt service routine (ISR) solves this problem by using built-in [interrupts](http://arduino.cc/en/Reference/AttachInterrupt) to more accurately monitor the pulses from the RC receiver.

ISR
```
...
rudder, pulse: 1508	angle: 91		sail, pulse: 1364	angle: 65
rudder, pulse: 1508	angle: 91		sail, pulse: 1364	angle: 65
rudder, pulse: 1508	angle: 91		sail, pulse: 1364	angle: 65
rudder, pulse: 1508	angle: 91		sail, pulse: 1364	angle: 65
rudder, pulse: 1504	angle: 91		sail, pulse: 1372	angle: 65
rudder, pulse: 1504	angle: 90		sail, pulse: 1372	angle: 66
rudder, pulse: 1504	angle: 90		sail, pulse: 1372	angle: 66
rudder, pulse: 1504	angle: 90		sail, pulse: 1372	angle: 66
rudder, pulse: 1504	angle: 90		sail, pulse: 1372	angle: 66
rudder, pulse: 1508	angle: 91		sail, pulse: 1364	angle: 65
...
```

Notice that now we've gone from +-39 to +-4. Much better!

It's still not perfect, so a final solution might have a low pass filter that rejects small changes. Eg, something like:

```
if (abs((newValue - oldValue)) > 8) {
   // Only assign if the difference is greater than 8 units.
   result = newValue;
}
```



# Testing

Things to check when testing:

1. What is the minimum and maximum RC angle for the sail channel? rudder channel?
2. How does the RC transmitter trim affect the readings, for sail? for rudder?
3. How does the RC transmitter ST Rate knob affect the readings, for sail? for rudder?

# Credits

PWM modified from [here](http://www.camelsoftware.com/firetail/blog/radio/reading-pwm-signals-from-a-remote-control-receiver-with-arduino/)

