JAVA_INCLUDE=/usr/lib/jvm/java/include
GENERATED=../../../target/generated-sources/c++/org/harctoolbox/devslashlirc
INCLUDES = -I$(JAVA_INCLUDE) -I$(JAVA_INCLUDE)/linux -I$(GENERATED) -I/usr/local/include/lirc/include -I../../main/c++
WARN=-Wall -Wextra
OPT=-O0
DEBUG=-g

ifeq ($(shell uname -m),x86_64)
    LIBDIR := ../../../native/Linux-amd64
else
    $(error Building on other architectures than x84_64 not yet supported)
endif

TEST_PROGS= reportMode2 yamaha_power_on mode2dump receive

all: test

%.o: %.cpp
	$(CXX) -o $@ $(WARN) $(INCLUDES) -fPIC $(OPT) $(DEBUG) $(CFLAGS) -c $<

%: %.cpp
	$(CXX) -o $@ $(WARN) $(INCLUDES) -Wl,-rpath -Wl,$(LIBDIR) -O0 -g $(CFLAGS) $< -L$(LIBDIR) -ldevslashlirc

test: $(TEST_PROGS)
	./reportMode2
	./yamaha_power_on
	sleep 1; ./receive
	./mode2dump

clean:
	rm -f *.o *.a *.so $(TEST_PROGS)
