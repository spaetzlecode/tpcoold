# Update (2015): Do not use this!

I can no longer recommend using this hack. 

I found out that thinkfan can actually be configured to run "full-speed" setting - use the numerical value 126 for that level. Here's my thinkfan.conf:

    (0,	0,	55)
    (1,	48,	60)
    (2,	50,	61)
    (3,	52,	63)
    (4,	56,	65)
    (5,	59,	66)
    (6,	61,	68)
    (7,	63,	70)
    (126,	65,	32767)
    
Use at your own risk.

On a related note, level 127 is supposed to be "auto" mode, so you could implement something similar to tpcoold using the following configuration (untested!!!):

    (127,	0,	66)
    (126,	63,	32767)

Simple fan control for thinkpad
==============
Workaround for thinkfan limitations to fix the overheating of my Lenovo Thinkpad T410.

Unfortunately, the highest fan level (7) of the thinkpad\_acpi driver does not give a high enough fan speed (only ~4500 RPM), whereas "full-speed" setting does (~7500 RPM). Thinkfan cannot be configured to use this mode (or that's what I thought, see update above).

This implements a hysteris control using the modes "auto" and "full-speed".

Configuration
-------------
No options at this time - modify source...

Installation
------------
First, uninstall thinkfan, if you are using it. Make sure __thinkpad\_acpi__ module is loaded.

The __install__ target for make should work for Debian-based systems:

    make
    sudo make install
    sudo update-rc.d tpcool defaults

and reboot...

Disclaimer
------------
This is an experimental program and has only been tested to work on one machine, and poorly so. I do not accept any responsibility for damages resulting from the use of this program. Be careful and do not burn your laptop's CPU, legs, sofa, etc.

