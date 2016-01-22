#!/bin/bash
set -e



PACKAGES='g++ cmake gettext libsdl-ttf2.0-dev libsdl-mixer1.2-dev libsdl-image1.2-dev libboost-all-dev'

sudo apt-get update
sudo apt-get install -y $PACKAGES

