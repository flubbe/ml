#!/bin/bash

#
# Dependencies.
#

# Create folder structure
mkdir ../3rd-party
cd ../3rd-party

# Download cnl 1.1.2 and put it in ../3rd-party/cnl
wget https://github.com/johnmcfarlane/cnl/archive/refs/tags/v1.1.2.tar.gz
tar -xf v1.1.2.tar.gz
rm v1.1.2.tar.gz
mv cnl-1.1.2 cnl

# switch back to main folder
cd ..


#
# Set up folder structure for build.
#

mkdir ml/bin
mkdir ml/build
cd ml/build

