# P2P+NFC tests
# Copyright (c) 2013, Qualcomm Atheros, Inc.
#
# This software may be distributed under the terms of the BSD license.
# See README for more details.

import time
import subprocess
import logging
logger = logging.getLogger(__name__)

import hwsim_utils

grpform_events = ["P2P-GROUP-STARTED",
                  "P2P-GO-NEG-FAILURE",
                  "P2P-GROUP-FORMATION-FAILURE",
                  "WPS-PIN-NEEDED",
                  "WPS-M2D",
                  "WPS-FAIL"]

def set_ip_addr_info(dev):
    dev.request("SET ip_addr_go 192.168.42.1")
    dev.request("SET ip_addr_mask 255.255.255.0")
    dev.request("SET ip_addr_start 192.168.42.100")
    dev.request("SET ip_addr_end 192.168.42.199")

def check_ip_addr(res):
    if 'ip_addr' not in res:
        raise Exception("Did not receive IP address from GO")
    if '192.168.42.' not in res['ip_addr']:
        raise Exception("Unexpected IP address received from GO")
    if 'ip_mask' not in res:
        raise Exception("Did not receive IP address mask from GO")
    if '255.255.255.' not in res['ip_mask']:
        raise Exception("Unexpected IP address mask received from GO")
    if 'go_ip_addr' not in res:
        raise Exception("Did not receive GO IP address from GO")
    if '192.168.42.' not in res['go_ip_addr']:
        raise Exception("Unexpected GO IP address received from GO")

def test_nfc_p2p_go_neg(dev):
    """NFC connection handover to form a new P2P group (initiator becomes GO)"""
    set_ip_addr_info(dev[0])
    dev[0].request("SET p2p_go_intent 10")
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[0].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res1['role'] != 'client' or res0['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res1)

def test_nfc_p2p_go_neg_reverse(dev):
    """NFC connection handover to form a new P2P group (responder becomes GO)"""
    set_ip_addr_info(dev[1])
    dev[0].request("SET p2p_go_intent 3")
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[0].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res0['role'] != 'client' or res1['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res0)

def test_nfc_p2p_initiator_go(dev):
    """NFC connection handover with initiator already GO"""
    set_ip_addr_info(dev[0])
    logger.info("Start autonomous GO")
    dev[0].p2p_start_go()
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[1].wait_event(["P2P-GROUP-STARTED"], timeout=15)
    if ev is None:
        raise Exception("Connection to the group timed out")
    res1 = dev[1].group_form_result(ev)
    if res1['result'] != 'success':
        raise Exception("Unexpected connection failure")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res1)

def test_nfc_p2p_responder_go(dev):
    """NFC connection handover with responder already GO"""
    set_ip_addr_info(dev[1])
    logger.info("Start autonomous GO")
    dev[1].p2p_start_go()
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[0].wait_event(["P2P-GROUP-STARTED"], timeout=15)
    if ev is None:
        raise Exception("Connection to the group timed out")
    res0 = dev[0].group_form_result(ev)
    if res0['result'] != 'success':
        raise Exception("Unexpected connection failure")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res0)

def test_nfc_p2p_both_go(dev):
    """NFC connection handover with both devices already GOs"""
    set_ip_addr_info(dev[0])
    set_ip_addr_info(dev[1])
    logger.info("Start autonomous GOs")
    dev[0].p2p_start_go()
    dev[1].p2p_start_go()
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[0].wait_event(["P2P-NFC-BOTH-GO"], timeout=15)
    if ev is None:
        raise Exception("Time out waiting for P2P-NFC-BOTH-GO (dev0)")
    ev = dev[1].wait_event(["P2P-NFC-BOTH-GO"], timeout=1)
    if ev is None:
        raise Exception("Time out waiting for P2P-NFC-BOTH-GO (dev1)")
    dev[0].remove_group()
    dev[1].remove_group()

