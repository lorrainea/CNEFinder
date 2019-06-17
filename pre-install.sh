#! /bin/sh

unzip boost.zip || exit 1 # if unzip is missing, the next command removes boost.zip - new cloning is necessary to restore it
rm boost.zip
