ifneq ($(shell uname -m),x86_64)
    $(error Building on other architectures than x84_64 not yet supported)
endif

MYDIR := $(dir $(firstword $(MAKEFILE_LIST)))
TOP := $(realpath $(MYDIR))

include $(MYDIR)/tools/paths.mk

PROJECT_NAME := $(notdir $(TOP))
PROJECT_NAME_LOWERCASE := $(shell echo $(PROJECT_NAME) | tr A-Z a-z)
EXTRACT_VERSION := $(TOP)/tools/extract_project_version.xsl
VERSION := $(shell $(XSLTPROC) $(EXTRACT_VERSION) pom.xml)
PROJECT_JAR := target/$(PROJECT_NAME)-$(VERSION).jar
PROJECT_BIN := target/$(PROJECT_NAME)-$(VERSION)-bin.zip
GH_PAGES := $(TOP)/gh-pages
NATIVES := native/Linux-amd64/libdevslashlirc.so native/Linux-i386/libdevslashlirc.so native/Linux-arm
LIBS := target/libdevslashlirc.so $(NATIVES)
DOCS := target/api-doc/index.html target/site/apidocs/index.html

all: $(LIBS) $(PROJECT_BIN) $(DOCS)

target native native/Linux-amd64 native/Linux-i386 native/Linux-arm:
	mkdir -p $@

test: lib
	(cd src/main/c++ && make)
	(cd src/test/c++ && make test)

$(PROJECT_JAR) $(PROJECT_BIN): $(LIBS)
	mvn install -Dmaven.test.skip=true

$(PROJECT_JAR)-test:
	mvn install -Dmaven.test.skip=false

release: push gh-pages tag deploy

JAVAH_MARKER = target/classes/org/harctoolbox/devslashlirc/Mode2LircDevice.class

$(JAVAH_MARKER):
	mvn compile

lib: target/libdevslashlirc.so

target/libdevslashlirc.so: native/Linux-amd64/libdevslashlirc.so
	cp $< $@

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

release: push gh-pages tag deploy

version:
	@echo $(VERSION)

setversion:
	mvn versions:set -DnewVersion=$(NEWVERSION)
	git commit -S -m "Set version to $(NEWVERSION)" pom.xml

deploy:
	mvn deploy -P release

target/api-doc/index.html: | target
	doxygen

target/site/apidocs/index.html:
	mvn javadoc:javadoc

push:
	git push

gh-pages: target/api-doc/index.html target/site/apidocs/index.html
	tools/update-gh-pages.sh

doc: target/api-doc/index.html target/site/apidocs/index.html
	$(foreach f,$^,$(shell $(BROWSE) $(f)))

tag:
	git checkout master
	git status
	git tag -s -a Version-$(VERSION) -m "Tagging Version-$(VERSION)"
	git push origin Version-$(VERSION)

clean:
	mvn clean
	(cd src/main/c++ && make clean)
	(cd src/test/c++ && make clean)
	rm -f hs_err*.log  doxygen_sqlite3.db
	rm -r -f native
	rm -rf gh-pages

.NOTPARALLEL:

.PHONY: clean $(PROJECT_JAR)-test release
