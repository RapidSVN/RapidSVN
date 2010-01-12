#!/usr/bin/env python
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
# check if a file has the proper license in it
#
# USAGE: check-license.py [-GPL|-LGPL] file1 file2 ... fileN
#
# If the license cannot be found, then the filename is printed to stdout.
# Typical usage:
#    $ find . -name "*.[ch]pp" | xargs check-license.py > bad-files
#
# -GPL switch is used to change the license to GPL. Typical usage:
#    $ find . -name "*.[ch]pp" | xargs check-license.py -GPL
# -LGPL switch is used to change the license to LGPL. Typical usage:
#    $ find . -name "*.[ch]pp" | xargs check-license.py -LGPL

OLD_LICENSE = '''\
/\*
 \* ====================================================================
 \* Copyright \(c\) 2002-20[[0-9]+ The RapidS[vV][nN] Group.  All rights reserved.[ ]*
[^=]*====================================================================
 \*/
'''

# Remember not to do regexp quoting for NEW_LICENSE_*.  Only OLD_LICENSE
# is used for matching; NEW_LICENSE_* is inserted as-is.
NEW_LICENSE_GPL = '''\
/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
'''

import sys
import re

re_OLD = re.compile(OLD_LICENSE)

def check_file(fname):
  s = open(fname).read()
  if not re_OLD.search(s):
    print fname

def change_license(fname):
  s = open(fname).read()
  m = re_OLD.search(s)
  if not m:
    print 'ERROR: missing old license:', fname
  else:
    new_s = s[:m.start()] + NEW_LICENSE_GPL + s[m.end():]
    if s!=new_s:
        open(fname, 'w').write(new_s)
        print 'Changed:', fname

if __name__ == '__main__':
  if sys.argv[1] == '-GPL':
    print 'Changing license text to GPL...'
    for f in sys.argv[2:]:
      change_license(f)
  elif sys.argv[1] == '-LGPL':
    print 'LGPL is not supported anymore'
    sys.exit(1)
  else:
    for f in sys.argv[1:]:
      check_file(f)
