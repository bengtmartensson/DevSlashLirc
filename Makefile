INCLUDES = -I/usr/local/include/lirc/include -I.
WARN=-Wall -Wextra
OPT=-O0
DEBUG=-g

OBJS=LircT.o LircDevice.o Mode2LircDevice.o LircCodeLircDevice.o IrSequence.o IrSignal.o

PROGS=lirconian newLircDevice reportMode2 yamaha_power_on repeat mode2dump receive

GENERATED_SRC=target/generated-sources/native

%.o: %.cpp
	$(CXX) -o $@ $(WARN) $(INCLUDES) -fPIC $(OPT) $(DEBUG) $(CFLAGS) -c $<

all: target/liblircdevice.so

%: tests/%.cpp liblircdevice.so
	$(CXX) -o $@ $(WARN) $(INCLUDES) -Wl,-rpath -Wl,. -O0 -g $(CFLAGS) $< -L. -llircdevice

clean:
	rm -rf api-doc
	rm -f *.o *.a *.so hs_err*.log core.* $(PROGS)
	(cd src/main/native ; make clean)
	mvn clean

doc: | target
	doxygen
	xdg-open target/api-doc/index.html

target:
	mkdir -p $@

test:
	(cd src/test/native ; make test)

target/liblircdevice.so: header
	(cd src/main/native ; make)
	install src/main/native/liblircdevice.so target
	install src/main/native/liblircdevice.so native

header: \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_LircDevice.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_LircCodeLircDevice.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_Mode2LircDevice.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_LircDriver.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_LircCodeLircDriver.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_Mode2LircDriver.h

# Lie a bit to avoid running maven 3 times.
$(GENERATED_SRC)/org_harctoolbox_lircdevice_%.h: target/classes/org/harctoolbox/lircdevice/LircDevice.class
	javah -o $@ -classpath target/classes org.harctoolbox.lircdevice.$*

target/classes/org/harctoolbox/lircdevice/%.class:
	mvn package -Dmaven.test.skip=true

.SECONDARY: \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_LircCodeLircDevice.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_Mode2LircDevice.h \
	$(GENERATED_SRC)/org_harctoolbox_lircdevice_LircDevice.h \
	target/classes/org/harctoolbox/lircdevice/LircDevice.class
