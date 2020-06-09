import machine
import BLE

def main():
    print("\n\nfrozentest.py\n")
    pin_x = machine.Pin("10", mode=machine.Pin.OUT, pull=machine.Pin.PULL_UP, value=0)
    for i in range(200001):
        if not i % 10000:
            print(i)

    print("FINISHED MPY")

main()
