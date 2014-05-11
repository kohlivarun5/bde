
type t = int

external bdldt_date_make 
  : year:int -> month:int -> day:int 
    -> int = "bdldt_date_make"
let make ~year ~month ~day = bdldt_date_make ~year ~month ~day


(* Returns tuple of year,month,day *)
external bdldt_date_decompose : int -> (int*int*int) = "bdldt_date_decompose"
let decompose t = bdldt_date_decompose t

(* Utility function to convert to string, so we can debug *)
let to_string t =
  let (year,month,day) = decompose t in 
  Printf.sprintf "%d-%d-%d" year month day

