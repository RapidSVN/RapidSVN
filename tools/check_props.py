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
# This script checks and sets subversion properties for the 
# RapidSVN source tree like svn:mime-type or svn:eol-style 
#
# Remark: execute this script in <rapidsvn tree>
#
# Usage:
# python check_props.py
from os.path import walk
from subprocess import Popen

def set_prop(filename, prop, value):
  args=['svn', 'ps', prop, value, filename]
  p=Popen(args, stdout=None)
  t=p.communicate()[0]

TEXT_EXTENSIONS=['.cpp', '.hpp', '.py', '.sh', '.txt', '.html', '.map', '.md5', '.xpm']

def is_text(fname):
  fname = fname.lower()
  for ext in TEXT_EXTENSIONS:
    if fname.endswith(ext):
      return True
  
  return False

MIME_TYPES={
  '.png': 'image/png',
  '.ico': 'image/x-icon',
  '.xpm': 'image/x-xpm'
}

def walk_func(arg, dirname, fnames):
  for fname in fnames:
    fpath = dirname + '/' + fname
    if is_text(fname):
      set_prop(fpath, 'svn:eol-style', 'native')
      
    ext=fname[-4:].lower()
    if MIME_TYPES.has_key(ext):
      set_prop(fpath, 'svn:mime-type', MIME_TYPES[ext])

walk('.', walk_func, None)
