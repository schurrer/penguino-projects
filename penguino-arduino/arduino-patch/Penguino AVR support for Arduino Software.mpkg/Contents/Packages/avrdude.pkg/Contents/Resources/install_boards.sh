#!/bin/sh
cd /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino
cat boards_addition.txt boards.txt > new_boards.txt
mv new_boards.txt boards.txt
rm boards_addition.txt
