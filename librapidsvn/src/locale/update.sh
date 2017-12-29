#!/bin/bash
#
# This script updates the file rapidsvn.pot
# and merges it with the existing translations
# in the language subdirectories

xgettext -C -k_ -s -o rapidsvn.pot ../*.cpp ../../include/*.h ../../../rapidsvn/*.cpp
for X in de fr hu it_IT pt_BR ru es uk zh_CN ja ; do
	msgmerge -U $X/rapidsvn.po rapidsvn.pot
done

