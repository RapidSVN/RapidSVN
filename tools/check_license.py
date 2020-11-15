#!/usr/bin/env python
# ====================================================================
# Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
# along with this program (in the file GPL.txt).
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
import sys
import re
import os.path

OLD_LICENSE_TXT = r'''\
=======[=]+
Copyright \(c\) 2002-2[0-9]+ The RapidS[vV][nN] Group.  All rights reserved.[ ]*
[^=]*=======[=]+'''

OLD_LICENSE_SHELL = r'''\
# =======[=]+
# Copyright \(c\) 2002-2[0-9]+ The RapidS[vV][nN] Group.  All rights reserved.[ ]*
[^=]*=======[=]+'''

OLD_LICENSE_CPP = r'''/\*
 \* =======[=]+
 \* Copyright \(c\) 2002-2[0-9]+ The RapidS[vV][nN] Group.  All rights reserved.[ ]*
[^=]*=======[=]+
 \*/'''

# Remember not to do regexp quoting for NEW_LICENSE_*.  Only OLD_LICENSE
# is used for matching; NEW_LICENSE_* is inserted as-is.
NEW_LICENSE_TXT_GPL = '''\
====================================================================
Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program (in the file GPL.txt).
If not, see <http://www.gnu.org/licenses/>.

This software consists of voluntary contributions made by many
individuals.  For exact contribution history, see the revision
history and logs, available at http://rapidsvn.tigris.org/.
===================================================================='''

NEW_LICENSE_TXT_LGPL = '''\
====================================================================
Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program (in the file LGPL.txt).
If not, see <http://www.gnu.org/licenses/>.

This software consists of voluntary contributions made by many
individuals.  For exact contribution history, see the revision
history and logs, available at http://rapidsvn.tigris.org/.
===================================================================='''

NEW_LICENSE_CPP_GPL = '/*\n * ' + NEW_LICENSE_TXT_GPL.replace('\n', '\n * ').replace('* \n', '*\n') + '\n */'
NEW_LICENSE_CPP_LGPL = '/*\n * ' + NEW_LICENSE_TXT_LGPL.replace('\n', '\n * ').replace('* \n', '*\n') + '\n */'
NEW_LICENSE_SHELL_GPL = "\n# " + NEW_LICENSE_TXT_GPL.replace('\n', '\n# ').replace('# \n', '#\n')
NEW_LICENSE_SHELL_LGPL = "\n# " + NEW_LICENSE_TXT_LGPL.replace('\n', '\n# ').replace('# \n', '#\n')

converters = [
  (('.h', '.hpp', '.c', '.cpp', ), re.compile(OLD_LICENSE_CPP), NEW_LICENSE_CPP_GPL, NEW_LICENSE_CPP_LGPL),
  (('.py', '.sh', ), re.compile(OLD_LICENSE_SHELL), NEW_LICENSE_SHELL_GPL, NEW_LICENSE_SHELL_LGPL),
  (('.txt', ), re.compile(OLD_LICENSE_TXT), NEW_LICENSE_TXT_GPL, NEW_LICENSE_TXT_LGPL),
  ]


def get_converter(fname):
    if fname.endswith('.png.h'):
      return None
    _, ext = os.path.splitext(fname)
    for converter in converters:
      if ext in converter[0]:
        return converter[1:]
    return None

def check_file(fname):
  converter = get_converter(fname)
  if not converter:
    return
  r, _, _ = converter
  print(fname, end=' ')
  s = open(fname).read()
  if r.search(s):
    print("OK")
  else:
    print("MISSING")

def change_license(fname, license):
  converter = get_converter(fname)
  if not converter:
    return

  r, new_license_gpl, new_license_lgpl = converter
  if license == 'GPL':
    new_license = new_license_gpl
  elif license == 'LGPL':
    new_license = new_license_lgpl
  else:
    assert False, license

  s = open(fname).read()
  m = r.search(s)
  if not m:
    print('ERROR: missing old license:', fname)
  else:
    new_s = s[:m.start()] + new_license + s[m.end():]
    if s != new_s:
        open(fname, 'w').write(new_s)
        print('Changed:', fname)

if __name__ == '__main__':
  if sys.argv[1] in ('-GPL', '-LGPL'):
    license = sys.argv[1][1:]
    print('Changing license text to %s...' % license)
    for f in sys.argv[2:]:
      change_license(f, license)
  else:
    for f in sys.argv[1:]:
      check_file(f)
