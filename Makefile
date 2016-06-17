all: lib doxygen

clean:
	mvn clean
	(cd src/main/c++ && make clean)
	(cd src/test/c++ && make clean)

doxygen: | target
	doxygen

doc: doxygen
	xdg-open target/api-doc/index.html

target:
	mkdir -p $@

test:
	(cd src/test/c++ && make test)

maven:
	mvn package -Dmaven.test.skip=true

lib: target/libdevslashlirc.so

target/libdevslashlirc.so: maven
	(cd src/main/c++ && make)
	cp src/main/c++/libdevslashlirc.so $@
