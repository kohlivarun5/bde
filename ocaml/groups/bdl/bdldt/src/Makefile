all: 
	ocamlc -c bdldt_date.mli bdldt_date.ml
	ocamlopt -c bdldt_date.ml
	ocamlmklib -v -o bdldt_date bdldt_date.cmo bdldt_date.cmx

docs:
	rm -rf docs
	mkdir docs
	ocamlfind ocamldoc -html -d docs bdldt_date.mli

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a
