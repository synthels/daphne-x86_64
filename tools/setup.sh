#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_DIR="build"

cd "${SCRIPT_DIR}/.."

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
    mkdir ${BUILD_DIR}
fi

if [[ $1 = "x86_64" ]]
then
    mkdir ${BUILD_DIR}/iso
    cp limine.cfg build/iso
fi
