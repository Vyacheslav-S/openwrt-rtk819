# IBSS test cases
# Copyright (c) 2013, Jouni Malinen <j@w1.fi>
#
# This software may be distributed under the terms of the BSD license.
# See README for more details.

import logging
logger = logging.getLogger()
import time
import re

import hwsim_utils

def connect_ibss_cmd(dev, id):
    dev.dump_monitor()
    dev.select_network(id, freq="2412")

def wait_ibss_connection(dev):
    logger.info(dev.ifname + " waiting for IBSS start/join to complete")
    ev = dev.wait_event(["CTRL-EVENT-CONNECTED"], timeout=20)
    if ev is None:
        raise Exception("Connection to the IBSS timed out")
    exp = r'<.>(CTRL-EVENT-CONNECTED) - Connection to ([0-9a-f:]*) completed.*'
    s = re.split(exp, ev)
    if len(s) < 3:
        return None
    return s[2]

def wait_4way_handshake(dev1, dev2):
    logger.info(dev1.ifname + " waiting for 4-way handshake completion with " + dev2.ifname + " " + dev2.p2p_interface_addr())
    ev = dev1.wait_event(["IBSS-RSN-COMPLETED " + dev2.p2p_interface_addr()],
                         timeout=20)
    if ev is None:
        raise Exception("4-way handshake in IBSS timed out")

def wait_4way_handshake2(dev1, dev2, dev3):
    logger.info(dev1.ifname + " waiting for 4-way handshake completion with " + dev2.ifname + " " + dev2.p2p_interface_addr() + " and " + dev3.p2p_interface_addr())
    ev = dev1.wait_event(["IBSS-RSN-COMPLETED " + dev2.p2p_interface_addr(),
                          "IBSS-RSN-COMPLETED " + dev3.p2p_interface_addr()],
                         timeout=20)
    if ev is None:
        raise Exception("4-way handshake in IBSS timed out")
    ev = dev1.wait_event(["IBSS-RSN-COMPLETED " + dev2.p2p_interface_addr(),
                          "IBSS-RSN-COMPLETED " + dev3.p2p_interface_addr()],
                         timeout=20)
    if ev is None:
        raise Exception("4-way handshake in IBSS timed out")

def add_ibss(dev, ssid, psk=None, proto=None, key_mgmt=None, pairwise=None, group=None, beacon_int=None, bssid=None):
    id = dev.add_network()
    dev.set_network(id, "mode", "1")
    dev.set_network(id, "frequency", "2412")
    dev.set_network_quoted(id, "ssid", ssid)
    if psk:
        dev.set_network_quoted(id, "psk", psk)
    if proto:
        dev.set_network(id, "proto", proto)
    if key_mgmt:
        dev.set_network(id, "key_mgmt", key_mgmt)
    if pairwise:
        dev.set_network(id, "pairwise", pairwise)
    if group:
        dev.set_network(id, "group", group)
    if beacon_int:
        dev.set_network(id, "beacon_int", beacon_int)
    if bssid:
        dev.set_network(id, "bssid", bssid)
    dev.request("ENABLE_NETWORK " + str(id) + " no-connect")
    return id

def add_ibss_rsn(dev, ssid):
    return add_ibss(dev, ssid, "12345678", "RSN", "WPA-PSK", "CCMP", "CCMP")

def add_ibss_wpa_none(dev, ssid):
    return add_ibss(dev, ssid, "12345678", "WPA", "WPA-NONE", "TKIP", "TKIP")

def add_ibss_wpa_none_ccmp(dev, ssid):
    return add_ibss(dev, ssid, "12345678", "WPA", "WPA-NONE", "CCMP", "CCMP")

def test_ibss_rsn(dev):
    """IBSS RSN"""
    ssid="ibss-rsn"

    logger.info("Start IBSS on the first STA")
    id = add_ibss_rsn(dev[0], ssid)
    connect_ibss_cmd(dev[0], id)
    bssid0 = wait_ibss_connection(dev[0])

    logger.info("Join two STAs to the IBSS")

    id = add_ibss_rsn(dev[1], ssid)
    connect_ibss_cmd(dev[1], id)
    bssid1 = wait_ibss_connection(dev[1])
    if bssid0 != bssid1:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA1 BSSID " + bssid1)
        # try to merge with a scan
        dev[1].scan()
    wait_4way_handshake(dev[0], dev[1])
    wait_4way_handshake(dev[1], dev[0])

    id = add_ibss_rsn(dev[2], ssid)
    connect_ibss_cmd(dev[2], id)
    bssid2 = wait_ibss_connection(dev[2])
    if bssid0 != bssid2:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA2 BSSID " + bssid2)
        # try to merge with a scan
        dev[2].scan()
    wait_4way_handshake(dev[0], dev[2])
    wait_4way_handshake2(dev[2], dev[0], dev[1])

    # Allow some time for all peers to complete key setup
    time.sleep(3)
    hwsim_utils.test_connectivity(dev[0].ifname, dev[1].ifname)
    hwsim_utils.test_connectivity(dev[0].ifname, dev[2].ifname)
    hwsim_utils.test_connectivity(dev[1].ifname, dev[2].ifname)

    dev[1].request("REMOVE_NETWORK all")
    time.sleep(1)
    id = add_ibss_rsn(dev[1], ssid)
    connect_ibss_cmd(dev[1], id)
    bssid1 = wait_ibss_connection(dev[1])
    if bssid0 != bssid1:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA1 BSSID " + bssid1)
        # try to merge with a scan
        dev[1].scan()
    wait_4way_handshake(dev[0], dev[1])
    wait_4way_handshake(dev[1], dev[0])
    time.sleep(3)
    hwsim_utils.test_connectivity(dev[0].ifname, dev[1].ifname)

