#!/bin/sh

__option=$1

if [ "x$__option" = "x" ]; then
	exec dpkg-buildpackage -sa -rfakeroot
elif [ "x$__option" = "xno-sign" ]; then
	echo "This build command is only for packaging."
	exec dpkg-buildpackage -b -us -uc -rfakeroot
else
	echo "This options is invalid."
fi

