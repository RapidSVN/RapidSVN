#!/bin/bash
#
# ====================================================================
# Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
# This file merges the translatable strings from the translation
# template rapidsvn.pot with the actual translation rapidsvn.po
#
# Remark: execute this script in <rapidsvn tree>/src/locale/<language>
#
# Usage:
# >../../../tools/update-po.sh

POT="../rapidsvn.pot"
PO="rapidsvn.po"
NEW_PO="rapidsvn.po.new"
OLD_PO="rapidsvn.po.old"
if test ! -f $POT ; then
  echo "rapidsvn.pot not found    MAKE SURE YOU EXECUTE THIS SCRIPT IN"
  echo "                          <rapidsvn tree>/src/locale/<language>"
  exit 1
fi

OPTS="-o$NEW_PO $PO $POT"
msgmerge $OPTS

CHANGED=`diff -q $PO $NEW_PO`
if test -z "$CHANGED" ; then
  echo "$PO not changed"
else
  echo "$PO has been changed"
  mv $PO $OLD_PO
  mv $NEW_PO $PO
fi

# end of file
