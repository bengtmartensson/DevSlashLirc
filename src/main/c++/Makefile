ifneq ($(wildcard /usr/lib/jvm/java/include),)
    JAVA_INCLUDE=/usr/lib/jvm/java/include
else
    JAVA_INCLUDE=/usr/lib/jvm/java-8-openjdk-armhf/include
endif
JAVAC=javac
GENERATED=../../../target/generated-sources/c++/org/harctoolbox/devslashlirc

# Where linux/lirc.h is to be found. "Should" now be a part of the system.
# Use the next line if your system does not have it.
ifeq ($(wildcard /usr/include/linux/lirc.h),)
LIRC_INCLUDE=-Ifallback
endif

INCLUDES = -I$(JAVA_INCLUDE) -I$(JAVA_INCLUDE)/linux -I$(GENERATED) $(LIRC_INCLUDE) -I.
WARN=-Wall -Wextra -Werror -Wmissing-declarations
OPT=-O2
DEBUG=-g
STD=--std=c++11
M=

OBJS=LircT.o IrSequence.o \
LircDevice.o Mode2LircDevice.o LircCodeLircDevice.o \
org_harctoolbox_devslashlirc_Mode2LircDevice.o \
org_harctoolbox_devslashlirc_LircCodeLircDevice.o \
org_harctoolbox_devslashlirc_LircDevice.o
#LircDriver.o LircCodeLircDriver.o Mode2LircDriver.o \
#org_harctoolbox_devslashlirc_LircDriver.o \
#org_harctoolbox_devslashlirc_Mode2LircDriver.o org_harctoolbox_devslashlirc_LircCodeLircDriver.o

#PROGS=lirconian newLircDevice reportMode2 yamaha_power_on repeat mode2dump receive


all: libdevslashlirc.so libdevslashlirc.a ../../../target/libdevslashlirc.so

org_harctoolbox_devslashlirc_LircDevice.o: $(GENERATED)/org_harctoolbox_devslashlirc_LircDevice.h
org_harctoolbox_devslashlirc_Mode2LircDevice.o: $(GENERATED)/org_harctoolbox_devslashlirc_Mode2LircDevice.h
org_harctoolbox_devslashlirc_LircCodeLircDevice.o: $(GENERATED)/org_harctoolbox_devslashlirc_LircCodeLircDevice.h


$(GENERATED)/org_harctoolbox_devslashlirc_%.h:
	$(JAVAC) -h $(GENERATED) -classpath ../../../target/classes ../java/org/harctoolbox/devslashlirc/$*.java

../../../target/libdevslashlirc.so: libdevslashlirc.so | ../../../target
	cp $< $@

../../../target:
	mkdir $@

%.o: %.cpp
	$(CXX) $(M) $(STD) $(DEFINES) -o $@ $(WARN) $(INCLUDES) -fPIC $(OPT) $(DEBUG) $(CFLAGS) -c $<

#%.o: %.c
#	$(CC) -o $@ $(INCLUDES) -fPIC $(OPT) $(DEBUG) $(CFLAGS) -c $<

libdevslashlirc.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

libdevslashlirc.so: $(OBJS)
	$(CXX) $(M) -shared -Wl,--export-dynamic -o $@ $(OBJS)

#%: tests/%.cpp libdevslashlirc.so
#	$(CXX) -o $@ $(WARN) $(INCLUDES) -Wl,-rpath -Wl,. -O0 -g $(CFLAGS) $< -L. -ldevslashlirc

clean:
	rm -f *.o *.a *.so $(PROGS)
