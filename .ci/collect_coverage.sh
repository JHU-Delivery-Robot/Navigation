#!/bin/bash
set -e # immediately exit on error

# regex documentation
# .*\.   -- match a file extension
#  (?:   -- with either (group)
#    cpp -- the extension cpp
#    |   -- or
#    c   -- the extension c
#  )
# $      -- only match if the regex is at the end of the line
# SOURCES=$(find . \( -path ./lib -o -path './Image Testing' \) -prune -o -print \
          # | grep -P '.*\.(?:cpp|c)$')
# echo $SOURCES

# for FILENAME in $SOURCES;
# do
    # echo $FILENAME
    # gcov -o "$(dirname $FILENAME)/build" $FILENAME > /dev/null
# done

gcovr -e lib -e tests -d --json -o ci-cov.json
sed -i 's/build\///g' ci-cov.json
gcovr --add-tracefile ci-cov.json --xml -o cov-report.xml

