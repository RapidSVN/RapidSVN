#!/bin/bash
#
# ====================================================================
# Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
# >check-props.sh

# text files get native line endings
find . -name "*.?pp" | xargs svn ps svn:eol-style native
find . -name "*.py" | xargs svn ps svn:eol-style native
find . -name "*.sh" | xargs svn ps svn:eol-style native
find . -name "*.txt" | xargs svn ps svn:eol-style native
find . -name "*.html" | xargs svn ps svn:eol-style native
find . -name "*.map" | xargs svn ps svn:eol-style native
find . -name "*.md5" | xargs svn ps svn:eol-style native

# images
find . -name "*.png" | xargs svn ps svn:mime-type image/png
find . -name "*.ico" | xargs svn ps svn:mime-type image/x-icon
find . -name "*.xpm" | xargs svn ps svn:mime-type image/x-xpm
find . -name "*.xpm" | xargs svn ps svn:eol-style native


# end of file
