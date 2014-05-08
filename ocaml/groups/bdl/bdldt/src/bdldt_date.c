#include <caml/memory.h>

#include <stdio.h>

extern "C" {

CAMLprim value
bdldt_date_make( value ml_year, value ml_month, value ml_day)
{
    CAMLparam3( ml_year, ml_month, ml_day);

    const int year  = Int_val(ml_year);
    const int month = Int_val(ml_month);
    const int day   = Int_val(ml_day);

    const int date_int = year + month + day;

    printf("Date is :{%d}   <<----->> %d:%s\n",date_int,__LINE__,__FILE__);

    return Val_int(date_int);
}

}
