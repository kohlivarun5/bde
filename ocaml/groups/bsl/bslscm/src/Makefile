LIB_NAME	= bslscm

SOURCES_MLI	= bslscm_version.mli
SOURCES_ML 	= bslscm_version.ml bslscm.ml
SOURCES_STUBS	= bslscm_version_stubs.cpp
SOURCES_STUBS_O=$(SOURCES_STUBS:.cpp=.o)

CPP_INC_FLAGS=$(BSL_HEADERS_WITH_I)

CPP_LIB_FLAGS=$(CPP_LIBSTD_FLAGS) -cclib $(BSL_LIB)

all: stubs libs top docs

stubs: Makefile $(SOURCES_STUBS_O)
	ocamlmklib -o $(LIB_NAME)_stubs $(SOURCES_STUBS_O) $(CPP_LIB_FLAGS)

libs:
	$(OCAMLC_CMD) $(SOURCES_MLI) $(SOURCES_ML)
	$(OCAMLOPT_CMD) $(SOURCES_ML)
	ocamlmklib -verbose -custom -o $(LIB_NAME) 		  \
		$(SOURCES_ML:.ml=.cmo) $(SOURCES_ML:.ml=.cmx) \
		$(CPP_LIB_FLAGS)
	ln -s -f $(LIB_NAME).a lib$(LIB_NAME).a

top:
	ocamlmktop -custom  -o $(LIB_NAME).top $(LIB_NAME).cma \
		./lib$(LIB_NAME)_stubs.a $(CPP_LIB_FLAGS)

docs:
	rm -rf ../docs
	mkdir ../docs
	ocamldoc -html -d ../docs $(SOURCES_MLI)

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a *.top *.out
	rm -rf ../docs

.cpp.o:
	gcc -v -g -Wall -c -fPIC -o $*.o $*.cpp $(CPP_INC_FLAGS) \
		-I$(OCAML_INCLUDE) $(CPP_INC_FLAGS)
