ifneq ($(shell uname -m),x86_64)
    $(error Building on other architectures than x84_64 not yet supported)
endif

BROWSE=xdg-open

dist: natives maven

all: lib doxygen

clean:
	mvn clean
	(cd src/main/c++ && make clean)
	(cd src/test/c++ && make clean)
	rm -f hs_err*.log  doxygen_sqlite3.db
	rm -r -f native
	rm -rf gh-pages

target/api-doc/index.html: | target
	doxygen

target/site/apidocs/index.html:
	mvn javadoc:javadoc

gh-pages: target/api-doc/index.html target/site/apidocs/index.html
	tools/update-gh-pages.sh

doc: target/api-doc/index.html target/site/apidocs/index.html
	$(foreach f,$^,$(shell $(BROWSE) $(f)))

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

target/libdevslashlirc.so: native/Linux-amd64/libdevslashlirc.so
	cp $< $@

libs: target/libdevslashlirc.so native/libdevslashlirc.so_x86-64 native/libdevslashlirc.so_x86-32

natives: native/Linux-amd64/libdevslashlirc.so native/Linux-i386/libdevslashlirc.so native/Linux-arm
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
# Until cross compiling is working, this is a working procedure:
#    make all
# Log in RPi, cd to src/main/c++ there
#    make clean all
# Then
#    cp target/libdevslashlirc.so native/Linux-arm
#native/Linux-arm/libdevslashlirc.so: $(JAVAH_MARKER) | native/Linux-arm
#	(cd src/main/c++ && make clean)
#	(cd src/main/c++ && make CXX=arm-linux-gnu-c++)
#	cp src/main/c++/libdevslashlirc.so $@

.NOTPARALLEL:
