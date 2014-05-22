#!/bin/bash -e
libtoolize --automake --copy --debug --force > /dev/null 2>&1
aclocal
autoconf
automake --add-missing > /dev/null
