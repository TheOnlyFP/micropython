import machine
print("machine found")

import BLE
print("BLE found")


def main():
    print("\n\nfrozentest.py\n")
    pin_x = machine.Pin("10", mode=machine.Pin.OUT, pull=machine.Pin.PULL_UP, value=0)
    for i in range(1000):
        if not i % 500:
            print(i)

    print("FINISHED MPY")

main()

def tester():
    machine.machine()
    print("mpy execution works")


def on_connect():
    """executed on connect of BLE
    """
    print("on_connect")

def on_disconnect():
    """executed on disconnect of BLE
    """
    print("on_disconnect")

def set_dev_config_complete():
    """executed on disconnect of BLE
    """
    print("set_dev_config_complete")

def app_advertise_complete():
    """executed on disconnect of BLE
    """
    print("app_advertise_complete")

def default_app_on_db_init_complete():
    """executed on disconnect of BLE
    """
    print("default_app_on_db_init_complete")

def default_app_on_get_dev_name():
    """executed on disconnect of BLE
    """
    print("default_app_on_get_dev_name")

def default_app_on_get_dev_appearance():
    """executed on disconnect of BLE
    """
    print("default_app_on_get_dev_appearance")

def default_app_on_get_dev_slv_pref_params():
    """executed on disconnect of BLE
    """
    print("default_app_on_get_dev_slv_pref_params")

def default_app_on_set_dev_info():
    """executed on disconnect of BLE
    """
    print("default_app_on_set_dev_info")

def default_app_update_params_request():
    """executed on disconnect of BLE
    """
    print("default_app_update_params_request")

def default_app_generate_static_random_addr():
    """executed on disconnect of BLE
    """
    print("default_app_generate_static_random_addr")

def default_app_on_pairing_request():
    """executed on disconnect of BLE
    """
    print("default_app_on_pairing_request")

def default_app_on_tk_exch():
    """executed on disconnect of BLE
    """
    print("default_app_on_tk_exch")

def default_app_on_ltk_exch():
    """executed on disconnect of BLE
    """
    print("default_app_on_ltk_exch")
