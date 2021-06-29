#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_DIR="build"

cd "${SCRIPT_DIR}/.."

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
	mkdir ${BUILD_DIR}
fi

# Add a clean script to build
touch ${BUILD_DIR}/clean.sh

if [[ $1 = "-x64" ]]
then
	mkdir ${BUILD_DIR}/iso
	echo "#!/bin/sh
	rm -r ./*
	bash ../tools/setup.sh -x64" > ${BUILD_DIR}/clean.sh
fi
