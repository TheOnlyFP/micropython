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
    print("mpy execution works")


def on_connect():
    """executed on connect of BLE
    """
    print("on_connect")
