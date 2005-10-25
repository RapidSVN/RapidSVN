#/bin/bash
#
# ====================================================================
# Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
#
# This software is licensed as described in the file LICENSE.txt,
# which you should have received as part of this distribution.
#
# This software consists of voluntary contributions made by many
# individuals.  For exact contribution history, see the revision
# history and logs, available at http://rapidsvn.tigris.org/.
# ====================================================================
#
# script to enforce rapidsvn coding styles
#
# Usage example to beautify all of the cpp files in src:
# >../tools/beautify.sh *.cpp

INDENT="indent -nut --indent-level2 --braces-after-if-line --brace-indent0 --no-blank-lines-after-commas --declaration-indentation1"
for FILE in $@ ; do 
  if test ! -f $FILE ; then
    echo "Invalid file $FILE" >&2
  else
    mv $FILE $FILE.orig
    #now for the style
    $INDENT $FILE.orig -o $FILE
    rm $FILE.orig
  fi
done

