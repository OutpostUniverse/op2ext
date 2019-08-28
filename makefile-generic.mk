
# Implements generic declarative project syntax, used to build a wide
# variety of projects, with dependency tracking, and out of source builds.
#
# Example syntax:
#   include makefile-generic.mk
#
#   projectName_CXXFLAGS := -g
#
#   $(eval $(call DefineCppProject,projectName,outputFile.lib,sourceFolder))


# Default build and clean targets
.PHONY: default all clean clean-all source
.DEFAULT_GOAL := all
default: all


#### Compiler configurations ####
# Allow for easily setting a compiler toolchain (gcc, clang, mingw)
# Example (from main Makefile):
#   config := mingw
# Example (from command line, overrides main file):
#   make config=mingw

# Inherit from environment
default_CXX := $(CXX)
default_CC := $(CC)

gcc_CXX := g++
gcc_CC := gcc

clang_CXX := clang++
clang_CC := clang

mingw_CXX := i686-w64-mingw32-g++
mingw_CC := i686-w64-mingw32-gcc


# Set default config name
config ?= default

# Set global variables based on selected config name
CXX = $($(config)_CXX)
CC = $($(config)_CC)


#### General compile rules ####
# These rules are for auto depedency tracked rebuilds (with out of source build support).
# These rules assume dependencies and flags are set elsewhere.
# See `DefineCppProject` macro for defining project dependencies.

# Variables to track dependencies
DEPNAME = $(patsubst %.o,%.d,$@)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPNAME).temp
POSTCOMPILE = mv -f $(DEPNAME).temp $(DEPNAME) && touch $@

# Variable to create missing output folders
MKDIR = mkdir -p "$(@D)"

# Variable with main C++ compile rule (with dependency generation)
COMPILE.cpp = $(CXX) $(OUTPUT_OPTION) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c $<

