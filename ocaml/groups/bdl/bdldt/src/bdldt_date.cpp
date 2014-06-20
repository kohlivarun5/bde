#include <caml/memory.h>
#include <caml/alloc.h>

extern "C" {

CAMLprim value
bdldt_date_make(value ml_year, value ml_month, value ml_day)
{
    int year  = Int_val (ml_year);
    int month = Int_val (ml_month);
    int day   = Int_val (ml_day);

    int date_int = (year*10000) + (month*100) + day;

    return Val_int(date_int);
}

CAMLprim value
bdldt_date_decompose(value ml_date_int)
{
    CAMLparam1 (ml_date_int);
    CAMLlocal1 (ml_tuple);

    int date_int = Int_val (ml_date_int);
    ml_tuple = caml_alloc (3, 0);
    int year = date_int / 10000;
    int month = (date_int % 10000) / 100;
    int day = (date_int % 100);

    Store_field( ml_tuple, 0, Val_int (year));
    Store_field( ml_tuple, 1, Val_int (month));
    Store_field( ml_tuple, 2, Val_int (day));

    CAMLreturn( ml_tuple );
}

}//extern "C"
