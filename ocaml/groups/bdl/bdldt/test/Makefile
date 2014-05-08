all: 
	ocamlc -c test_bdldt_date.ml
	ocamlopt -c test_bdldt_date.ml

docs:
	rm -rf docs
	mkdir docs
	ocamlfind ocamldoc -html -d docs test_bdldt_date.mli

clean:
	rm -f *.cmi *.cmxa *.cma *.cmx *.cmo *.o *.so *.a
