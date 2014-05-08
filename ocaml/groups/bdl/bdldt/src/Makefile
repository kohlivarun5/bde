SOURCES = bdldt_date.mli bdldt_date.ml

all: libs top docs

libs:
	ocamlc -warn-error -c $(SOURCES)
	ocamlopt -warn-error -c $(SOURCES)
	ocamlmklib -v -o bdldt_date bdldt_date.cmo bdldt_date.cmx

top:
	ocamlmktop -o bdldt.top $(SOURCES) -cclib -L`pwd` -cclib -l./bdldt_date.a 
docs:
	rm -rf ../docs
	mkdir ../docs
	ocamldoc -html -d ../docs bdldt_date.mli

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a *.top *.out
	rm -rf ../docs