# Output coloring variables
Green := \e[1;32m
White := \e[0m

## Rules to build various final target ouputs ##

%.exe:
	@$(MKDIR)
	$(CXX) -o "$@" $^ $(LDFLAGS) $(LDLIBS)

%.dll:
	@$(MKDIR)
	$(CXX) -o "$@" $^ -shared $(LDFLAGS) $(LDLIBS)

%.lib:
	@$(MKDIR)
	ar rcs "$@" $^

# Rule to build intermediate project files
%.o:
	@$(MKDIR)
	$(COMPILE.cpp)
	@$(POSTCOMPILE)

# Ensure missing dependency files don't generate a warning
# (Missing dependency files will cause a rebuild of the associated source file)
.PRECIOUS: %.d
%.d: ;


#### Project auto dependency support ####
# This allows a project name and source directory to be defined, and then
# searches for all source files and creates a dependency graph and default
# project build and clean targets.
# This assumes project specific flags (or default flags) are defined externally.


# Set default build directory
BUILDDIR ?= .build

# DefineProject(ProjectName, Output, SourceFolder)
# Uses project specific flags (or global defaults without "ProjectName_" prefix):
#   ProjectName_CPPFLAGS
#   ProjectName_CXXFLAGS
#   ProjectName_LDFLAGS
#   ProjectName_LDLIBS
define DefineCppProject =

## Define project specific variables ##

# Project specific flags
$(1)_CPPFLAGS ?= $(CPPFLAGS)
$(1)_CXXFLAGS ?= $(CXXFLAGS)
$(1)_LDFLAGS ?= $(LDFLAGS)
$(1)_LDLIBS ?= $(LDLIBS)

# Project specific source folder, intermediate folder, and output file
$(1)_INTDIR := $(BUILDDIR)/$(config)/$(1)
$(1)_OUTPUT := $(2)
$(1)_SRCDIR := $(3)

# Project specific source files (directory scan), and associated object and dependency files
$(1)_SRCS := $$(shell find $$($(1)_SRCDIR) -name '*.cpp')
$(1)_OBJS := $$(patsubst $$($(1)_SRCDIR)/%.cpp,$$($(1)_INTDIR)/%.o,$$($(1)_SRCS))
$(1)_DEPS := $$(patsubst %.o,%.d,$$($(1)_OBJS))

## Project specific build rules and dependencies ##

# Define high level targets (project name, not actual output files)
.PHONY: $(1) clean-$(1) clean-all-$(1) show-$(1)

# Project specific build rule (for actual output file)
$(1): $$($(1)_OUTPUT)
# Project build rules inherit project specific variables
$(1): CPPFLAGS := $$($(1)_CPPFLAGS)
$(1): CXXFLAGS := $$($(1)_CXXFLAGS)
$(1): LDFLAGS := $$($(1)_LDFLAGS)
$(1): LDLIBS := $$($(1)_LDLIBS)

# Output file depends on all object files (true for .exe, .dll, and .lib)
$$($(1)_OUTPUT): $$($(1)_OBJS)

# Object files depend on source and dependency files
$$($(1)_OBJS): $$($(1)_INTDIR)/%.o: $$($(1)_SRCDIR)/%.cpp $$($(1)_INTDIR)/%.d

# Include all generated dependency info
# (The `wildcard` will filter out files that don't exist to avoid warnings)
include $$(wildcard $$($(1)_DEPS))


## Project specific clean rules ##
clean-$(1):
	rm -fr "$$($(1)_INTDIR)"
clean-all-$(1): clean-$(1)
	rm -f "$$($(1)_OUTPUT)"

# Master clean rules clean this project
clean: clean-$(1)
clean-all: clean-all-$(1)


# Simple rule to show source files found in scan of project folder
source-$(1):
	@echo -n "\nProject $$(Green)$(1)$$(White) source files: \n  "
	@echo "$$($(1)_SRCS)" | sed -e 's/ /\n  /g'

# Show source files for every configured project
source: source-$(1)

endef


#### Google Test support ####
# This allows for doing a local build of the Google Test library.
# Provides rules for doing a system install of the resulting binary.
# Assumes standard install location of source files, as found on Ubuntu.
# Google test source files can be installed with Apt.

gtest_CXXFLAGS := -std=c++17
gtest_SRCDIR := /usr/src/googletest/
gtest_INCDIR := /usr/src/googletest/googletest/include/
gtest_BUILDDIR = $(BUILDDIR)/$(config)/gtest/

# Toolchain specifc library install folders
gtest_gcc_LIBDIR := /usr/lib/
gtest_clang_LIBDIR := /usr/lib/
gtest_mingw_LIBDIR := /usr/i686-w64-mingw32/lib/
# Select based on configured toolchain
gtest_LIBDIR = $(gtest_$(config)_LIBDIR)

# Toolchain specific compile options
gtest_gcc_DEFINES :=
gtest_clang_DEFINES :=
gtest_mingw_DEFINES := -DCMAKE_SYSTEM_NAME="Windows" -Dgtest_disable_pthreads=ON
# Select based on configured toolchain
gtest_DEFINES = $(gtest_$(config)_DEFINES)

# To ease configuring test projects to use a local build
gtest_LOCALBUILD_LDFLAGS = -L$(gtest_BUILDDIR)googlemock/ -L$(gtest_BUILDDIR)googlemock/gtest/
gtest_LINK_LDLIBS := -lgtest -lgtest_main

.PHONY: install-source-gtest gtest install-gtest clean-gtest

install-source-gtest:
	apt install googletest

gtest:
	mkdir -p "$(gtest_BUILDDIR)"
	cd "$(gtest_BUILDDIR)" && cmake -DCMAKE_CXX_COMPILER="$(CXX)" -DCMAKE_C_COMPILER="$(CC)" -DCMAKE_CXX_FLAGS="$(gtest_CXXFLAGS)" $(gtest_DEFINES) "$(gtest_SRCDIR)"
	make -C "$(gtest_BUILDDIR)"

install-gtest:
	cp $(gtest_BUILDDIR)googlemock/gtest/lib*.a "$(gtest_LIBDIR)"
	cp $(gtest_BUILDDIR)googlemock/lib*.a "$(gtest_LIBDIR)"

clean-gtest:
	rm -rf "$(gtest_BUILDDIR)"
