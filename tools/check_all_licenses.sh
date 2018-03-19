#!/bin/sh
find ../libsvncpp -type f | xargs ./check_license.py -LGPL
find ../librapidsvn -type f | xargs ./check_license.py -GPL
find ../rapidsvn -type f | xargs ./check_license.py -GPL
find . -type f | xargs ./check_license.py -GPL
