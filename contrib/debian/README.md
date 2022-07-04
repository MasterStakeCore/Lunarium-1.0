
Debian
====================
This directory contains files used to package lunariumd/lunarium-qt
for Debian-based Linux systems. If you compile lunariumd/lunarium-qt yourself, there are some useful files here.

## lunarium: URI support ##


lunarium-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install lunarium-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your lunariumqt binary to `/usr/bin`
and the `../../share/pixmaps/lunarium128.png` to `/usr/share/pixmaps`

lunarium-qt.protocol (KDE)

