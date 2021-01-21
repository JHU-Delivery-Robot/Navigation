#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

VG_CMD="valgrind --error-exitcode=64 --leak-check=full --track-origins=yes"
FAIL="no"

recordFail() {
    if [[ $? -eq 64 ]]; then
        FAIL="yes"
    fi
}

echo
$VG_CMD ./roboctrl
recordFail
echo
$VG_CMD ./robosim
recordFail
echo
$VG_CMD ./simtests
recordFail

if [[ $FAIL == "yes" ]]; then
    echo -e "${RED}ERROR: Some valgrind checks failed. Check the log for more info.${NC}"
    exit 1
fi
echo -e "${GREEN}All Valgrind checks passed.${NC}"
exit 0
