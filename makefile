# To see a list of available make targets from bash, type:
# make <space> <tab> <tab>


include makefile-generic.mk


# Set compiler to mingw (can still override from command line)
config := mingw


CXXFLAGS := -std=c++17 -O2 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -static-libgcc -static-libstdc++
LDLIBS := -lstdc++fs -lws2_32

op2extDll_DEFINES := -DOP2EXT_INTERNAL_BUILD

# By default, compile and link both static library and dynamic link library
all: op2extLib op2extDll

$(eval $(call DefineCppProject,op2extLib,op2ext.lib,srcStatic/))
$(eval $(call DefineCppProject,op2extDll,op2ext.dll,srcDLL/,op2extLib))
$(eval $(call DefineCppProject,testModule,testModule.dll,TestModule/,op2extDll))
$(eval $(call DefineUnitTestProject,testLib,test/,op2extLib))
$(eval $(call DefineUnitTestProject,testDll,testDll/,op2extDll))


# Docker and CircleCI commands
$(eval $(call DefineDockerImage,.circleci/,outpostuniverse/gcc-mingw-wine-googletest-circleci,1.2))
$(eval $(call DefineCircleCi))


ifdef Outpost2Path

.PHONY: install run install-testModule run-testModule

install: $(Outpost2Path)op2ext.dll

$(Outpost2Path)op2ext.dll: op2ext.dll
	cp op2ext.dll "$(Outpost2Path)"

run: install
	wine "$(Outpost2Path)Outpost2.exe"


testModuleName := testModule
testModulePath := $(Outpost2Path)$(testModuleName)/
testModulePathFilename := $(testModulePath)op2mod.dll

install-testModule: $(testModulePathFilename)

$(testModulePathFilename): testModule.dll
	mkdir -p "$(testModulePath)"
	cp testModule.dll "$(testModulePathFilename)"

run-testModule: install-testModule
	wine "$(Outpost2Path)Outpost2.exe" /loadmod "$(testModuleName)"

endif
