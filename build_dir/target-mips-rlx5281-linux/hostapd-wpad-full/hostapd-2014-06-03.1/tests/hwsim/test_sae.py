# Test cases for SAE
# Copyright (c) 2013, Jouni Malinen <j@w1.fi>
#
# This software may be distributed under the terms of the BSD license.
# See README for more details.

import time
import subprocess
import logging
logger = logging.getLogger()

import hwsim_utils
import hostapd

def test_sae(dev, apdev):
    """SAE with default group"""
    params = hostapd.wpa2_params(ssid="test-sae",
                                 passphrase="12345678")
    params['wpa_key_mgmt'] = 'SAE'
    hapd = hostapd.add_ap(apdev[0]['ifname'], params)
    key_mgmt = hapd.get_config()['key_mgmt']
    if key_mgmt.split(' ')[0] != "SAE":
        raise Exception("Unexpected GET_CONFIG(key_mgmt): " + key_mgmt)

    dev[0].request("SET sae_groups ")
    id = dev[0].connect("test-sae", psk="12345678", key_mgmt="SAE",
                        scan_freq="2412")
    if dev[0].get_status_field('sae_group') != '19':
            raise Exception("Expected default SAE group not used")

def test_sae_groups(dev, apdev):
    """SAE with all supported groups"""
    # This would be the full list of supported groups, but groups 14-16
    # (2048-4096 bit MODP) are a bit too slow on some VMs and can result in
    # hitting mac80211 authentication timeout, so skip them for now.
    #sae_groups = [ 19, 25, 26, 20, 21, 2, 5, 14, 15, 16, 22, 23, 24 ]
    sae_groups = [ 19, 25, 26, 20, 21, 2, 5, 22, 23, 24 ]
    groups = [str(g) for g in sae_groups]
    params = hostapd.wpa2_params(ssid="test-sae-groups",
                                 passphrase="12345678")
    params['wpa_key_mgmt'] = 'SAE'
    params['sae_groups'] = ' '.join(groups)
    hostapd.add_ap(apdev[0]['ifname'], params)

    for g in groups:
        logger.info("Testing SAE group " + g)
        dev[0].request("SET sae_groups " + g)
        id = dev[0].connect("test-sae-groups", psk="12345678", key_mgmt="SAE",
                            scan_freq="2412")
        if dev[0].get_status_field('sae_group') != g:
            raise Exception("Expected SAE group not used")
        dev[0].remove_network(id)

def test_sae_group_nego(dev, apdev):
    """SAE group negotiation"""
    params = hostapd.wpa2_params(ssid="test-sae-group-nego",
                                 passphrase="12345678")
    params['wpa_key_mgmt'] = 'SAE'
    params['sae_groups'] = '19'
    hostapd.add_ap(apdev[0]['ifname'], params)

    dev[0].request("SET sae_groups 25 26 20 19")
    dev[0].connect("test-sae-group-nego", psk="12345678", key_mgmt="SAE",
                   scan_freq="2412")
    if dev[0].get_status_field('sae_group') != '19':
        raise Exception("Expected SAE group not used")

def test_sae_anti_clogging(dev, apdev):
    """SAE anti clogging"""
    params = hostapd.wpa2_params(ssid="test-sae", passphrase="12345678")
    params['wpa_key_mgmt'] = 'SAE'
    params['sae_anti_clogging_threshold'] = '1'
    hostapd.add_ap(apdev[0]['ifname'], params)

    dev[0].request("SET sae_groups ")
    dev[1].request("SET sae_groups ")
    id = {}
    for i in range(0, 2):
        dev[i].scan(freq="2412")
        id[i] = dev[i].connect("test-sae", psk="12345678", key_mgmt="SAE",
                               scan_freq="2412", only_add_network=True)
    for i in range(0, 2):
        dev[i].select_network(id[i])
    for i in range(0, 2):
        ev = dev[i].wait_event(["CTRL-EVENT-CONNECTED"], timeout=10)
        if ev is None:
            raise Exception("Association with the AP timed out")

def test_sae_forced_anti_clogging(dev, apdev):
    """SAE anti clogging (forced)"""
    params = hostapd.wpa2_params(ssid="test-sae", passphrase="12345678")
    params['wpa_key_mgmt'] = 'SAE WPA-PSK'
    params['sae_anti_clogging_threshold'] = '0'
    hostapd.add_ap(apdev[0]['ifname'], params)
    dev[2].connect("test-sae", psk="12345678", scan_freq="2412")
    for i in range(0, 2):
        dev[i].request("SET sae_groups ")
        dev[i].connect("test-sae", psk="12345678", key_mgmt="SAE",
                       scan_freq="2412")

def test_sae_mixed(dev, apdev):
    """Mixed SAE and non-SAE network"""
    params = hostapd.wpa2_params(ssid="test-sae", passphrase="12345678")
    params['wpa_key_mgmt'] = 'SAE WPA-PSK'
    params['sae_anti_clogging_threshold'] = '0'
    hostapd.add_ap(apdev[0]['ifname'], params)

    dev[2].connect("test-sae", psk="12345678", scan_freq="2412")
    for i in range(0, 2):
        dev[i].request("SET sae_groups ")
        dev[i].connect("test-sae", psk="12345678", key_mgmt="SAE",
                       scan_freq="2412")
