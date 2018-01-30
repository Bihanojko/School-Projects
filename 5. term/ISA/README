// Subject: Network Applications and Network Administration
// Project: Monitoring of DHCP Communication
// Author: Nikola Valesova
// Login: xvales02
// Date: 19. 11. 2016
// README

Monitorování DHCP komunikace

Program monitoruje vytížení síťového prefixu a vytváří ze získaných údajů statistiky. Při spuštění s přepínačem -c způsobí vypsání aktuální statistiky o vytížení do souboru logfile.csv, a to každých int sekund.

Použití:
	./dhcp-stats <ip-prefix> [ <ip-prefix> [ ... ] ] [-c <int>]

Pro vypsání nápovědy program spusťte jako:
	./dhcp-stats -h 
nebo
	./dhcp-stats --help

Pro zobrazení manuálové stránky zadejte sekvenci:
	man -l dhcp-stats.1

Pro spuštění na referenčním zařízení je třeba doinstalovat balíčky knihoven ncurses a libstdc++ pomocí příkazů:
	yum install ncurses-devel
	yum install libstdc++-static libstdc++-static.i686

Seznam odevzdaných souborů:
	README
	Makefile
	dhcp-stats.1
	dhcp-stats.cpp
	dhcp-stats.h
	get-packets.cpp
	process-packets.cpp
	output-functions.cpp
