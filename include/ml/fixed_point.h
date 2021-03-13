/**
 * ml - simple header-only mathematics library
 * 
 * fixed-point types.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

#ifndef ML_NO_CNL

/** signed 32 bit signed fixed-point type with 16 fractional bits. */
typedef cnl::static_number<31,-16> fixed_t;

/** signed 32 bit signed fixed-point type with 4 fractional bits. */
typedef cnl::static_number<31,-4> fixed_28_4_t;

/** signed 32 bit signed fixed-point type with 8 fractional bits. */
typedef cnl::static_number<31,-8> fixed_24_8_t;

#endif /* ML_NO_CNL */

/** unsigned 32 bit unsigned fixed point type with one represented as ~0. */
typedef closed_unit_interval<std::uint32_t> fixed_32_t;

} /* namespace ml */