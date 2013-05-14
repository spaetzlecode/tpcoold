
tpcoold: tpcoold.c

.PHONY: install

install: tpcoold
	install thinkpad_acpi.conf /etc/modprobe.d/
	install tpcoold /usr/local/sbin/tpcoold
	install tpcool /etc/init.d

