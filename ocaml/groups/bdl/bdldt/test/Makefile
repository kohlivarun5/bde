LIB_NAME	= bdldt

SOURCES_MLI	= 
SOURCES_ML 	= test_bdldt_date.ml

INCLUDE_PATH	= ../src/ 

WARN_ERROR_FLAGS = -warn-error -w -verbose
OCAMLC_CMD = ocamlc $(WARN_ERROR_FLAGS) -I $(INCLUDE_PATH)
OCAMLOPT_CMD = ocamlopt $(WARN_ERROR_FLAGS) -I $(INCLUDE_PATH)

LIBS = ../src/$(LIB_NAME).cma -cclib ../src/lib$(LIB_NAME)_stubs.a


all: libs

#stubs:
#	$(OCAMLC_CMD) $(SOURCES_STUBS)
#	ocamlmklib -o $(LIB_NAME)_stubs $(SOURCES_STUBS:.c=.o)

libs:
	echo "\n\n ===== Building libs\n"
	$(OCAMLC_CMD) $(LIBS) $(SOURCES_MLI) $(SOURCES_ML) -o test_$(LIB_NAME).byte.tsk
	$(OCAMLOPT_CMD) $(LIBS:.cma=.cmxa) $(SOURCES_ML) -o test_$(LIB_NAME).native.tsk
	echo "\n\n ===== Running Byte TESTS ========== \n\n"
	./test_$(LIB_NAME).byte.tsk
	echo "\n\n ===== Running Native TESTS ========== \n\n"
	./test_$(LIB_NAME).native.tsk

#top:
#	ocamlmktop -o $(LIB_NAME).top $(SOURCES) -cclib -L`pwd` -cclib -l./$(LIB_NAME).a
#docs:
#	rm -rf ../docs
#	mkdir ../docs
#	ocamldoc -html -d ../docs $(SOURCES_MLI)

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a *.top *.out *.tsk
