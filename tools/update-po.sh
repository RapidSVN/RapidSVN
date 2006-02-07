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