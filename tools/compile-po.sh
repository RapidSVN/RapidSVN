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
# This script compiles the translated strings to the binary
# representation so the application can load this during runtime.
#
# All existing translation files are compiled.
#
# Remark: execute this script in <rapidsvn tree>/src/locale
#
# Usage:
# >../../../tools/compile-po.sh

POT="rapidsvn.pot"
if test ! -f $POT ; then
  echo "rapidsvn.pot not found    MAKE SURE YOU EXECUTE THIS SCRIPT IN"
  echo "                          <rapidsvn tree>/src/locale"
  exit 1
fi

for DIR in ?? ; do
  PO="$DIR/rapidsvn.po"
  MO="$DIR/rapidsvn.mo"
  msgfmt -o$MO $PO
done

# end of file