#! /usr/bin/env python
# -*- coding: iso-8859-1 -*-

from create_nightly import buildMessages
from os import chdir

if __name__ == "__main__":
   chdir("..")
   buildMessages()
