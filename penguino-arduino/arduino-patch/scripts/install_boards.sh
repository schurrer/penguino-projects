#!/bin/sh
cd /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino
if ! grep penguino boards.txt; then
	cat boards_addition.txt boards.txt > new_boards.txt;
	mv new_boards.txt boards.txt;
fi
rm boards_addition.txt
