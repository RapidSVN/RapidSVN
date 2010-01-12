#!/bin/bash
#
# ====================================================================
# Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program (in the file GPL.txt.
# If not, see <http://www.gnu.org/licenses/>.
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
  echo $DIR
  msgfmt -o $MO $PO
done

for DIR in ??_?? ; do
  PO="$DIR/rapidsvn.po"
  MO="$DIR/rapidsvn.mo"
  echo $DIR
  msgfmt -o $MO $PO
done

# end of file
