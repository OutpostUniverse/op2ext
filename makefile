# =====
# Note: MinGW is not able to link the DLL project due to name mangling
# differences in the Outpost2DLL imported symbols. It is however able to
# compile and check the source code for errors, including compiling and linking
# of the static library project, and the unit test project. This means much of
# the code can be checked for errors on Linux, even if a full compile is not
# (yet) possible.
# =====

# Set compiler to mingw (can still override from command line)
CXX := i686-w64-mingw32-g++
CC := i686-w64-mingw32-gcc


include makefile-generic.mk


CXXFLAGS := -std=c++17 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -static-libgcc -static-libstdc++ -LOutpost2DLL/Lib/
LDLIBS := -lOutpost2DLL -lstdc++fs -lws2_32

op2extDLL_CPPFLAGS := -DOP2EXT_INTERNAL_BUILD -IsrcStatic
op2extDLL_LDFLAGS := $(LDFLAGS) -L./
op2extDLL_LDLIBS := $(LDLIBS) -lop2ext

GTESTSRCDIR := /usr/src/googletest/
GTESTINCDIR := /usr/src/googletest/googletest/include/
GTESTBUILDDIR = $(BUILDDIR)/gtest/
GTESTLIBDIR := /usr/i686-w64-mingw32/lib/

test_CPPFLAGS = -I$(op2extLib_SRCDIR) -I$(GTESTINCDIR)
test_LDFLAGS := $(LDFLAGS) -L./ -L$(GTESTBUILDDIR)googlemock/ -L$(GTESTBUILDDIR)googlemock/gtest/
test_LDLIBS := $(LDLIBS) -lop2ext -lgtest -lgtest_main

all: op2extLib
test: op2extLib

# Debug
# $(info $(call DefineCppProject,op2extLib,op2ext.lib,srcStatic))

$(eval $(call DefineCppProject,op2extLib,op2ext.lib,srcStatic))
$(eval $(call DefineCppProject,test,$(BUILDDIR)/test/unitTest.exe,test))
# $(eval $(call DefineCppProject,op2extDLL,op2ext.dll,srcDLL))


GTESTSRCDIR := /usr/src/googletest/
GTESTINCDIR := /usr/src/googletest/googletest/include/
GTESTBUILDDIR := $(BUILDDIR)/gtest/
GTESTLIBDIR := /usr/i686-w64-mingw32/lib/

.PHONY: gtest gtest-install gtest-clean
gtest:
	mkdir -p "$(GTESTBUILDDIR)"
	cd "$(GTESTBUILDDIR)" && cmake -DCMAKE_CXX_COMPILER="$(CXX)" -DCMAKE_C_COMPILER="$(CC)" -DCMAKE_CXX_FLAGS="-std=c++17" -DCMAKE_SYSTEM_NAME="Windows" -Dgtest_disable_pthreads=ON "$(GTESTSRCDIR)"
	make -C "$(GTESTBUILDDIR)"
gtest-install:
	cp $(GTESTBUILDDIR)googlemock/gtest/lib*.a "$(GTESTLIBDIR)"
	cp $(GTESTBUILDDIR)googlemock/lib*.a "$(GTESTLIBDIR)"
gtest-clean:
	rm -rf "$(GTESTBUILDDIR)"


.PHONY: check
check: test
	wine "$(test_OUTPUT)"

# Docker commands

.PHONY: docker-build-image docker-push

docker-build-image:
	docker build .circleci/ --tag outpostuniverse/gcc-mingw-wine-googletest-circleci:latest --tag outpostuniverse/gcc-mingw-wine-googletest-circleci:1.2
docker-push:
	docker login
	docker push outpostuniverse/gcc-mingw-wine-googletest-circleci


# CircleCI commands

.PHONY: circleci-build circleci-validate

circleci-build:
	circleci build
circleci-validate:
	circleci config validate
