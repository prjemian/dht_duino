#!/usr/bin/env python

"""
Read input via USB from Arduino reporting: time humidity, & temperature.

USAGE::

    ./dht_duino.py /dev/ttyUSB1

USEFUL LINUX COMMANDS:

The Python commands are provided by the PySerial python package.

description | command
:--- | :---
Has system recognized the Arduino? | `lsusb | grep "FT232 Serial"`
Will tell if device connected to some tty | `dmesg | grep -i ftdi`
List available serial ports | `python -m serial.tools.list_ports`
Miniterm application | `python -m serial.tools.miniterm <port_name> 115200`
"""

import argparse
import datetime
import serial

PORT = "/dev/ttyUSB0"
BAUD = 115200
TIMEOUT = 5


def main(port, baud, timeout=5):
    baud = int(baud)
    timeout = int(timeout)
    with serial.Serial(port, baud, timeout=timeout) as ser:
        # ser.flush()
        print(f"{ser.name = } (port)")
        print(f"{ser.baudrate = } (bit/s)")
        print(f"{ser.timeout = } (s)")
        while True:
            try:
                # read a '\n' terminated line
                line = ser.readline().decode("utf8").strip()
                print(datetime.datetime.now(), line)
            except (KeyboardInterrupt, serial.SerialException) as exc:
                print(f"{exc = }")
                break


def get_args():
    doc = __doc__.strip().splitlines()[0]
    parser = argparse.ArgumentParser(description=doc)
    parser.add_argument("port", help="serial port (such as '/dev/ttyUSB0')")
    parser.add_argument(
        "-b",
        "--baud",
        type=int,
        default=115200,
        help="communications rate (default: 115200)",
    )
    parser.add_argument(
        "-t",
        "--timeout",
        type=int,
        default=5,
        help="timeout (s) for next reading (default: 5)",
    )
    return parser.parse_args()


if __name__ == "__main__":
    args = get_args()
    main(args.port, args.baud, args.timeout)
