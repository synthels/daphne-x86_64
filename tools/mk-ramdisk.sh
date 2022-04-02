#!/bin/bash

echo "compiling ramdisk..."
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
ROOT="${SCRIPT_DIR}/.."
cd "${ROOT}/base"
tar cvf ../build/iso/ramdisk.tar.gz *
