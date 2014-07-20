// bdldfp_decimalutil.cpp                                             -*-C++-*-
#include <bdldfp_decimalutil.h>

#include <bdldfp_decimalconvertutil.h>

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id$")

#include <bdldfp_decimalplatform.h>

#include <bsl_cmath.h>
#include <bsls_assert.h>

#include <errno.h>
#include <math.h>  // For the  FP_* macros

#if BDLDFP_DECIMALPLATFORM_DECNUMBER
extern "C" {
#include <decSingle.h>
}
#endif

#if BDLDFP_DECIMALPLATFORM_C99_TR
#  ifndef  __STDC_WANT_DEC_FP__
#    error __STDC_WANT_DEC_FP__ must be defined on the command line!
     char die[-42];     // if '#error' unsupported
#  endif
#endif

namespace BloombergLP {
namespace bdldfp {

namespace {

                    // ===============
                    // class BufferBuf
                    // ===============

template <int Size>
class BufferBuf : public bsl::streambuf {
    // A static (capacity) stream buffer helper

    char d_buf[Size + 1];  // Text plus closing NUL character

  public:
    // CREATORS
    BufferBuf();
        // Create an empty 'BufferBuf'.

    // MANIPULATORS
    void reset();
        // Clear this buffer (make it empty).

    const char *str();
        // Return a pointer to a non-modifiable, NUL-terminated string of
        // characters that is the content of this buffer.
};
                    // ---------------
                    // class BufferBuf
                    // ---------------

template <int Size>
BufferBuf<Size>::BufferBuf()
{
    reset();
}

template <int Size>
void BufferBuf<Size>::reset()
{
    this->setp(this->d_buf, this->d_buf + Size);
}

template <int Size>
const char *BufferBuf<Size>::str()
{
    *this->pptr() = 0; return this->pbase();
}

                      // parse helper functions

namespace {

int parseDecimal(Decimal32 *o, const char *s)
{
    return DecimalUtil::parseDecimal32(o,s);
}

int parseDecimal(Decimal64 *o, const char *s)
{
    return DecimalUtil::parseDecimal64(o,s);
}

int parseDecimal(Decimal128 *o, const char *s)
{
    return DecimalUtil::parseDecimal128(o,s);
}

}  // close unnamed namespace

template <class DECIMAL_TYPE, class COEFFICIENT_TYPE>
inline
DECIMAL_TYPE makeDecimal(COEFFICIENT_TYPE coeff, int exponent)
{
    if (exponent > bsl::numeric_limits<DECIMAL_TYPE>::max_exponent) {
        errno = ERANGE;
        if (coeff < 0) {
            return bsl::numeric_limits<DECIMAL_TYPE>::infinity();     // RETURN
        }
        else {
            return -bsl::numeric_limits<DECIMAL_TYPE>::infinity();    // RETURN
        }
    }

    // TODO: TBD we should not convert through strings - it should be possible
    // to convert directly
    BufferBuf<48> bb;
    bsl::ostream out(&bb);
    out.imbue(bsl::locale::classic());
    out << coeff << 'e' << exponent;

    DECIMAL_TYPE rv;
    int parseResult = parseDecimal(&rv, bb.str());
    BSLS_ASSERT(0 == parseResult);
    return rv;
}

}  // close unnamed namespace

                             // Creator functions


int DecimalUtil::parseDecimal32(Decimal32 *out, const char *str)
{
    BSLS_ASSERT(out != 0);
    BSLS_ASSERT(str != 0);

    *out = DecimalImplUtil::parse32(str);
    return 0;
}

int DecimalUtil::parseDecimal64(Decimal64 *out, const char *str)
{

    BSLS_ASSERT(out != 0);
    BSLS_ASSERT(str != 0);

    *out = DecimalImplUtil::parse64(str);
    return 0;
}

int DecimalUtil::parseDecimal128(Decimal128 *out, const char *str)
{
    BSLS_ASSERT(out != 0);
    BSLS_ASSERT(str != 0);

    *out = DecimalImplUtil::parse128(str);
    return 0;
}


int DecimalUtil::parseDecimal32(Decimal32 *out, const bsl::string& str)
{
    BSLS_ASSERT(out != 0);

    return parseDecimal32(out, str.c_str());
}
int DecimalUtil::parseDecimal64(Decimal64 *out, const bsl::string& str)
{
    BSLS_ASSERT(out != 0);

    return parseDecimal64(out, str.c_str());
}
int DecimalUtil::parseDecimal128(Decimal128 *out, const bsl::string& str)
{
    BSLS_ASSERT(out != 0);

    return parseDecimal128(out, str.c_str());
}

