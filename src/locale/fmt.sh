#!/bin/bash
for X in de fr pt_BR ru uk zh_CN ; do
	msgfmt -o "$X/rapidsvn.mo" "$X/rapidsvn.po"
done
