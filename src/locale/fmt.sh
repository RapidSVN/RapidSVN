#!/bin/bash
for X in de fr it_IT pt_BR ru es uk zh_CN ; do
	msgfmt -o "$X/rapidsvn.mo" "$X/rapidsvn.po"
done
