LIB_NAME	= bdldt

SOURCES_MLI	= bdldt_date.mli
SOURCES_ML 	= bdldt_date.ml
SOURCES_STUBS	= bdldt_date.c


WARN_ERROR_FLAGS = -warn-error -w -verbose
OCAMLC_CMD = ocamlc $(WARN_ERROR_FLAGS) -c
OCAMLOPT_CMD = ocamlopt $(WARN_ERROR_FLAGS) -c


all: stubs libs top docs

stubs:
	$(OCAMLC_CMD) $(SOURCES_STUBS)
	ocamlmklib -o $(LIB_NAME)_stubs $(SOURCES_STUBS:.c=.o)

libs:
	$(OCAMLC_CMD) $(SOURCES_MLI) $(SOURCES_ML)
	$(OCAMLOPT_CMD) $(SOURCES_ML)
	ocamlmklib -verbose -custom -o $(LIB_NAME) $(SOURCES_ML:.ml=.cmo) $(SOURCES_ML:.ml=.cmx) -cclib -L`pwd`/ 
	ln -s $(LIB_NAME).a lib$(LIB_NAME).a

top:
	ocamlmktop -custom  -o $(LIB_NAME).top $(LIB_NAME).cma ./lib$(LIB_NAME)_stubs.a

docs:
	rm -rf ../docs
	mkdir ../docs
	ocamldoc -html -d ../docs $(SOURCES_MLI)

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a *.top *.out
	rm -rf ../docs
