JAVA_INCLUDE=/usr/lib/jvm/java/include
GENERATED=../../../target/generated-sources/c++/org/harctoolbox/devslashlirc
INCLUDES = -I$(JAVA_INCLUDE) -I$(JAVA_INCLUDE)/linux -I$(GENERATED) -I/usr/local/include/lirc/include -I../../main/c++
WARN=-Wall -Wextra
OPT=-O0
DEBUG=-g

TEST_PROGS= reportMode2 yamaha_power_on mode2dump receive

all: test

%.o: %.cpp
	$(CXX) -o $@ $(WARN) $(INCLUDES) -fPIC $(OPT) $(DEBUG) $(CFLAGS) -c $<

%: %.cpp
	$(CXX) -o $@ $(WARN) $(INCLUDES) -Wl,-rpath -Wl,../../../target -O0 -g $(CFLAGS) $< -L../../../target -ldevslashlirc

test: $(TEST_PROGS)
	./reportMode2
	./yamaha_power_on
	./receive
	./mode2dump

clean:
	rm -f *.o *.a *.so $(TEST_PROGS)
