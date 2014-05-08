
let () = 
  let date = Bdldt_date.make ~year:2014 ~month:12 ~day:20 in
  let (year,month,day) = Bdldt_date.decompose date in
  let s1 = Printf.sprintf "%d-%d-%d" year month day in
  let _  = Printf.printf "s1 : {%s}\n" s1 in
  let s2 = Bdldt_date.to_string date in
  let _  = Printf.printf "s2 : {%s}\n" s2 in
  let _ = Printf.printf 
            "The strings are %s\n" 
            (if s1 = s2 then "same" else "different") in
  ()
