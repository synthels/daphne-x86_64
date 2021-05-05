args=("")

# Parse args
for arg in "$@"; do
	if [ $arg == "-iso" ]; then
		args+=("-iso")
	elif [ $arg == "-qemu" ]; then
		args+=("-qemu")
	elif [ $arg == "-help" ]; then
		echo "Usage: phiOS.sh [options]"
		echo "Options:"
		echo " -iso   Output an ISO file alongside the binary"
		echo " -qemu  Test using QEMU"
		exit 0
	fi
done

source "kernel/build.sh" ${args[@]}