def test_nfc_p2p_client(dev):
    """NFC connection handover when one device is P2P client"""
    logger.info("Start autonomous GOs")
    dev[0].p2p_start_go()
    logger.info("Connect one device as a P2P client")
    pin = dev[1].wps_read_pin()
    dev[0].p2p_go_authorize_client(pin)
    dev[1].p2p_connect_group(dev[0].p2p_dev_addr(), pin, timeout=60)
    logger.info("Client connected")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])

    logger.info("NFC connection handover between P2P client and P2P device")
    req = dev[1].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[2].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[1].dump_monitor()
    dev[2].dump_monitor()
    res = dev[2].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[1].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[1].wait_event(["P2P-NFC-WHILE-CLIENT"], timeout=15)
    if ev is None:
        raise Exception("Time out waiting for P2P-NFC-WHILE-CLIENT")
    ev = dev[2].wait_event(["P2P-NFC-PEER-CLIENT"], timeout=1)
    if ev is None:
        raise Exception("Time out waiting for P2P-NFC-PEER-CLIENT")

    logger.info("Connect to group based on upper layer trigger")
    pin = dev[2].wps_read_pin()
    dev[0].p2p_go_authorize_client(pin)
    dev[2].p2p_connect_group(dev[0].p2p_dev_addr(), pin, timeout=60)
    logger.info("Client connected")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    hwsim_utils.test_connectivity_p2p(dev[1], dev[2])
    dev[2].remove_group()
    dev[1].remove_group()
    dev[0].remove_group()

