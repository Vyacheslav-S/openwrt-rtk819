# -*- coding: utf-8 -*-
# TNC tests
# Copyright (c) 2014, Jouni Malinen <j@w1.fi>
#
# This software may be distributed under the terms of the BSD license.
# See README for more details.

import os.path

import hostapd
from test_ap_eap import int_eap_server_params

def test_tnc_peap_soh(dev, apdev):
    """TNC PEAP-SoH"""
    params = int_eap_server_params()
    params["tnc"] = "1"
    hostapd.add_ap(apdev[0]['ifname'], params)

    dev[0].connect("test-wpa2-eap", key_mgmt="WPA-EAP",
                   eap="PEAP", identity="user", password="password",
                   ca_cert="auth_serv/ca.pem",
                   phase1="peapver=0 tnc=soh cryptobinding=0",
                   phase2="auth=MSCHAPV2",
                   wait_connect=False)
    ev = dev[0].wait_event(["CTRL-EVENT-CONNECTED"], timeout=10)
    if ev is None:
        raise Exception("Connection timed out")

    dev[1].connect("test-wpa2-eap", key_mgmt="WPA-EAP",
                   eap="PEAP", identity="user", password="password",
                   ca_cert="auth_serv/ca.pem",
                   phase1="peapver=0 tnc=soh1 cryptobinding=1",
                   phase2="auth=MSCHAPV2",
                   wait_connect=False)
    ev = dev[1].wait_event(["CTRL-EVENT-CONNECTED"], timeout=10)
    if ev is None:
        raise Exception("Connection timed out")

    dev[2].connect("test-wpa2-eap", key_mgmt="WPA-EAP",
                   eap="PEAP", identity="user", password="password",
                   ca_cert="auth_serv/ca.pem",
                   phase1="peapver=0 tnc=soh2 cryptobinding=2",
                   phase2="auth=MSCHAPV2",
                   wait_connect=False)
    ev = dev[2].wait_event(["CTRL-EVENT-CONNECTED"], timeout=10)
    if ev is None:
        raise Exception("Connection timed out")

def test_tnc_ttls(dev, apdev):
    """TNC TTLS"""
    params = int_eap_server_params()
    params["tnc"] = "1"
    hostapd.add_ap(apdev[0]['ifname'], params)

    if not os.path.exists("tnc/libhostap_imc.so"):
        logger.info("No IMC installed - skip")
        return "skip"

    dev[0].connect("test-wpa2-eap", key_mgmt="WPA-EAP",
                   eap="TTLS", identity="DOMAIN\mschapv2 user",
                   anonymous_identity="ttls", password="password",
                   phase2="auth=MSCHAPV2",
                   ca_cert="auth_serv/ca.pem",
                   wait_connect=False)
    ev = dev[0].wait_event(["CTRL-EVENT-CONNECTED"], timeout=10)
    if ev is None:
        raise Exception("Connection timed out")

def test_tnc_fast(dev, apdev):
    """TNC FAST"""
    params = int_eap_server_params()
    params["tnc"] = "1"
    params["pac_opaque_encr_key"] ="000102030405060708090a0b0c0d0e00"
    params["eap_fast_a_id"] = "101112131415161718191a1b1c1d1e00"
    params["eap_fast_a_id_info"] = "test server2"

    hostapd.add_ap(apdev[0]['ifname'], params)

    if not os.path.exists("tnc/libhostap_imc.so"):
        logger.info("No IMC installed - skip")
        return "skip"

    dev[0].connect("test-wpa2-eap", key_mgmt="WPA-EAP",
                   eap="FAST", identity="user",
                   anonymous_identity="FAST", password="password",
                   phase2="auth=GTC",
                   phase1="fast_provisioning=2",
                   pac_file="blob://fast_pac_auth_tnc",
                   ca_cert="auth_serv/ca.pem",
                   wait_connect=False)
    ev = dev[0].wait_event(["CTRL-EVENT-CONNECTED"], timeout=10)
    if ev is None:
        raise Exception("Connection timed out")
