.. _infineon-pulse-counter-sample:

Pulse Counter Alarm Sample
##########################

Overview
********

This sample illustrates the use of the counter driver as a pulse counter.
The pulses captured using the pin P9_0 and channel-0 of the TCPWM1 block
is utilized as a hardware counter. The top value of the counter is set to 10.
Thus after 10 pulses user callback will be called. The use of the alarm is
illustrated in the example. For the pin P9_1 is used in gpio mode and
generates the pulses. Pin P9_1 is hardcoded inside the main.c file under
PULSE_PIN and PULSE_NODE. Pin P9_0 is found inside of the .overlay file.
Modification of both of these are required to change the pins.

Requirements
************

Connect P9_0 and P9_1 using jumper wire.

Supported boards
****************

* cy8cproto_062_4343w

Building and Running
********************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/infineon/pulse_counter
   :board: cy8cproto_062_4343w
   :goals: build flash
   :compact:

Sample Output
=============

 .. code-block:: console

   *** Booting Zephyr OS build zephyr-v3.3.0-4096-gd2bb1fdb4048 ***
    Pulse Counter Sample
    Set alarm in 10 count
    Count = 0
    Count = 0
    Count = 0
    Count = 0
    Count = 1
    Count = 1
    Count = 2
    Count = 2
    Count = 3
    Count = 3
    Count = 4
    Count = 4
    Count = 5
    Count = 5
    Count = 6
    Count = 6
    Count = 7
    Count = 7
    Count = 8
    Count = 8
    Count = 9
    Count = 9
    Count = 10
    Count = 10
    !! Alarm !!
    !! Count = 11
