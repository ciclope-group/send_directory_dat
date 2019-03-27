bin_PROGRAMS=sendat
sendat_SOURCES=sendat.c
sendat_LDADD=$(LIBOBJS)

bin_SCRIPTS= sendat.sh sendat.conf
install-exec-hook:
	mkdir -p /etc/sendat
	pwd
	cp $(srcdir)/sendat.conf /etc/sendat/sendat.conf
	cp $(srcdir)/sendat.sh /etc/init.d/sendat
	chmod u+x /etc/init.d/sendat

	ln -s ../init.d/sendat /etc/rc5.d/S01sendat
	ln -s ../init.d/sendat /etc/rc4.d/S01sendat
	ln -s ../init.d/sendat /etc/rc3.d/S01sendat

uninstall-hook:
	rm -rf /etc/sendat
	rm /etc/init.d/sendat
	rm /etc/rc5.d/S01sendat
	rm /etc/rc4.d/S01sendat
	rm /etc/rc3.d/S01sendat
