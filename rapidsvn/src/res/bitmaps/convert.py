#!/usr/bin/env python

import os
import glob

g=glob.glob("*.xpm")
for x in g:
    y=x.replace(".xpm", ".bmp")
    os.system("xpmtoppm %s | ppmtobmp > %s" % (x, y))
