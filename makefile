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


CXXFLAGS := -std=c++17 -g -Wall -Wno-unknown-pragmas
LDFLAGS := -static-libgcc -static-libstdc++ -LOutpost2DLL/Lib/
LDLIBS := -lOutpost2DLL -lstdc++fs -lws2_32

op2extDll_CPPFLAGS := -DOP2EXT_INTERNAL_BUILD -IsrcStatic
op2extDll_LDFLAGS = $(LDFLAGS) -L$(dir $(op2extLib_OUTPUT))
op2extDll_LDLIBS = $(LDLIBS) -l$(basename $(notdir $(op2extLib_OUTPUT)))

all: op2extLib

# Debug
# $(info $(call DefineCppProject,op2extLib,op2ext.lib,srcStatic))
# $(info $(call DefineCppProject,op2extDll,op2ext.dll,srcDLL,op2extLib))
# $(info $(call DefineUnitTestProject,test,test,op2extLib))

$(eval $(call DefineCppProject,op2extLib,op2ext.lib,srcStatic))
$(eval $(call DefineCppProject,op2extDll,op2ext.dll,srcDLL,op2extLib))
$(eval $(call DefineUnitTestProject,test,test,op2extLib))


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