def test_ibss_wpa_none(dev):
    """IBSS WPA-None"""
    ssid="ibss-wpa-none"

    logger.info("Start IBSS on the first STA")
    id = add_ibss_wpa_none(dev[0], ssid)
    connect_ibss_cmd(dev[0], id)
    bssid0 = wait_ibss_connection(dev[0])

    # This is a bit ugly, but no one really cares about WPA-None, so there may
    # not be enough justification to clean this up.. For now, wpa_supplicant
    # will show two connection events with mac80211_hwsim where the first one
    # comes with all zeros address.
    if bssid0 == "00:00:00:00:00:00":
        logger.info("Waiting for real BSSID on the first STA")
        bssid0 = wait_ibss_connection(dev[0])

    logger.info("Join two STAs to the IBSS")

    id = add_ibss_wpa_none(dev[1], ssid)
    connect_ibss_cmd(dev[1], id)
    id = add_ibss_wpa_none(dev[2], ssid)
    connect_ibss_cmd(dev[2], id)

    bssid1 = wait_ibss_connection(dev[1])
    if bssid0 != bssid1:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA1 BSSID " + bssid1)
        bssid1 = wait_ibss_connection(dev[1])

    bssid2 = wait_ibss_connection(dev[2])
    if bssid0 != bssid2:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA2 BSSID " + bssid2)
        bssid2 = wait_ibss_connection(dev[2])

    print bssid0
    print bssid1
    print bssid2

    # Allow some time for all peers to complete key setup
    time.sleep(1)

    # This is supposed to work, but looks like WPA-None does not work with
    # mac80211 currently..
    try:
        hwsim_utils.test_connectivity(dev[0].ifname, dev[1].ifname)
    except Exception, e:
        logger.info("Ignoring known connectivity failure: " + str(e))
    try:
        hwsim_utils.test_connectivity(dev[0].ifname, dev[2].ifname)
    except Exception, e:
        logger.info("Ignoring known connectivity failure: " + str(e))
    try:
        hwsim_utils.test_connectivity(dev[1].ifname, dev[2].ifname)
    except Exception, e:
        logger.info("Ignoring known connectivity failure: " + str(e))

def test_ibss_wpa_none_ccmp(dev):
    """IBSS WPA-None/CCMP"""
    ssid="ibss-wpa-none"

    logger.info("Start IBSS on the first STA")
    id = add_ibss_wpa_none(dev[0], ssid)
    connect_ibss_cmd(dev[0], id)
    bssid0 = wait_ibss_connection(dev[0])

    # This is a bit ugly, but no one really cares about WPA-None, so there may
    # not be enough justification to clean this up.. For now, wpa_supplicant
    # will show two connection events with mac80211_hwsim where the first one
    # comes with all zeros address.
    if bssid0 == "00:00:00:00:00:00":
        logger.info("Waiting for real BSSID on the first STA")
        bssid0 = wait_ibss_connection(dev[0])


    logger.info("Join a STA to the IBSS")
    id = add_ibss_wpa_none(dev[1], ssid)
    connect_ibss_cmd(dev[1], id)

    bssid1 = wait_ibss_connection(dev[1])
    if bssid0 != bssid1:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA1 BSSID " + bssid1)
        bssid1 = wait_ibss_connection(dev[1])

    print bssid0
    print bssid1

    # Allow some time for all peers to complete key setup
    time.sleep(1)

    # This is supposed to work, but looks like WPA-None does not work with
    # mac80211 currently..
    try:
        hwsim_utils.test_connectivity(dev[0].ifname, dev[1].ifname)
    except Exception, e:
        logger.info("Ignoring known connectivity failure: " + str(e))

def test_ibss_open(dev):
    """IBSS open (no security)"""
    ssid="ibss"
    id = add_ibss(dev[0], ssid, key_mgmt="NONE", beacon_int="150")
    connect_ibss_cmd(dev[0], id)
    bssid0 = wait_ibss_connection(dev[0])

    id = add_ibss(dev[1], ssid, key_mgmt="NONE", beacon_int="200")
    connect_ibss_cmd(dev[1], id)
    bssid1 = wait_ibss_connection(dev[1])
    if bssid0 != bssid1:
        logger.info("STA0 BSSID " + bssid0 + " differs from STA1 BSSID " + bssid1)

def test_ibss_open_fixed_bssid(dev):
    """IBSS open (no security) and fixed BSSID"""
    ssid="ibss"
    bssid="02:11:22:33:44:55"
    try:
        dev[0].request("AP_SCAN 2")
        add_ibss(dev[0], ssid, key_mgmt="NONE", bssid=bssid, beacon_int="150")
        dev[0].request("REASSOCIATE")

        dev[1].request("AP_SCAN 2")
        add_ibss(dev[1], ssid, key_mgmt="NONE", bssid=bssid, beacon_int="200")
        dev[1].request("REASSOCIATE")

        bssid0 = wait_ibss_connection(dev[0])
        bssid1 = wait_ibss_connection(dev[1])
        if bssid0 != bssid:
            raise Exception("STA0 BSSID " + bssid0 + " differs from fixed BSSID " + bssid)
        if bssid1 != bssid:
            raise Exception("STA0 BSSID " + bssid0 + " differs from fixed BSSID " + bssid)
    finally:
        dev[0].request("AP_SCAN 1")
        dev[1].request("AP_SCAN 1")