def test_nfc_p2p_static_handover_tagdev_client(dev):
    """NFC static handover to form a new P2P group (NFC Tag device becomes P2P Client)"""

    set_ip_addr_info(dev[0])

    logger.info("Perform NFC connection handover")

    res = dev[1].request("SET p2p_listen_reg_class 81")
    res2 = dev[1].request("SET p2p_listen_channel 6")
    if "FAIL" in res or "FAIL" in res2:
        raise Exception("Could not set Listen channel")
    pw = dev[1].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    res = dev[1].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    res = dev[1].request("P2P_LISTEN")
    if "FAIL" in res:
        raise Exception("Failed to start Listen mode")
    dev[1].dump_monitor()

    dev[0].dump_monitor()
    dev[0].request("SET p2p_go_intent 10")
    res = dev[0].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[0].wait_event(grpform_events, timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(grpform_events, timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res1['role'] != 'client' or res0['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res1)

def test_nfc_p2p_static_handover_tagdev_client_group_iface(dev):
    """NFC static handover to form a new P2P group (NFC Tag device becomes P2P Client with group iface)"""

    set_ip_addr_info(dev[0])

    logger.info("Perform NFC connection handover")

    res = dev[1].request("SET p2p_listen_reg_class 81")
    res2 = dev[1].request("SET p2p_listen_channel 6")
    if "FAIL" in res or "FAIL" in res2:
        raise Exception("Could not set Listen channel")
    pw = dev[1].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    dev[1].request("SET p2p_no_group_iface 0")
    res = dev[1].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    res = dev[1].request("P2P_LISTEN")
    if "FAIL" in res:
        raise Exception("Failed to start Listen mode")
    dev[1].dump_monitor()

    dev[0].dump_monitor()
    dev[0].request("SET p2p_go_intent 10")
    res = dev[0].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[0].wait_event(grpform_events, timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(grpform_events, timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res1['role'] != 'client' or res0['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res1)

def test_nfc_p2p_static_handover_tagdev_go(dev):
    """NFC static handover to form a new P2P group (NFC Tag device becomes GO)"""

    set_ip_addr_info(dev[1])

    logger.info("Perform NFC connection handover")

    res = dev[1].request("SET p2p_listen_reg_class 81")
    res2 = dev[1].request("SET p2p_listen_channel 6")
    if "FAIL" in res or "FAIL" in res2:
        raise Exception("Could not set Listen channel")
    pw = dev[1].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    res = dev[1].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    res = dev[1].request("P2P_LISTEN")
    if "FAIL" in res:
        raise Exception("Failed to start Listen mode")
    dev[1].dump_monitor()

    dev[0].dump_monitor()
    dev[0].request("SET p2p_go_intent 3")
    res = dev[0].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[0].wait_event(grpform_events, timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(grpform_events, timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res0['role'] != 'client' or res1['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res0)

def test_nfc_p2p_static_handover_tagdev_go_forced_freq(dev):
    """NFC static handover to form a new P2P group on forced channel (NFC Tag device becomes GO)"""

    set_ip_addr_info(dev[1])

    logger.info("Perform NFC connection handover")

    res = dev[1].request("SET p2p_listen_reg_class 81")
    res2 = dev[1].request("SET p2p_listen_channel 6")
    if "FAIL" in res or "FAIL" in res2:
        raise Exception("Could not set Listen channel")
    pw = dev[1].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    res = dev[1].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    res = dev[1].request("P2P_LISTEN")
    if "FAIL" in res:
        raise Exception("Failed to start Listen mode")
    dev[1].dump_monitor()

    dev[0].dump_monitor()
    dev[0].request("SET p2p_go_intent 3")
    res = dev[0].request("WPS_NFC_TAG_READ " + sel + " freq=2442")
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[0].wait_event(grpform_events, timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(grpform_events, timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res0['role'] != 'client' or res1['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res0)

def test_nfc_p2p_static_handover_join_tagdev_go(dev):
    """NFC static handover to join a P2P group (NFC Tag device is the GO)"""

    logger.info("Start autonomous GO")
    set_ip_addr_info(dev[0])
    dev[0].p2p_start_go()

    logger.info("Write NFC Tag on the GO")
    pw = dev[0].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    res = dev[0].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[0].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")

    logger.info("Read NFC Tag on a P2P Device to join a group")
    res = dev[1].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[1].wait_event(grpform_events, timeout=30)
    if ev is None:
        raise Exception("Joining the group timed out")
    res = dev[1].group_form_result(ev)
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res)

    logger.info("Read NFC Tag on another P2P Device to join a group")
    res = dev[2].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[2].wait_event(grpform_events, timeout=30)
    if ev is None:
        raise Exception("Joining the group timed out")
    res = dev[2].group_form_result(ev)
    hwsim_utils.test_connectivity_p2p(dev[0], dev[2])
    check_ip_addr(res)

def test_nfc_p2p_static_handover_join_tagdev_client(dev):
    """NFC static handover to join a P2P group (NFC Tag device is the P2P Client)"""
    set_ip_addr_info(dev[0])
    logger.info("Start autonomous GO")
    dev[0].p2p_start_go()

    dev[1].request("SET ignore_old_scan_res 1")
    dev[2].request("SET ignore_old_scan_res 1")

    logger.info("Write NFC Tag on the P2P Client")
    res = dev[1].request("P2P_LISTEN")
    if "FAIL" in res:
        raise Exception("Failed to start Listen mode")
    pw = dev[1].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    res = dev[1].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")

    logger.info("Read NFC Tag on the GO to trigger invitation")
    res = dev[0].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[1].wait_event(grpform_events, timeout=30)
    if ev is None:
        raise Exception("Joining the group timed out")
    res = dev[1].group_form_result(ev)
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res)

    logger.info("Write NFC Tag on another P2P Client")
    res = dev[2].request("P2P_LISTEN")
    if "FAIL" in res:
        raise Exception("Failed to start Listen mode")
    pw = dev[2].request("WPS_NFC_TOKEN NDEF").rstrip()
    if "FAIL" in pw:
        raise Exception("Failed to generate password token")
    res = dev[2].request("P2P_SET nfc_tag 1").rstrip()
    if "FAIL" in res:
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    sel = dev[2].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR-TAG").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")

    logger.info("Read NFC Tag on the GO to trigger invitation")
    res = dev[0].request("WPS_NFC_TAG_READ " + sel)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")

    ev = dev[2].wait_event(grpform_events, timeout=30)
    if ev is None:
        raise Exception("Joining the group timed out")
    res = dev[2].group_form_result(ev)
    hwsim_utils.test_connectivity_p2p(dev[0], dev[2])
    check_ip_addr(res)

def test_nfc_p2p_go_legacy_config_token(dev):
    logger.info("Start autonomous GOs")
    dev[0].p2p_start_go()
    logger.info("Connect legacy WPS STA with configuration token")
    conf = dev[0].request("WPS_NFC_CONFIG_TOKEN NDEF").rstrip()
    if "FAIL" in conf:
        raise Exception("Failed to generate configuration token")
    dev[1].dump_monitor()
    res = dev[1].request("WPS_NFC_TAG_READ " + conf)
    if "FAIL" in res:
        raise Exception("Failed to provide NFC tag contents to wpa_supplicant")
    ev = dev[1].wait_event(["CTRL-EVENT-CONNECTED"], timeout=15)
    if ev is None:
        raise Exception("Joining the group timed out")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    dev[1].request("DISCONNECT")
    dev[0].remove_group()

def test_nfc_p2p_go_legacy_handover(dev):
    logger.info("Start autonomous GOs")
    dev[0].p2p_start_go()
    logger.info("Connect legacy WPS STA with connection handover")
    req = dev[1].request("NFC_GET_HANDOVER_REQ NDEF WPS-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[0].request("NFC_GET_HANDOVER_SEL NDEF WPS-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    res = dev[0].request("NFC_REPORT_HANDOVER RESP WPS " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant (GO)")
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER INIT WPS " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant (legacy STA)")
    ev = dev[1].wait_event(["CTRL-EVENT-CONNECTED"], timeout=15)
    if ev is None:
        raise Exception("Joining the group timed out")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    dev[1].request("DISCONNECT")
    dev[0].remove_group()

def test_nfc_p2p_ip_addr_assignment(dev):
    """NFC connection handover and legacy station IP address assignment"""
    set_ip_addr_info(dev[1])
    dev[0].request("SET p2p_go_intent 3")
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[0].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res0['role'] != 'client' or res1['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res0)

    logger.info("Connect legacy P2P client that does not use new IP address assignment")
    res = dev[2].request("P2P_SET disable_ip_addr_req 1")
    if "FAIL" in res:
        raise Exception("Failed to disable IP address assignment request")
    pin = dev[2].wps_read_pin()
    dev[1].p2p_go_authorize_client(pin)
    res = dev[2].p2p_connect_group(dev[1].p2p_dev_addr(), pin, timeout=60)
    logger.info("Client connected")
    hwsim_utils.test_connectivity_p2p(dev[1], dev[2])
    if 'ip_addr' in res:
        raise Exception("Unexpected IP address assignment")

def test_nfc_p2p_ip_addr_assignment2(dev):
    """NFC connection handover and IP address assignment for two clients"""
    set_ip_addr_info(dev[1])
    dev[0].request("SET p2p_go_intent 3")
    logger.info("Perform NFC connection handover")
    req = dev[0].request("NFC_GET_HANDOVER_REQ NDEF P2P-CR").rstrip()
    if "FAIL" in req:
        raise Exception("Failed to generate NFC connection handover request")
    sel = dev[1].request("NFC_GET_HANDOVER_SEL NDEF P2P-CR").rstrip()
    if "FAIL" in sel:
        raise Exception("Failed to generate NFC connection handover select")
    dev[0].dump_monitor()
    dev[1].dump_monitor()
    res = dev[1].request("NFC_REPORT_HANDOVER RESP P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(resp)")
    res = dev[0].request("NFC_REPORT_HANDOVER INIT P2P " + req + " " + sel)
    if "FAIL" in res:
        raise Exception("Failed to report NFC connection handover to wpa_supplicant(init)")

    ev = dev[0].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=15)
    if ev is None:
        raise Exception("Group formation timed out")
    res0 = dev[0].group_form_result(ev)

    ev = dev[1].wait_event(["P2P-GROUP-STARTED",
                            "P2P-GO-NEG-FAILURE",
                            "P2P-GROUP-FORMATION-FAILURE",
                            "WPS-PIN-NEEDED"], timeout=1)
    if ev is None:
        raise Exception("Group formation timed out")
    res1 = dev[1].group_form_result(ev)
    logger.info("Group formed")

    if res0['role'] != 'client' or res1['role'] != 'GO':
        raise Exception("Unexpected roles negotiated")
    hwsim_utils.test_connectivity_p2p(dev[0], dev[1])
    check_ip_addr(res0)
    print "Client 1 IP address: " + res0['ip_addr']

    logger.info("Connect a P2P client")
    pin = dev[2].wps_read_pin()
    dev[1].p2p_go_authorize_client(pin)
    res = dev[2].p2p_connect_group(dev[1].p2p_dev_addr(), pin, timeout=60)
    logger.info("Client connected")
    hwsim_utils.test_connectivity_p2p(dev[1], dev[2])
    check_ip_addr(res)
    print "Client 2 IP address: " + res['ip_addr']
    if res['ip_addr'] == res0['ip_addr']:
        raise Exception("Same IP address assigned to both clients")

def test_nfc_p2p_tag_enable_disable(dev):
    """NFC tag enable/disable for P2P"""
    if "FAIL" in dev[0].request("WPS_NFC_TOKEN NDEF").rstrip():
        raise Exception("Failed to generate password token")
    if "OK" not in dev[0].request("P2P_SET nfc_tag 1"):
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    if "OK" not in dev[0].request("P2P_SET nfc_tag 0"):
        raise Exception("Failed to disable NFC Tag for P2P static handover")

    dev[0].request("SET p2p_no_group_iface 0")
    if "OK" not in dev[0].request("P2P_SET nfc_tag 1"):
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    if "OK" not in dev[0].request("P2P_SET nfc_tag 0"):
        raise Exception("Failed to disable NFC Tag for P2P static handover")
    if "OK" not in dev[0].request("P2P_SET nfc_tag 1"):
        raise Exception("Failed to enable NFC Tag for P2P static handover")
    if "OK" not in dev[0].request("P2P_SET nfc_tag 0"):
        raise Exception("Failed to disable NFC Tag for P2P static handover")
