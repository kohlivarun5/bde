LIB_NAME	= bslscm

SOURCES_MLI	= bslscm_version.mli
SOURCES_ML 	= bslscm_version.ml bslscm.ml
SOURCES_STUBS	= bslscm_version.cpp
SOURCES_STUBS_O=$(SOURCES_STUBS:.cpp=.o)

WARN_ERROR_FLAGS = -warn-error -w -verbose
OCAMLC_CMD = ocamlc $(WARN_ERROR_FLAGS) -c
OCAMLOPT_CMD = ocamlopt $(WARN_ERROR_FLAGS) -c

OCAMLLIB=`ocamlc -where`

OCAML_INCLUDE=$(OCAMLLIB)


CPP_INC_FLAGS=$(BSL_HEADERS_WITH_I)

#The following is hard coded because BSL_HEADERS_WITH_I is not getting picked from environment for some reason.

# CPP_INC_FLAGS= -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bsl+bslhdrs/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bsl+stdhdrs/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bslalg/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bsldoc/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bslim/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bslma/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bslmf/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bsls/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bslscm/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bslstl/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/bsltf/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/doc/ -I/Users/varunkohli/SkyDrive/GitHub/bde/ocaml/..//groups/bsl/group/

CPP_LIB_FLAGS= -cclib -L`pwd`/ -cclib -lstdc++ -cclib $(BSL_LIB)

all: stubs libs top docs

stubs: Makefile $(SOURCES_STUBS_O)
	ocamlmklib -o $(LIB_NAME)_stubs $(SOURCES_STUBS_O) $(CPP_LIB_FLAGS)

libs:
	$(OCAMLC_CMD) $(SOURCES_MLI) $(SOURCES_ML)
	$(OCAMLOPT_CMD) $(SOURCES_ML)
	ocamlmklib -verbose -custom -o $(LIB_NAME) $(SOURCES_ML:.ml=.cmo) $(SOURCES_ML:.ml=.cmx) $(CPP_LIB_FLAGS)
	ln -s -f $(LIB_NAME).a lib$(LIB_NAME).a

top:
	ocamlmktop -custom  -o $(LIB_NAME).top $(LIB_NAME).cma ./lib$(LIB_NAME)_stubs.a $(CPP_LIB_FLAGS)

docs:
	rm -rf ../docs
	mkdir ../docs
	ocamldoc -html -d ../docs $(SOURCES_MLI)

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a *.top *.out
	rm -rf ../docs

.cpp.o:
	gcc -v -g -Wall -c -fPIC -o $*.o $*.cpp $(CPP_INC_FLAGS) -I$(OCAML_INCLUDE) $(CPP_INC_FLAGS)
