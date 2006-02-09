#!/bin/bash
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
# This script extracts all translatable strings and creates
# the translation template rapidsvn.pot
#
# Remark: execute this script in <rapidsvn tree>/src/locale
#
# Usage:
# >../../tools/update-pot.sh

POT="rapidsvn.pot"
NEW_POT="rapidsvn.pot.new"
OLD_POT="rapidsvn.pot.old"
if test ! -f $POT ; then
  echo "rapidsvn.pot not found    MAKE SURE YOU EXECUTE THIS SCRIPT IN"
  echo "                          <rapidsvn tree>/src/locale"
  exit 1
fi

OPTS="-k_ -s -o$NEW_POT ../*.cpp"
xgettext $OPTS

CHANGED=`diff -q $POT $NEW_POT`
if test -z "$CHANGED" ; then
  echo "$POT not changed"
  rm $NEW_POT
else
  echo "$POT has been changed"
  mv $POT $OLD_POT
  mv $NEW_POT $POT
fi

# end of file