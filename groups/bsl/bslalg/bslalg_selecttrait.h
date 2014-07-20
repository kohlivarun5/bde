// bslalg_selecttrait.h                                               -*-C++-*-
#ifndef INCLUDED_BSLALG_SELECTTRAIT
#define INCLUDED_BSLALG_SELECTTRAIT

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a meta-function to select a suitable trait on a type.
//
//@CLASSES:
//  bslalg::SelectTrait: trait detection mechanism
//
//@SEE_ALSO:
//
//@DESCRIPTION: This component provides a meta-function, 'bslalg::SelectTrait',
// for selecting the most appropriate trait from a list of candidate traits for
// parameterized 'TYPE'.
//
///Usage
///-----

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLALG_HASTRAIT
#include <bslalg_hastrait.h>
#endif

#ifndef INCLUDED_BSLALG_TYPETRAITNIL
#include <bslalg_typetraitnil.h>
#endif

#ifndef INCLUDED_BSLMF_METAINT
#include <bslmf_metaint.h>
#endif

#ifndef INCLUDED_BSLMF_SWITCH
#include <bslmf_switch.h>
#endif

namespace BloombergLP {

namespace bslalg {

                          // =================
                          // class SelectTrait
                          // =================

template <typename TYPE,
          typename TRAIT1,
          typename TRAIT2 = TypeTraitNil,
          typename TRAIT3 = TypeTraitNil,
          typename TRAIT4 = TypeTraitNil,
          typename TRAIT5 = TypeTraitNil>
struct SelectTrait {
    // Select one trait out of several that the parameterized type 'T' may
    // possess.  If 'T' has the parameterized trait 'TRAIT1', then the nested
    // 'Type' is 'TRAIT1', else if 'T' has an optionally parameterized
    // 'TRAIT2', then 'Type' is 'TRAIT2', etc.  Also computes an integral
    // selection constant and meta-value.  If 'T' has none of the parameterized
    // 'TRAIT*', then the nested 'Type' is 'TypeTraitNil'.

    // PUBLIC TYPES
    enum {
        SELECTION = (HasTrait<TYPE, TRAIT1>::VALUE ? 1 :
                     HasTrait<TYPE, TRAIT2>::VALUE ? 2 :
                     HasTrait<TYPE, TRAIT3>::VALUE ? 3 :
                     HasTrait<TYPE, TRAIT4>::VALUE ? 4 :
                     HasTrait<TYPE, TRAIT5>::VALUE ? 5 :
                     0)
            // Integral value indicating which trait was selected: 1 for
            // 'TRAIT1', 2 for 'TRAIT2', etc. and 0 if none were selected.
    };

    typedef bslmf::MetaInt<SELECTION> MetaSelection;
        // A meta-value representing the same thing a 'SELECTION', but as a
        // type instead of as an integer.

    typedef typename bslmf::Switch<SELECTION,
                                   TypeTraitNil,
                                   TRAIT1,
                                   TRAIT2,
                                   TRAIT3,
                                   TRAIT4,
                                   TRAIT5>::Type Type;
        // The actual trait that was selected, or 'TypeTraitNil' if no trait
        // was selected.
};

}  // close package namespace


}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg Finance L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
