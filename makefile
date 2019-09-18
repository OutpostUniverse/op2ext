# To see a list of available make targets from bash, type:
# make <space> <tab> <tab>

# =====
# Note: MinGW is not able to link the DLL project due to name mangling
# differences in the Outpost2DLL imported symbols. It is however able to
# compile and check the source code for errors, including compiling and linking
# of the static library project, and the unit test project. This means much of
# the code can be checked for errors on Linux, even if a full compile is not
# (yet) possible.
# =====


include makefile-generic.mk


# Set compiler to mingw (can still override from command line)
config := mingw


CXXFLAGS := -std=c++17 -O2 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -static-libgcc -static-libstdc++ -LOutpost2DLL/Lib/
LDLIBS := -lOutpost2DLL -lstdc++fs -lws2_32

op2extDll_DEFINES := -DOP2EXT_INTERNAL_BUILD

# By default, compile and link static library, and compile (but not link) DLL
all: op2extLib op2extDll

$(eval $(call DefineCppProject,op2extLib,op2ext.lib,srcStatic/))
$(eval $(call DefineCppProject,op2extDll,op2ext.dll,srcDLL/,op2extLib))
$(eval $(call DefineUnitTestProject,test,test/,op2extLib))


# Docker and CircleCI commands
$(eval $(call DefineDockerImage,.circleci/,outpostuniverse/gcc-mingw-wine-googletest-circleci,1.2))
$(eval $(call DefineCircleCi))


ifdef Outpost2Path

.PHONY: install run

install: $(Outpost2Path)op2ext.dll

$(Outpost2Path)op2ext.dll: op2ext.dll
	cp op2ext.dll "$(Outpost2Path)"

run: install
	wine "$(Outpost2Path)Outpost2.exe"

endif
