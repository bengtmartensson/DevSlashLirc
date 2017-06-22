ifneq ($(shell uname -m),x86_64)
    $(error Building on other architectures than x84_64 not yet supported)
endif

dist: natives maven

all: lib doxygen

clean:
	mvn clean
	(cd src/main/c++ && make clean)
	(cd src/test/c++ && make clean)
	rm -f hs_err*.log  doxygen_sqlite3.db
	rm -r -f native

doxygen: | target
	doxygen
	mvn javadoc:javadoc

doc: doxygen
	-xdg-open target/api-doc/index.html

target native native/Linux-amd64 native/Linux-i386 native/Linux-arm:
	mkdir -p $@

test: lib
	(cd src/main/c++ && make)
	(cd src/test/c++ && make test)

maven:
	mvn install -Dmaven.test.skip=true

JAVAH_MARKER = target/classes/org/harctoolbox/devslashlirc/Mode2LircDevice.class

$(JAVAH_MARKER):
	mvn compile

lib: target/libdevslashlirc.so

target/libdevslashlirc.so: $(JAVAH_MARKER) | native
	(cd src/main/c++ && make)
	cp src/main/c++/libdevslashlirc.so $@

libs: target/libdevslashlirc.so native/libdevslashlirc.so_x86-64 native/libdevslashlirc.so_x86-32

natives: native/Linux-amd64/libdevslashlirc.so native/Linux-i386/libdevslashlirc.so
#native/Linux-arm/libdevslashlirc.so

native/Linux-amd64/libdevslashlirc.so: $(JAVAH_MARKER) | native/Linux-amd64
	(cd src/main/c++ && make clean)
	(cd src/main/c++ && make)
	cp src/main/c++/libdevslashlirc.so $@

native/Linux-i386/libdevslashlirc.so: $(JAVAH_MARKER) | native/Linux-i386
	(cd src/main/c++ && make clean)
	(cd src/main/c++ && make M=-m32)
	cp src/main/c++/libdevslashlirc.so $@

# Not yet working
native/Linux-arm/libdevslashlirc.so: $(JAVAH_MARKER) | native/Linux-arm
	(cd src/main/c++ && make clean)
	(cd src/main/c++ && make CXX=arm-linux-gnu-c++)
	cp src/main/c++/libdevslashlirc.so $@

.NOTPARALLEL:
