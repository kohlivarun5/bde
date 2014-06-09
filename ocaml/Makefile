
BDE_SOURCE_ROOT=$(PWD)/../
BDE_BUILD_DIR=$(BDE_SOURCE_ROOT)/build/Debug/

BSL_HEADERS=$(shell \ls -d1 $(BDE_SOURCE_ROOT)/groups/bsl/*/)

BSL_HEADERS_WITH_I=$(addprefix -I, $(BSL_HEADERS))

BSL_LIB=$(BDE_BUILD_DIR)/groups/bsl/libbsl.a

all:
	$(MAKE) -e -C ./groups/bsl/bslscm/src/
