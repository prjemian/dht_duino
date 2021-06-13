# dht_duino

Report time, humidity, and temperature using Arduino.

## Hardware

* Arduino (Uno Rev 3 or Mega)
* DHT22 digital humidity & temperature sensor
* DS3231 real-time clock

The [hardware assembly](assembly.md) provides photographs of the hardware
assembly and other details.

## Linux: Connect and locate the Arduino Serial Port

<details>
<summary>Connected: `/dev/ttyUSB0`</summary>

```sh
(bluesky_2021_2) jemian@otz .../prjemian/dht_duino $ dmesg | grep -i ftdi
[  919.439376] usb 1-2: Manufacturer: FTDI
[  919.540376] usbcore: registered new interface driver ftdi_sio
[  919.540390] usbserial: USB Serial support registered for FTDI USB Serial Device
[  919.540448] ftdi_sio 1-2:1.0: FTDI USB Serial Device converter detected
[  919.540894] usb 1-2: FTDI USB Serial Device converter now attached to ttyUSB0
```

</details>

<details>
<summary>Disconnected: `/dev/ttyUSB0`</summary>

```sh
(bluesky_2021_2) jemian@wow .../prjemian/dht_duino $ dmesg | grep -i ftdi | tail -4
[3373492.334136] ftdi_sio 1-12:1.0: FTDI USB Serial Device converter detected
[3373492.334528] usb 1-12: FTDI USB Serial Device converter now attached to ttyUSB0
[3374411.734158] ftdi_sio ttyUSB0: FTDI USB Serial Device converter now disconnected from ttyUSB0
[3374411.734200] ftdi_sio 1-12:1.0: device disconnected
```

</details>


## Linux: run `dht_duino.py` program

Listen for the `dht_duino.ino` reports via USB
using a Python program:

<details>
<summary>Successful</summary>

```sh
(bluesky_2021_2) jemian@otz .../prjemian/dht_duino $ ./dht_duino.py /dev/ttyUSB0
ser.name = '/dev/ttyUSB0' (port)
ser.baudrate = 115200 (bit/s)
ser.timeout = 5 (s)
2021-06-13 15:08:59.397310 ======================================
2021-06-13 15:08:59.397566 compiled: Jun 12 2021 00:48:33
2021-06-13 15:08:59.397953 RTC is older than compile time!  (Updating DateTime)
2021-06-13 15:09:01.413054 2021-06-12 00:48:35  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:03.395940 2021-06-12 00:48:37  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:05.410855 2021-06-12 00:48:39  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:07.409839 2021-06-12 00:48:41  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:09.408497 2021-06-12 00:48:43  RH=39.10 % T=24.00 C % RTC_T=25.00 C
```

</details>

<details>
<summary>Not Successful</summary>

We know this will fail because `$USER` is not in the dialout group:

```sh
grep dialout /etc/group | grep ${USER}
```

is empty.

```sh
(bluesky_2021_2) jemian@wow .../prjemian/dht_duino $ ./dht_duino.py /dev/ttyUSB0
Traceback (most recent call last):
  File "/home/beams/JEMIAN/.conda/envs/bluesky_2021_2/lib/python3.8/site-packages/serial/serialposix.py", line 322, in open
    self.fd = os.open(self.portstr, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
PermissionError: [Errno 13] Permission denied: '/dev/ttyUSB0'

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "./dht_duino.py", line 72, in <module>
    main(args.port, args.baud, args.timeout)
  File "./dht_duino.py", line 34, in main
    with serial.Serial(port, baud, timeout=timeout) as ser:
  File "/home/beams/JEMIAN/.conda/envs/bluesky_2021_2/lib/python3.8/site-packages/serial/serialutil.py", line 244, in __init__
    self.open()
  File "/home/beams/JEMIAN/.conda/envs/bluesky_2021_2/lib/python3.8/site-packages/serial/serialposix.py", line 325, in open
    raise SerialException(msg.errno, "could not open port {}: {}".format(self._port, msg))
serial.serialutil.SerialException: [Errno 13] could not open port /dev/ttyUSB0: [Errno 13] Permission denied: '/dev/ttyUSB0'
```

</details>

## Run the `dht_duino.py` test program

Needs the *PySerial* package, in addition to the standard Python libraries.

*Help* message:

```sh
(bluesky_2021_2) jemian@wow .../prjemian/dht_duino $ ./dht_duino.py -h
usage: dht_duino.py [-h] [-b BAUD] [-t TIMEOUT] port

Read input via USB from Arduino reporting: time humidity, & temperature.

positional arguments:
  port                  serial port (such as '/dev/ttyUSB0')

optional arguments:
  -h, --help            show this help message and exit
  -b BAUD, --baud BAUD  communications rate (default: 115200)
  -t TIMEOUT, --timeout TIMEOUT
                        timeout (s) for next reading (default: 5)
```

* Run the program: `./dht_duino.py /dev/ttyUSB0`
* Stop the program with `^C` from the keyboard.

The Arduino reports every 2 seconds.  The program will stop if the next report is not received in 5 seconds (default timeout).

```sh
(bluesky_2021_2) jemian@otz .../prjemian/dht_duino $ ./dht_duino.py /dev/ttyUSB0
ser.name = '/dev/ttyUSB0' (port)
ser.baudrate = 115200 (bit/s)
ser.timeout = 5 (s)
2021-06-13 15:08:59.397310 ======================================
2021-06-13 15:08:59.397566 compiled: Jun 12 2021 00:48:33
2021-06-13 15:08:59.397953 RTC is older than compile time!  (Updating DateTime)
2021-06-13 15:09:01.413054 2021-06-12 00:48:35  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:03.395940 2021-06-12 00:48:37  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:05.410855 2021-06-12 00:48:39  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:07.409839 2021-06-12 00:48:41  RH=39.10 % T=24.10 C % RTC_T=25.00 C
2021-06-13 15:09:09.408497 2021-06-12 00:48:43  RH=39.10 % T=24.00 C % RTC_T=25.00 C
^Cexc = KeyboardInterrupt()
```
