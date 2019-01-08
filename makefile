# =====
# Note: This makefile fails at the link step due to name mangling differences
# between MSVC and MinGW. This affects the import of TApp related functions.
# Additionally there is a link error for _ReturnAddress.
# Nevertheless, the makefile is useful for testing changes from Linux,
# as it provides compiler feedback for syntax errors and warnings.
# =====

# Set compiler to mingw (can still override from command line)
CXX := i686-w64-mingw32-g++

SRCDIR := .
BUILDDIR := .build
BINDIR := $(BUILDDIR)/bin
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/obj
OUTPUT := op2ext.dll

CPPFLAGS := -DOP2EXT_INTERNAL_BUILD
CXXFLAGS := -std=c++17 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -shared -LSubmodules/Outpost2DLL/Lib/
LDLIBS := -lOutpost2DLL -lstdc++fs -lws2_32

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

SRCS := $(shell find $(SRCDIR) -maxdepth 1 -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
FOLDERS := $(sort $(dir $(SRCS)))

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	@mkdir -p ${@D}
	$(CXX) $^ $(LDFLAGS) -o $@ $(LDLIBS)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(DEPDIR)/%.d | build-folder
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

.PHONY:build-folder
build-folder:
	@mkdir -p $(patsubst $(SRCDIR)/%,$(OBJDIR)/%, $(FOLDERS))
	@mkdir -p $(patsubst $(SRCDIR)/%,$(DEPDIR)/%, $(FOLDERS))

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SRCS)))

.PHONY:clean, clean-deps, clean-all
clean:
	-rm -fr $(OBJDIR)
	-rm -fr $(DEPDIR)
	-rm -fr $(BINDIR)
	-rm -f $(OUTPUT)
clean-deps:
	-rm -fr $(DEPDIR)
clean-all:
	-rm -rf $(BUILDDIR)


GTESTDIR := $(BUILDDIR)/gtest

.PHONY:gtest
gtest:
	mkdir -p $(GTESTDIR)
	cd $(GTESTDIR) && cmake -DCMAKE_CXX_FLAGS="-std=c++17" /usr/src/gtest/
	make -C $(GTESTDIR)


TESTDIR := test
TESTOBJDIR := $(BUILDDIR)/testObj
TESTSRCS := #$(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TESTSRCS))
TESTFOLDERS := $(sort $(dir $(TESTSRCS)))
TESTLDFLAGS := -L./ -L$(GTESTDIR)
TESTLIBS := -lgtest -lgtest_main -lpthread -lstdc++fs
TESTOUTPUT := $(BUILDDIR)/testBin/runTests

TESTDEPFLAGS = -MT $@ -MMD -MP -MF $(TESTOBJDIR)/$*.Td
TESTCOMPILE.cpp = $(CXX) $(TESTDEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
TESTPOSTCOMPILE = @mv -f $(TESTOBJDIR)/$*.Td $(TESTOBJDIR)/$*.d && touch $@

.PHONY:check
check: $(TESTOUTPUT)
	cd test && ../$(TESTOUTPUT)

$(TESTOUTPUT): $(TESTOBJS) $(OUTPUT)
	@mkdir -p ${@D}
	$(CXX) $(TESTOBJS) $(TESTLDFLAGS) $(TESTLIBS) -o $@

$(TESTOBJS): $(TESTOBJDIR)/%.o : $(TESTDIR)/%.cpp $(TESTOBJDIR)/%.d | test-build-folder
	$(TESTCOMPILE.cpp) $(OUTPUT_OPTION) -I$(SRCDIR) $<
	$(TESTPOSTCOMPILE)

.PHONY:test-build-folder
test-build-folder:
	@mkdir -p $(patsubst $(TESTDIR)/%,$(TESTOBJDIR)/%, $(TESTFOLDERS))

$(TESTOBJDIR)/%.d: ;
.PRECIOUS: $(TESTOBJDIR)/%.d

include $(wildcard $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.d,$(TESTSRCS)))
