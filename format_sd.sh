#!/bin/sh

umount_partions() {
	if [ $# -lt 1 ]; then
		echo parms: $*
		return 30
	fi

	DEV=$1
	test -b $DEV || return 31

	#for PART in `fdisk -l ${DEV} | grep ${DEV}p | awk -F ' ' '{print $1}'`
	#do
	#	path=`cat /proc/mounts | grep -w ${PART} | awk -F ' ' '{print $2}'`
	#	if [ -n "${path}" ]; then
	#		/bin/busybox umount -f ${path} 
	#	fi
	#done

	for path in `cat /proc/mounts | grep ${DEV} | awk -F ' ' '{print $2}'`
	do
		if [ -n "${path}" ]; then
			/bin/busybox umount -f ${path} 
		fi
	done

	return 0
}

rm_partions() {
	if [ $# -lt 1 ]; then
		echo parms
		return 10
	fi

	DEV=$1
	test -b $DEV || return 12

	num=`fdisk -l ${DEV} | grep ${DEV}p | wc -l`
	if [ $num -gt 1 ]; then
		for PN in `fdisk -l ${DEV} | grep ${DEV}p | awk -F ' ' '{print $1}' | awk -F 'p' '{print $2}' | sort -r | sed '$d'`
		do
			fdisk ${DEV} << EOF
d
${PN}
w
EOF

			ret=$?
			if [ $ret -ne 0 ]; then
				return $ret
			fi

		done

	fi

	num=`fdisk -l ${DEV} | grep ${DEV}p | wc -l`
	if [ $num -eq 1 ]; then
		fdisk ${DEV} << EOF
d
w
EOF

		ret=$?
		if [ $ret -ne 0 ]; then
			return $ret
		fi
	fi

	num=`fdisk -l ${DEV} | grep ${DEV}p | wc -l`
	if [ ${num} -ne 0 ]; then
		return 11
	fi

	return 0
}

format() {
	if [ $# -lt 2 ]; then
		echo parms
		return 20
	fi

	DEV=$1
	FT=$2

	test -b $DEV || return 23

	if [ "$FT" == "exfat" ]; then
		T=7
		FS=exfat
	elif [ "$FT" == "fat32" ]; then
		T=b
		FS=vfat
	else
		echo "Usage: $0 {exfat|fat32} {umount|rm|format|remount}" >&2
		return 21
	fi

	dd if=/dev/zero of=${DEV} count=1 bs=446
	dd if=/dev/zero of=${DEV} bs=512 skip=1 count=15

	fdisk ${DEV} << EOF
n
p
1


t
${T}
w
EOF
	ret=$?
	if [ $ret -ne 0 ]; then
		return $ret
	fi

	TYPE=`fdisk -l $DEV | grep ${DEV}p1 | awk -F ' ' '{printf $8}'`

	if [ "${TYPE}" == "${T}" ]; then
		mkfs.${FS} ${DEV}p1 -n GoStream
		return $?
	else
		echo "fdisk error - ${TYPE}"
		return 22
	fi

	return 0
}

if [ $# -lt 2 ]; then
	echo "Usage: $0 {exfat|fat32} {umount|rm|format|remount}" >&2
	exit 1
fi

TP=$1

DEV=`fdisk -l | grep Disk | grep /dev/mmcblk[0-9]: | grep -v mmcblk0 | awk -F ' ' '{print $2}' | awk -F ':' '{print $1}'`

test -z "$DEV" && exit 3
test -b $DEV || exit 4

case "$2" in
	umount)
		ret=`umount_partions $DEV`
		ret=$?
		if [ $ret -ne 0 ]; then
			exit $ret
		fi

		;;
	rm)
		ret=`cat /proc/mounts | grep ${DEV} | wc -l`
		if [ $ret -ne 0 ]; then
			exit 3
		fi

		rm_partions $DEV
		ret=$?
		if [ $ret -ne 0 ]; then
			exit $ret
		fi

		;;
	format)
		ret=`cat /proc/mounts | grep ${DEV} | wc -l`
		if [ $ret -ne 0 ]; then
			exit 3
		fi

		ret=`fdisk -l ${DEV} | grep ${DEV}p | wc -l`
		if [ $ret -ne 0 ]; then
			exit 4
		fi

		format $DEV $TP
		ret=$?
		if [ $ret -ne 0 ]; then
			exit $ret
		fi

		;;
	remount)
		unset OP

		ret=`fdisk -l ${DEV} | grep ${DEV}p1 | wc -l`
		if [ $ret -ne 1 ]; then
			exit 5
		fi

		ret=`cat /proc/mounts | grep ${DEV}p1 | wc -l`
		if [ $ret -eq 1 ]; then
			OP="-o remount"
		fi

		mount ${DEV}p1 /mnt/sdcard ${OP}
		ret=$?
		if [ $ret -ne 0 ]; then
			if [ -n "$OP" ]; then
				OP=${OP},nonempty
			else
				OP="-o nonempty"
			fi

			mount ${DEV}p1 /mnt/sdcard ${OP}
			ret=$?
			if [ $ret -ne 0 ]; then
				exit $ret
			fi
		fi

		;;
	*)
		echo "Usage: $0 {exfat|fat32} {umount|rm|format|remount}" >&2
		exit 2
		;;
	esac

exit 0
