#!/usr/bin/env bash

# Before running this script, make sure that project is build in Debug mode
# and tests are executed

# collect coverage info
lcov --capture --directory . --output-file coverage.info --no-external --quiet

# remove test files from report
lcov --remove coverage.info "*tests*" --output-file coverage.info --no-external --quiet

# display report in console
lcov --list coverage.info

#clean
rm coverage.info