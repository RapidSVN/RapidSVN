#/bin/bash
#Script to enforce rapidsvn coding styles
#
#Usage example to beautify all of the cpp files in src:
#>../tools/beautify.sh *.cpp

INDENT="indent -nut --indent-level2 --braces-after-if-line --brace-indent0 --no-blank-lines-after-commas --declaration-indentation1"
for FILE in $@ ; do 
  if test ! -f $FILE ; then
    echo "Invalid file $FILE" >&2
  else
    mv $FILE $FILE.orig
    #now for the style
    $INDENT $FILE.orig -o $FILE
  fi
done

