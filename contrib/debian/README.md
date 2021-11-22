
Debian
====================
This directory contains files used to package ksocd/ksoc-qt
for Debian-based Linux systems. If you compile ksocd/ksoc-qt yourself, there are some useful files here.

## ksoc: URI support ##


ksoc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install ksoc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your ksoc-qt binary to `/usr/bin`
and the `../../share/pixmaps/ksoc128.png` to `/usr/share/pixmaps`

ksoc-qt.protocol (KDE)

