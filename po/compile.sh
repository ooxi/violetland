#!/bin/bash

# Compile PO files:
for POLANG in $(cat LANGUAGES)
do
	echo $POLANG :
	mkdir -p $POLANG/LC_MESSAGES/
	msgfmt -c -v -o $POLANG/LC_MESSAGES/violetland.mo $POLANG.po
done
