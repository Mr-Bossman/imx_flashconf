# imx-flashconfig

This is a program to generate flash config for booting imxrt from flexspi.

```
userland@userland:~$ imx-flashconf -h
+--------------------------------------------------------------+
| -h - This message.                                           |
| -o - Output file name.                                       |
| -n - Name of flash config to use, defined in flash_confs.c . |
| -p - Padding offset, other than default (0x1000).            |
| -b - Create binary image, and offset to put u-boot.img at.   |
| -l - List names of flash defined in flash_confs.c .          |
+--------------------------------------------------------------+
```