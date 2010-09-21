#!/bin/bash

# Compile PO files:
for POLANG in $(cat LANGUAGES)
do
	mkdir -p $POLANG/LC_MESSAGES/
	msgfmt -c -v -o $POLANG/LC_MESSAGES/violetland.mo $POLANG.po
done
