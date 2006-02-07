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
# representation so the application can load this during runtime
#
# Remark: execute this script in <rapidsvn tree>/src/locale/<language>
#
# Usage:
# >../../../tools/compile-po.sh

PO="rapidsvn.po"
MO="rapidsvn.mo"

msgfmt -o$MO $PO

# end of file