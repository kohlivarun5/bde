#include <caml/memory.h>
#include <caml/alloc.h>

#include <stdio.h>

extern "C" {

CAMLprim value
bdldt_date_make(value ml_year, value ml_month, value ml_day)
{
    const int year  = Int_val(ml_year);
    const int month = Int_val(ml_month);
    const int day   = Int_val(ml_day);

    const int date_int = (year*10000) + (month*100) + day;

    return Val_int(date_int);
}

CAMLprim value
bdldt_date_decompose(value ml_date_int)
{
    const int date_int = Int_val(ml_date_int);

    CAMLparam1( ml_date_int );
    CAMLlocal1( ml_tuple );
    ml_tuple = caml_alloc(3, 0);

    const int year = date_int / 10000;
    const int month = (date_int % 10000) / 100;
    const int day = (date_int % 100);

    Store_field( ml_tuple, 0, year);
    Store_field( ml_tuple, 1, month);
    Store_field( ml_tuple, 2, day);

    CAMLreturn( ml_tuple );
}


}
