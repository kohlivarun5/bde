
type t = int

external bdldt_date_make 
  : year:int -> month:int -> day:int 
    -> int = "bdldt_date_make"

let make ~year ~month ~day = bdldt_date_make ~year ~month ~day
