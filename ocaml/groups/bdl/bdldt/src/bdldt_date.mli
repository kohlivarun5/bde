type t

val make : year:int -> month:int -> day:int -> t

(* Returns tuple of year,month,day *)
val decompose : t -> (int * int * int)

(* Utility function to convert to string, so we can debug *)
val to_string : t -> string
