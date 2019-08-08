# =====
# Note: Makefile is used for error checking the source with a Linux compiler.
# The Linux compiler is not able to do a full build of the target DLL.
# It is used to check for non-standards compliant code and warnings.
# =====

# Set compiler to mingw (can still override from command line)
CXX := i686-w64-mingw32-g++
CC := i686-w64-mingw32-gcc

SRCDIR := srcStatic
BUILDDIR := .build
INTDIR := $(BUILDDIR)/intermediate
OUTPUT := op2ext.lib

CPPFLAGS := -DOP2EXT_INTERNAL_BUILD
CXXFLAGS := -std=c++17 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -static-libgcc -static-libstdc++ -LOutpost2DLL/Lib/
LDLIBS := -lOutpost2DLL -lstdc++fs -lws2_32

DEPNAME = $(patsubst %.o,%.d,$@)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPNAME).temp
COMPILE.cpp = $(CXX) $(OUTPUT_OPTION) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c $<
POSTCOMPILE = @mv -f $(DEPNAME).temp $(DEPNAME) && touch $@

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.o,$(SRCS))
FOLDERS := $(sort $(dir $(SRCS)))

# MinGW is not able to link the main DLL due to dependence on Outpost2DLL.
# The main problem is name mangling differences for the imported symbols.
# In particular, the TApp related functions are a problem.
# The code can be compiled, which checks and reports errors, but the result
# is not linkable into a usable module.

# Compiling the static library should not be a problem.

# To avoid spurious errors, default make rule should be:
#   Compile and link static library
#   Compile but not link DLL

.PHONY: default all
default: all
all: $(OUTPUT)

$(OUTPUT): $(OBJS)

%:
	@mkdir -p ${@D}
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.dll:
	@mkdir -p ${@D}
	$(CXX) -o $@ $^ -shared $(LDFLAGS) $(LDLIBS)

%.lib:
	@mkdir -p ${@D}
	ar rcs $@ $^

$(OBJS): $(INTDIR)/%.o : $(SRCDIR)/%.cpp $(INTDIR)/%.d | build-folder
	$(COMPILE.cpp)
	$(POSTCOMPILE)

.PHONY: build-folder
build-folder:
	@mkdir -p $(patsubst $(SRCDIR)/%,$(INTDIR)/%, $(FOLDERS))

$(INTDIR)/%.d: ;
.PRECIOUS: $(INTDIR)/%.d

include $(wildcard $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.d,$(SRCS)))

.PHONY: clean clean-all
clean:
	-rm -rf "$(INTDIR)"
clean-all: clean
	-rm -rf "$(BUILDDIR)"
	-rm -f "$(OUTPUT)"


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


# Objects with references to Outpost2DLL are a problem for the linker
OBJSWITHREFS := $(INTDIR)/DllMain.o
SRCOBJS := $(filter-out $(OBJSWITHREFS),$(OBJS)) # Remove objects with problem references

TESTDIR := test
TESTINTDIR := $(BUILDDIR)/testObj
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTINTDIR)/%.o,$(TESTSRCS))
TESTFOLDERS := $(sort $(dir $(TESTSRCS)))
TESTCPPFLAGS := -I$(SRCDIR) -I$(GTESTINCDIR)
TESTLDFLAGS := $(LDFLAGS) -L./ -L$(GTESTBUILDDIR)googlemock/ -L$(GTESTBUILDDIR)googlemock/gtest/
TESTLIBS := $(LDLIBS) -lgtest -lgtest_main
TESTOUTPUT := $(BUILDDIR)/testBin/runTests

TESTCOMPILE.cpp = $(CXX) $(OUTPUT_OPTION) $(TESTCPPFLAGS) $(DEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c $<

.PHONY: test check
test: $(TESTOUTPUT)
check: | test
	wine $(TESTOUTPUT)

$(TESTOUTPUT): $(TESTOBJS) $(SRCOBJS)
	@mkdir -p ${@D}
	$(CXX) $^ $(TESTLDFLAGS) $(TESTLIBS) -o $@

$(TESTOBJS): $(TESTINTDIR)/%.o : $(TESTDIR)/%.cpp $(TESTINTDIR)/%.d | test-build-folder
	$(TESTCOMPILE.cpp)
	$(POSTCOMPILE)

.PHONY: test-build-folder
test-build-folder:
	@mkdir -p $(patsubst $(TESTDIR)/%,$(TESTINTDIR)/%, $(TESTFOLDERS))

$(TESTINTDIR)/%.d: ;
.PRECIOUS: $(TESTINTDIR)/%.d

include $(wildcard $(patsubst $(TESTDIR)/%.cpp,$(TESTINTDIR)/%.d,$(TESTSRCS)))


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
