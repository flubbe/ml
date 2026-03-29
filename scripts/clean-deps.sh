#!/bin/bash

#
# Delete dependencies.
#
if [ -d "../3rd-party/cnl" ]; then rm -Rf "../3rd-party/cnl"; fi

#
# Delete directory if empty
#
rmdir "../3rd-party"
