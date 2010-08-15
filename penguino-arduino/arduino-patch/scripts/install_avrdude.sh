#!/bin/sh
cd /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin
if ! [ -e avrdude-real ]; then
	mv avrdude avrdude-real;
fi
