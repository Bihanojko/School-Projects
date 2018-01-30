BINARY=xvales02
QT_CONFIG=xvales02.pro
GENERATED_MAKEFILE=xvales02Makefile
QMAKE_MERLIN_PATH=/usr/local/share/Qt-5.5.1/5.5/gcc_64/bin/
QT_MERLIN_LIBS=/usr/local/share/Qt-5.5.1/5.5/gcc_64/lib/

all:
	PATH=${QMAKE_MERLIN_PATH}:${PATH} && qmake ${QT_CONFIG}
	sed -i '1iexport LD_LIBRARY_PATH=${QT_MERLIN_LIBS}:${LD_LIBRARY_PATH}' ${GENERATED_MAKEFILE}
	$(MAKE) -f ${GENERATED_MAKEFILE}

run:
	chmod +x ${BINARY}
	./${BINARY}

clean:
	$(RM) *.o ${BINARY} ${GENERATED_MAKEFILE}
	$(RM) moc_*
