all: lib doxygen

clean:
	mvn clean
	(cd src/main/c++ && make clean)
	(cd src/test/c++ && make clean)
	rm -f hs_err*.log  doxygen_sqlite3.db

doxygen: | target
	doxygen
	mvn javadoc:javadoc

doc: doxygen
	-xdg-open target/api-doc/index.html

target native:
	mkdir -p $@

test: lib
	(cd src/test/c++ && make test)

maven:
	mvn package -Dmaven.test.skip=true

lib: target/libdevslashlirc.so

target/libdevslashlirc.so: maven
	(cd src/main/c++ && make)
	cp src/main/c++/libdevslashlirc.so $@

libs: target/libdevslashlirc.so native/libdevslashlirc.so_x86-64  native/libdevslashlirc.so_x86-32

native/libdevslashlirc.so_x86-64: | native
	(cd src/main/c++ && make clean)
	(cd src/main/c++ && make)
	cp src/main/c++/libdevslashlirc.so $@
	cp src/main/c++/libdevslashlirc.so native

native/libdevslashlirc.so_x86-32: | native
	(cd src/main/c++ && make clean)
	(cd src/main/c++ && make M=-m32)
	cp src/main/c++/libdevslashlirc.so $@

.NOTPARALLEL:
