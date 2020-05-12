# The minimal port

This port is intended to be a minimal MicroPython port that actually runs.
It is intented to run on dialog's da 14586 chip.

## Dialog sdk files

This port does not include the files from dialog's SDK.
It can be downloaded from dialog's website <br>
[https://www.dialog-semiconductor.com/products/connectivity/bluetooth-low-energy/smartbond-da14585-and-da14586](https://www.dialog-semiconductor.com/products/connectivity/bluetooth-low-energy/smartbond-da14585-and-da14586)

Copy the `sdk` folder from the downloaded SDK into this folder and rename it `dialog_sdk`.

You should now be able to compile micropython for the da14586.

## Building and running Linux version

Build the port via:

    $ make

## Building without the built-in MicroPython compiler

This minimal port can be built with the built-in MicroPython compiler
disabled.  This will reduce the firmware by about 20k on a Thumb2 machine,
and by about 40k on 32-bit x86.  Without the compiler the REPL will be
disabled, but pre-compiled scripts can still be executed.

To test out this feature, change the `MICROPY_ENABLE_COMPILER` config
option to "0" in the mpconfigport.h file in this directory.  Then
recompile and run the firmware and it will execute the frozentest.py
file.

This reduces the size of the compiled code to round 52000 bytes.