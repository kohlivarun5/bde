LIB_NAME	= bslscm

SOURCES_MLI	= 
SOURCES_ML 	= test_bslscm_version.ml

INCLUDE_PATH	= ../src/ 

WARN_ERROR_FLAGS = -warn-error -w -verbose
OCAMLOPT_CMD = ocamlopt $(WARN_ERROR_FLAGS) -I $(INCLUDE_PATH)

CPP_LIB_FLAGS= -cclib -L$(PWD)/../src -cclib $(BSL_LIB) -cclib -lstdc++ 

LIBS = ../src/$(LIB_NAME).cma -cclib ../src/lib$(LIB_NAME)_stubs.a $(CPP_LIB_FLAGS)


all: libs

libs:
	@echo "\n\n ===== Building libs\n"
	@$(OCAMLOPT_CMD) $(LIBS:.cma=.cmxa) $(SOURCES_ML) -o test_$(LIB_NAME).native.tsk
	echo "\n\n ===== Running TESTS ========== \n\n"
	@./test_$(LIB_NAME).native.tsk

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a *.top *.out *.tsk