                                // Math functions

Decimal64 DecimalUtil::fma(Decimal64 x, Decimal64 y, Decimal64 z)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR && BDLDFP_DECIMALPLATFORM_C99_NO_FMAD64
    // TODO TBD Is this OK?  Why don't we have fmad64 on IBM???
    return (x.value() * y.value()) + z.value();
#elif BDLDFP_DECIMALPLATFORM_C99_TR
    return fmad64(x.value(), y.value(), z.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 rv;
    decDoubleFMA(rv.data(),
                 x.data(),
                 y.data(),
                 z.data(),
                 DecimalImplUtil::getDecNumberContext());
    return rv;
#endif
}

Decimal128 DecimalUtil::fma(Decimal128 x, Decimal128 y, Decimal128 z)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fmad128(x.value(), y.value(), z.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal128 rv;
    decQuadFMA(rv.data(),
               x.data(),
               y.data(),
               z.data(),
               DecimalImplUtil::getDecNumberContext());
    return rv;
#endif
}
                       // Selecting, converting functions

Decimal32 DecimalUtil::fabs(Decimal32 value)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fabsd32(value.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    // TODO TBD Just flip the sign bit, but beware of endianness
    Decimal64 in(value);
    Decimal64 out;
    decDoubleAbs(out.data(),
                 in.data(),
                 DecimalImplUtil::getDecNumberContext());
    return Decimal32(out);
#endif
}
Decimal64 DecimalUtil::fabs(Decimal64 value)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fabsd64(value.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 rv;
    decDoubleAbs(rv.data(),
                 value.data(),
                 DecimalImplUtil::getDecNumberContext());
    return rv;
#endif
}
Decimal128 DecimalUtil::fabs(Decimal128 value)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fabsd128(value.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal128 rv;
    decQuadAbs(rv.data(),
               value.data(),
               DecimalImplUtil::getDecNumberContext());
    return rv;
#endif
}
                        // classification functions

#if BDLDFP_DECIMALPLATFORM_DECNUMBER
static int deClass2FP_(enum decClass cl)
{
    switch (cl) {
    case DEC_CLASS_SNAN:
    case DEC_CLASS_QNAN:          return FP_NAN;                      // RETURN
    case DEC_CLASS_NEG_INF:
    case DEC_CLASS_POS_INF:       return FP_INFINITE;                 // RETURN
    case DEC_CLASS_NEG_ZERO:
    case DEC_CLASS_POS_ZERO:      return FP_ZERO;                     // RETURN
    case DEC_CLASS_NEG_NORMAL:
    case DEC_CLASS_POS_NORMAL:    return FP_NORMAL;                   // RETURN
    case DEC_CLASS_NEG_SUBNORMAL:
    case DEC_CLASS_POS_SUBNORMAL: return FP_SUBNORMAL;                // RETURN
    }
    BSLS_ASSERT(!"Unknown decClass");
    return -1;
}
#endif

int DecimalUtil::classify(Decimal32 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fpclassify(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    enum decClass cl = decSingleClass(x.data());
    return deClass2FP_(cl);
#endif
}
int DecimalUtil::classify(Decimal64 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fpclassify(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    enum decClass cl = decDoubleClass(x.data());
    return deClass2FP_(cl);
#endif
}
int DecimalUtil::classify(Decimal128 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return fpclassify(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    enum decClass cl = decQuadClass(x.data());
    return deClass2FP_(cl);
#endif
}

bool DecimalUtil::isNan(Decimal32 x)
{
    return classify(x) == FP_NAN;
}
bool DecimalUtil::isNan(Decimal64 x)
{
    return classify(x) == FP_NAN;
}
bool DecimalUtil::isNan(Decimal128 x)
{
    return classify(x) == FP_NAN;
}

bool DecimalUtil::isInf(Decimal32 x)
{
    return classify(x) == FP_INFINITE;
}
bool DecimalUtil::isInf(Decimal64 x)
{
    return classify(x) == FP_INFINITE;
}
bool DecimalUtil::isInf(Decimal128 x)
{
    return classify(x) == FP_INFINITE;
}

bool DecimalUtil::isFinite(Decimal32 x)
{
    int cl = classify(x);
    return cl != FP_INFINITE && cl != FP_NAN;
}
bool DecimalUtil::isFinite(Decimal64 x)
{
    int cl = classify(x);
    return cl != FP_INFINITE && cl != FP_NAN;
}
bool DecimalUtil::isFinite(Decimal128 x)
{
    int cl = classify(x);
    return cl != FP_INFINITE && cl != FP_NAN;
}

bool DecimalUtil::isNormal(Decimal32 x)
{
    return classify(x) == FP_NORMAL;
}
bool DecimalUtil::isNormal(Decimal64 x)
{
    return classify(x) == FP_NORMAL;
}
bool DecimalUtil::isNormal(Decimal128 x)
{
    return classify(x) == FP_NORMAL;
}

                           // Comparison functions

bool DecimalUtil::isUnordered(Decimal32 x, Decimal32 y)
{
    return isNan(x) || isNan(y);
}
bool DecimalUtil::isUnordered(Decimal64 x, Decimal64 y)
{
    return isNan(x) || isNan(y);
}
bool DecimalUtil::isUnordered(Decimal128 x, Decimal128 y)
{
    return isNan(x) || isNan(y);
}
                             // Rounding functions

Decimal32 DecimalUtil::ceil(Decimal32 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return ceild32(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 xw(x);
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             xw.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_CEILING);
    return Decimal32(rv);
#endif
}

Decimal64 DecimalUtil::ceil(Decimal64 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return ceild64(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             x.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_CEILING);
    return rv;
#endif
}

Decimal128 DecimalUtil::ceil(Decimal128 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return ceild128(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal128 rv;
    decQuadToIntegralValue(rv.data(),
                           x.data(),
                           DecimalImplUtil::getDecNumberContext(),
                           DEC_ROUND_CEILING);
    return rv;
#endif
}

Decimal32 DecimalUtil::floor(Decimal32 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return floord32(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 xw(x);
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             xw.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_FLOOR);
    return Decimal32(rv);
#endif
}

Decimal64 DecimalUtil::floor(Decimal64 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return floord64(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             x.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_FLOOR);
    return rv;
#endif
}

Decimal128 DecimalUtil::floor(Decimal128 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return floord128(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal128 rv;
    decQuadToIntegralValue(rv.data(),
                           x.data(),
                           DecimalImplUtil::getDecNumberContext(),
                           DEC_ROUND_FLOOR);
    return rv;
#endif
}

Decimal32 DecimalUtil::trunc(Decimal32 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return truncd32(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 xw(x);
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             xw.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_DOWN);
    return Decimal32(rv);
#endif
}

Decimal64 DecimalUtil::trunc(Decimal64 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return truncd64(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             x.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_DOWN);
    return rv;
#endif
}

Decimal128 DecimalUtil::trunc(Decimal128 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return truncd128(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal128 rv;
    decQuadToIntegralValue(rv.data(),
                           x.data(),
                           DecimalImplUtil::getDecNumberContext(),
                           DEC_ROUND_DOWN);
    return rv;
#endif
}

Decimal32 DecimalUtil::round(Decimal32 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return roundd32(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 xw(x);
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             xw.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_HALF_UP);
    return Decimal32(rv);
#endif
}

Decimal64 DecimalUtil::round(Decimal64 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return roundd64(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal64 rv;
    decDoubleToIntegralValue(rv.data(),
                             x.data(),
                             DecimalImplUtil::getDecNumberContext(),
                             DEC_ROUND_HALF_UP);
    return rv;
#endif
}

Decimal128 DecimalUtil::round(Decimal128 x)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return roundd128(x.value());
#elif BDLDFP_DECIMALPLATFORM_DECNUMBER
    Decimal128 rv;
    decQuadToIntegralValue(rv.data(),
                           x.data(),
                           DecimalImplUtil::getDecNumberContext(),
                           DEC_ROUND_HALF_UP);
    return rv;
#endif
}

                             // Quantum functions

Decimal64 DecimalUtil::multiplyByPowerOf10(Decimal64 value, int exponent)
{
    BSLS_ASSERT(-1999999997 <= exponent);
    BSLS_ASSERT(               exponent <= 99999999);

#if BDLDFP_DECIMALPLATFORM_C99_TR
    return scalblnd64(*value.data(), exponent);
#else
    long long longLongExponent = exponent;
    Decimal64 result = value;
    decDoubleScaleB(result.data(),
                    value.data(),
                    makeDecimal64(longLongExponent, 0).data(),
                    DecimalImplUtil::getDecNumberContext());
    return result;
#endif
}

Decimal64 DecimalUtil::multiplyByPowerOf10(Decimal64 value, Decimal64 exponent)
{
    BSLS_ASSERT_SAFE(
      makeDecimal64(-1999999997, 0) <= exponent);
    BSLS_ASSERT_SAFE(                  exponent <= makeDecimal64(99999999, 0));

#if BDLDFP_DECIMALPLATFORM_C99_TR
    const int intExponent = __d64_to_long_long(*exponent.data());
    return scalblnd64(*value.data(), intExponent);
#else
    Decimal64 result = value;
    decDoubleScaleB(result.data(),
                    value.data(),
                    exponent.data(),
                    DecimalImplUtil::getDecNumberContext());
    return result;
#endif
}

Decimal128 DecimalUtil::multiplyByPowerOf10(Decimal128 value, int exponent)
{
    BSLS_ASSERT(-1999999997 <= exponent);
    BSLS_ASSERT(               exponent <= 99999999);

#if BDLDFP_DECIMALPLATFORM_C99_TR
    return scalblnd128(*value.data(), exponent);
#else
    Decimal128 result = value;
    DecimalImplUtil::ValueType128 scale =
                               DecimalImplUtil::makeDecimalRaw128(exponent, 0);
    decQuadScaleB(result.data(),
                  value.data(),
                  &scale,
                  DecimalImplUtil::getDecNumberContext());
    return result;
#endif
}

Decimal128 DecimalUtil::multiplyByPowerOf10(Decimal128 value,
                                            Decimal128 exponent)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    const int intExponent = __d128_to_long_long(*exponent.data());
    return scalblnd128(*value.data(), intExponent);
#else
    Decimal128 result = value;
    decQuadScaleB(result.data(),
                  value.data(),
                  exponent.data(),
                  DecimalImplUtil::getDecNumberContext());
    return result;
#endif
}

Decimal64 DecimalUtil::quantize(Decimal64 value, Decimal64 exponent)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return quantized64(*value.data(), *exponent.data());
#else
    Decimal64 result = value;
    decDoubleQuantize(result.data(),
                      value.data(),
                      exponent.data(),
                      DecimalImplUtil::getDecNumberContext());
    return result;
#endif
}

Decimal128 DecimalUtil::quantize(Decimal128 x, Decimal128 y)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return quantized128(*x.data(), *y.data());
#else
    Decimal128 rv = x;
    decQuadQuantize(rv.data(),
                    x.data(),
                    y.data(),
                    DecimalImplUtil::getDecNumberContext());
    return rv;
#endif
}

int DecimalUtil::quantum(Decimal64 x)
{
    BSLS_ASSERT(!isInf(x));
    BSLS_ASSERT(!isNan(x));
#if BDLDFP_DECIMALPLATFORM_C99_TR
    const int d64_bias = 398;
    return __d64_biased_exponent(*x.data()) - d64_bias;
#else
    return decDoubleGetExponent(x.data());
#endif
}

int DecimalUtil::quantum(Decimal128 x)
{
    BSLS_ASSERT(!isInf(x));
    BSLS_ASSERT(!isNan(x));

#if BDLDFP_DECIMALPLATFORM_C99_TR
    const int d128_bias = 6176;
    return __d128_biased_exponent(*x.data()) - d128_bias;
#else
    return decQuadGetExponent(x.data());
#endif
}

bool DecimalUtil::sameQuantum(Decimal64 x, Decimal64 y)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return samequantumd64(*x.data(), *y.data());
#else
    return decDoubleSameQuantum(x.data(), y.data()) == 1;
#endif
}

bool DecimalUtil::sameQuantum(Decimal128 x, Decimal128 y)
{
#if BDLDFP_DECIMALPLATFORM_C99_TR
    return samequantumd128(*x.data(), *y.data());
#else
    return decQuadSameQuantum(x.data(), y.data()) == 1;
#endif
}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Bloomberg L.P.
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
