#!/bin/sh

ulimit -c 0   # 禁止qt 段错误时生成core文件
EXECPATH=/oem/hw_rockchip_qt # 你要监测的程序所在目录
QTFILE=hw_rockchip_qt # 你要监测的程序名

sleepTime=1

get_pid()
{
        #ps -A | grep -w "$1" | awk '{print $1}'
	ps -A | grep -w "$1" | grep -v grep | awk '{print $1}'
}

run_qt()
{
	unset PID_QT

	if [ ! -d ${EXECPATH} ]; then
		logger -s "${EXECPATH} missing !"

		return 1
	fi

	cd ${EXECPATH}
	if [ ! -x ${QTFILE} ]; then
		logger -s "${QTFILE} missing !"

		return 2
	fi

	# 如果/userdata/hw_rockchip_qt/factory_reset 存在，将/oem/hw_rockchip_qt/qtkms.json复制到此位置
	if [ -e "/userdata/hw_rockchip_qt/factory_reset" ];then
		logger -s "factory_reset exist, will copy"
		if [ ! -d "/userdata/hw_rockchip_qt" ]; then
			mkdir /userdata/hw_rockchip_qt
		fi
		cp /oem/hw_rockchip_qt/qtkms.json /userdata/hw_rockchip_qt/qtkms.json
		sync
	fi


	# 如果/userdata/hw_rockchip_qt/qtkms.json不存在，将/oem/hw_rockchip_qt/qtkms.json复制到此位置
	if [ ! -e "/userdata/hw_rockchip_qt/qtkms.json" ];then
		logger -s "qtkms.json not exist, will copy"
		if [ ! -d "/userdata/hw_rockchip_qt" ]; then
			mkdir /userdata/hw_rockchip_qt
		fi
		cp /oem/hw_rockchip_qt/qtkms.json /userdata/hw_rockchip_qt/qtkms.json
		sync
	fi

	./${QTFILE} > /dev/null 2>&1 &

	PID_QT=$(get_pid "${QTFILE}")
	logger -s "`date +%Y/%m/%d-%H:%m:%S:%N`: Process ${QTFILE}(pid:$PID_QT) restart ok."
	#echo "`date +%Y/%m/%d-%H:%m:%S:%N`: Process ${QTFILE}(pid:$PID_QT) restart ok." >> /tmp/qt.log 

	echo $PID_QT
	return 0
}

check_staticroute()
{
	IFace=`cat /etc/dhcpcd.conf | grep "interface eth0"`
	[ -z "${IFace}" ] && return 0

	#IP=`cat /etc/dhcpcd.conf | grep "static ip_address=" | cut -d "=" -f 2`
	#[ -z "${IP}" ] && return 0

	GW=`cat /etc/dhcpcd.conf | grep "static routers=" | cut -d "=" -f 2`
	[ -z "${GW}" ] && return 0

	DF=`ip route | awk '/^default/{print $3}' | head -n 1`
	[ x"${DF}" = x"${GW}" ] && return 0

	logger -s "static route wrong, set default gw !!!"
	route add default gw ${GW}
	#echo `date` >> /tmp/route.log

	return 0
}

# free cache
#cd /oem/hw_rockchip_qt; nohup ./free_cache_buffer.sh > /dev/null &
echo 16384 > /proc/sys/vm/min_free_kbytes

####
if [ ! -d ${EXECPATH} ]; then
	logger -s "${EXECPATH} missing !"

	exit 1
fi

#cd ${EXECPATH}
if [ ! -x ${EXECPATH}/${QTFILE} ]; then
	logger -s "${QTFILE} missing !"

	exit 2
fi

#声明QT运行环境
export QT_QPA_FB_DRM=1
export QT_QPA_PLATFORM=linuxfb:rotation=0
export QT_QPA_EGLFS_KMS_CONNECTOR_INDEX=1
export QT_QPA_EGLFS_PHYSICAL_WIDTH=1920
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=1080
export QT_QPA_EGLFS_KMS_CONFIG=/userdata/hw_rockchip_qt/qtkms.json
export XDG_RUNTIME_DIR=/tmp
export RUNLEVEL=3


PID_QT=$(run_qt)

# 无限循环（0<1）
while [ 0 -lt 1 ]
do
	PID_QT=$(get_pid "${QTFILE}")
	if [ "$PID_QT" == "" ]; then
		PID_QT=$(run_qt)
	fi

	check_staticroute

	sleep $sleepTime > /dev/null 2>&1
done

exit 255

