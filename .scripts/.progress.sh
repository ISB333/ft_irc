#!/bin/bash

HIDE_CURSOR="\033[?25l"
SHOW_CURSOR="\033[?25h"

RESET=$'\033[0m'
CARRIAGE_RETURN=$'\r'
ERASE=$'\033[K'
ERASE_LINE="${CARRIAGE_RETURN}${ERASE}"

TOTAL_FILES=$(find ./ -type f -name "*.cpp" | wc -l)
COMPILED_FILES=$(find ./.obj/ -type f -name "*.o" | wc -l)

PROGRESS=$((COMPILED_FILES * 100 / TOTAL_FILES))
R=$((255 - (255 * PROGRESS / 100)))
G=$((255 * PROGRESS / 100))
COLOR="\033[38;2;${R};${G};0m"

FILLED_LENGTH=$((PROGRESS * 50 / 100))
BAR=$(printf "%0.s█" $(seq 1 $FILLED_LENGTH))
EMPTY=$(printf "%0.s " $(seq 1 $((50 - FILLED_LENGTH))))

printf "${HIDE_CURSOR}${ERASE_LINE}${COLOR}|${BAR}${EMPTY}| [${PROGRESS}%%] (${COMPILED_FILES}/${TOTAL_FILES})${RESET}"

if [ "$COMPILED_FILES" -eq 0 ] || [ "$COMPILED_FILES" -eq "$TOTAL_FILES" ]; then
	printf "${ERASE_LINE}${SHOW_CURSOR}"
fi