#!/bin/sh

DIR="$( cd "$( dirname "$0" )" && pwd )"
WPAS=$DIR/../../wpa_supplicant/wpa_supplicant
WPACLI=$DIR/../../wpa_supplicant/wpa_cli
HAPD=$DIR/../../hostapd/hostapd
HAPD_AS=$DIR/../../hostapd/hostapd
WLANTEST=$DIR/../../wlantest/wlantest
HLR_AUC_GW=$DIR/../../hostapd/hlr_auc_gw
DATE="$(date +%s)"

if [ -z "$LOGDIR" ] ; then
    LOGDIR="$DIR/logs/$DATE"
    mkdir -p $LOGDIR
else
    if [ -e $LOGDIR/alt-wpa_supplicant/wpa_supplicant/wpa_supplicant ]; then
	WPAS=$LOGDIR/alt-wpa_supplicant/wpa_supplicant/wpa_supplicant
    fi
    if [ -e $LOGDIR/alt-hostapd/hostapd/hostapd ]; then
	HAPD=$LOGDIR/alt-hostapd/hostapd/hostapd
    fi
    if [ -e $LOGDIR/alt-hostapd-as/hostapd/hostapd ]; then
	HAPD_AS=$LOGDIR/alt-hostapd-as/hostapd/hostapd
    fi
    if [ -e $LOGDIR/alt-hlr_auc_gw/hostapd/hlr_auc_gw ]; then
	HLR_AUC_GW=$LOGDIR/alt-hlr_auc_gw/hostapd/hlr_auc_gw
    fi
fi

if test -w "$DIR/logs" ; then
    rm -rf $DIR/logs/current
    ln -sf $DATE $DIR/logs/current
fi

if groups | tr ' ' "\n" | grep -q ^admin$; then
    GROUP=admin
else
    GROUP=adm
fi

for i in 0 1 2; do
    sed "s/ GROUP=.*$/ GROUP=$GROUP/" "$DIR/p2p$i.conf" > "$LOGDIR/p2p$i.conf"
done

sed "s/group=admin/group=$GROUP/" "$DIR/auth_serv/as.conf" > "$LOGDIR/as.conf"
sed "s/group=admin/group=$GROUP/;s%LOGDIR%$LOGDIR%" "$DIR/auth_serv/as2.conf" > "$LOGDIR/as2.conf"

if [ "$1" = "valgrind" ]; then
    VALGRIND=y
    VALGRIND_WPAS="valgrind --log-file=$LOGDIR/valgrind-wlan%d"
    VALGRIND_HAPD="valgrind --log-file=$LOGDIR/valgrind-hostapd"
    chmod -f a+rx $WPAS
    chmod -f a+rx $HAPD
    chmod -f a+rx $HAPD_AS
    HAPD_AS="valgrind --log-file=$LOGDIR/valgrind-auth-serv $HAPD_AS"
    shift
else
    unset VALGRIND
    VALGRIND_WPAS=
    VALGRIND_HAPD=
fi

if [ "$1" = "trace" ]; then
    TRACE="T"
    shift
else
    TRACE=""
fi

$DIR/stop.sh
test -f /proc/modules && sudo modprobe mac80211_hwsim radios=6
sudo ifconfig hwsim0 up
sudo $WLANTEST -i hwsim0 -n $LOGDIR/hwsim0.pcapng -c -dt -L $LOGDIR/hwsim0 &
for i in 0 1 2; do
    sudo $(printf -- "$VALGRIND_WPAS" $i) $WPAS -g /tmp/wpas-wlan$i -G$GROUP -Dnl80211 -iwlan$i -c $LOGDIR/p2p$i.conf \
         -ddKt$TRACE -f $LOGDIR/log$i &
done
sudo $(printf -- "$VALGRIND_WPAS" 5) $WPAS -g /tmp/wpas-wlan5 -G$GROUP \
    -ddKt$TRACE -f $LOGDIR/log5 &
sudo $VALGRIND_HAPD $HAPD -ddKt$TRACE -g /var/run/hostapd-global -G $GROUP -ddKt -f $LOGDIR/hostapd &

sleep 1
sudo chown -f $USER $LOGDIR/hwsim0.pcapng $LOGDIR/hwsim0 $LOGDIR/log* $LOGDIR/hostapd

if [ -x $HLR_AUC_GW ]; then
    cp $DIR/auth_serv/hlr_auc_gw.milenage_db $LOGDIR/hlr_auc_gw.milenage_db
    sudo $HLR_AUC_GW -u -m $LOGDIR/hlr_auc_gw.milenage_db -g $DIR/auth_serv/hlr_auc_gw.gsm > $LOGDIR/hlr_auc_gw &
fi

touch $LOGDIR/hostapd.db
sudo $HAPD_AS -ddKt $LOGDIR/as.conf $LOGDIR/as2.conf > $LOGDIR/auth_serv &
if [ "x$VALGRIND" = "xy" ]; then
    sleep 1
    sudo chown -f $USER $LOGDIR/*valgrind*
fi

# wait for programs to be fully initialized
for i in 0 1 2; do
    for j in `seq 1 10`; do
	if $WPACLI -g /tmp/wpas-wlan$i ping | grep -q PONG; then
	    break
	fi
	if [ $j = "10" ]; then
	    echo "Could not connect to /tmp/wpas-wlan$i"
	    exit 1
	fi
	sleep 1
    done
done

for j in `seq 1 10`; do
    if $WPACLI -g /var/run/hostapd-global ping | grep -q PONG; then
	break
    fi
    if [ $j = "10" ]; then
	echo "Could not connect to /var/run/hostapd-global"
	exit 1
    fi
    sleep 1
done

exit 0
