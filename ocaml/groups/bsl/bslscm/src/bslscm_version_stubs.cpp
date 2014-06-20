#include <caml/alloc.h>

#include <bslscm_version.h>

extern "C" {

CAMLprim value
bslscm_version_version( value unit )
{
    return caml_copy_string(BloombergLP::bslscm::Version::version());
}

}//extern "C"
