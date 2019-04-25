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
BINDIR := $(BUILDDIR)/bin
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/obj
OUTPUT := op2ext.lib

CPPFLAGS := -DOP2EXT_INTERNAL_BUILD
CXXFLAGS := -std=c++17 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -static-libgcc -static-libstdc++ -LOutpost2DLL/Lib/
LDLIBS := -lOutpost2DLL -lstdc++fs -lws2_32

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
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
	$(CXX) $^ $(LDFLAGS) -o $@ $(LDLIBS)

%.dll:
	@mkdir -p ${@D}
	$(CXX) $^ -shared $(LDFLAGS) -o $@ $(LDLIBS)

%.lib:
	@mkdir -p ${@D}
	ar rcs $@ $^

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(DEPDIR)/%.d | build-folder
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

.PHONY: build-folder
build-folder:
	@mkdir -p $(patsubst $(SRCDIR)/%,$(OBJDIR)/%, $(FOLDERS))
	@mkdir -p $(patsubst $(SRCDIR)/%,$(DEPDIR)/%, $(FOLDERS))

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SRCS)))

.PHONY: clean clean-all
clean:
	-rm -rf "$(BUILDDIR)"
clean-all: clean
	-rm -f "$(OUTPUT)"


GTESTSRCDIR := /usr/src/googletest/
GTESTINCDIR := /usr/src/googletest/googletest/include/
GTESTBUILDDIR := $(BUILDDIR)/gtest/
GTESTLIBDIR := /usr/i686-w64-mingw32/lib/

.PHONY: gtest gtest-install gtest-clean
gtest:
	mkdir -p $(GTESTBUILDDIR)
	cd $(GTESTBUILDDIR) && cmake -DCMAKE_CXX_FLAGS="-std=c++17" -DCMAKE_SYSTEM_NAME="Windows" -Dgtest_disable_pthreads=ON $(GTESTSRCDIR)
	make -C $(GTESTBUILDDIR)
gtest-install:
	cp $(GTESTBUILDDIR)googlemock/gtest/lib*.a $(GTESTLIBDIR)
	cp $(GTESTBUILDDIR)googlemock/lib*.a $(GTESTLIBDIR)
gtest-clean:
	rm -rf $(GTESTBUILDDIR)


# Objects with references to Outpost2DLL or _ReturnAddress are a problem for the linker
OBJSWITHREFS := $(OBJDIR)/DllMain.o $(OBJDIR)/IpDropDown.o
SRCOBJS := $(filter-out $(OBJSWITHREFS),$(OBJS)) # Remove objects with problem references

TESTDIR := test
TESTOBJDIR := $(BUILDDIR)/testObj
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TESTSRCS))
TESTFOLDERS := $(sort $(dir $(TESTSRCS)))
TESTCPPFLAGS := -I$(SRCDIR) -I$(GTESTINCDIR)
TESTLDFLAGS := -static-libgcc -static-libstdc++ -L./ -L$(GTESTBUILDDIR)googlemock/ -L$(GTESTBUILDDIR)googlemock/gtest/
TESTLIBS := -lgtest -lgtest_main -lstdc++fs
TESTOUTPUT := $(BUILDDIR)/testBin/runTests

TESTDEPFLAGS = -MT $@ -MMD -MP -MF $(TESTOBJDIR)/$*.Td
TESTCOMPILE.cpp = $(CXX) $(TESTCPPFLAGS) $(TESTDEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
TESTPOSTCOMPILE = @mv -f $(TESTOBJDIR)/$*.Td $(TESTOBJDIR)/$*.d && touch $@

.PHONY: check
check: $(TESTOUTPUT)
	wine $(TESTOUTPUT)

$(TESTOUTPUT): $(TESTOBJS) $(SRCOBJS)
	@mkdir -p ${@D}
	$(CXX) $^ $(TESTLDFLAGS) $(TESTLIBS) -o $@

$(TESTOBJS): $(TESTOBJDIR)/%.o : $(TESTDIR)/%.cpp $(TESTOBJDIR)/%.d | test-build-folder
	$(TESTCOMPILE.cpp) $(OUTPUT_OPTION) $<
	$(TESTPOSTCOMPILE)

.PHONY: test-build-folder
test-build-folder:
	@mkdir -p $(patsubst $(TESTDIR)/%,$(TESTOBJDIR)/%, $(TESTFOLDERS))

$(TESTOBJDIR)/%.d: ;
.PRECIOUS: $(TESTOBJDIR)/%.d

include $(wildcard $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.d,$(TESTSRCS)))
