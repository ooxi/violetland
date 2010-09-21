#!/bin/bash

# Generate a new POT file:
xgettext -o violetland.pot ../src/*.cpp ../src/*/*.cpp --keyword="_"

# Upgrade current PO files:
for POLANG in $(cat LANGUAGES)
do
  msgmerge -qU $POLANG.po violetland.pot
  rm $POLANG.po~ 2> /dev/null # Remove backup files. It would be better if they were just not created, though. 
done
