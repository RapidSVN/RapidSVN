#/bin/bash
#Script to enforce rapidsvn coding styles
#
#Usage example to beautify all of the cpp files in src:
#>../tools/beautify.sh *.cpp

INDENT="indent --indent-level2 --braces-after-if-line --brace-indent0 --no-blank-lines-after-commas -nut --declaration-indentation1"
for FILE in $@ ; do 
  if test ! -f $FILE ; then
    echo "Invalid file $FILE" >&2
  else
    mv $FILE $FILE.orig
    #remove tabs
    tr "\t" " " < $FILE.orig > $FILE.trout
    #no for the style
    $INDENT $FILE.trout -o $FILE
    rm $FILE.trout
  fi
done

