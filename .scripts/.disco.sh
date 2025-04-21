#!/bin/bash

source ./.scripts/.source.sh

aplay "./.scripts/.techno.wav" & SOUND_PID=$!
for ((i = 1; i <= 42; i++)); do
	random_brightness=$(echo "scale=6; ($RANDOM / 32767) * 10" | bc)
	random_gamma1=$(echo "scale=6; ($RANDOM / 32767) * 10" | bc)
	random_gamma2=$(echo "scale=6; ($RANDOM / 32767) * 10" | bc)
	random_gamma3=$(echo "scale=6; ($RANDOM / 32767) * 10" | bc)
	xrandr --output eDP --brightness "$random_brightness"
	xrandr --output eDP --gamma "$random_gamma1:$random_gamma2:$random_gamma3"
	if [ $((RANDOM % 2)) -eq 0 ]; then
		xrandr --output eDP --rotate normal
	else
		xrandr --output eDP --rotate inverted
	fi
done
kill "$SOUND_PID"
xrandr --output eDP --brightness 1
xrandr --output eDP --gamma 1:1:1
xrandr --output eDP --rotate normal
clear
echo -e "${RED}WARNING: PLEASE DO NOT BE EPILEPTIC!${RESET}"
