///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef INTEGRAL_2012_01_09_H_
  #define INTEGRAL_2012_01_09_H_

  #include <cmath>

  namespace math
  {
    template<typename real_value_type,
             typename real_function_type>
    real_value_type integral(const real_value_type& a,
                             const real_value_type& b,
                             const real_value_type& tol,
                             real_function_type real_function)
    {
      std::uint_fast32_t n2(1);

      real_value_type step = ((b - a) / 2U);

      real_value_type result = (real_function(a) + real_function(b)) * step;

      const std::uint_fast8_t k_max = UINT8_C(32);

      for(std::uint_fast8_t k = UINT8_C(0); k < k_max; ++k)
      {
        real_value_type sum(0);

        for(std::uint_fast32_t j(0U); j < n2; ++j)
        {
          const std::uint_fast32_t two_j_plus_one = (j * UINT32_C(2)) + UINT32_C(1);

          sum += real_function(a + (step * two_j_plus_one));
        }

        const real_value_type tmp = result;

        result = (result / 2U) + (step * sum);

        const real_value_type ratio = std::abs(tmp / result);

        const real_value_type delta = std::abs(ratio - 1U);

        if((k > UINT8_C(1)) && (delta < tol))
        {
          break;
        }

        n2 *= 2U;

        step /= 2U;
      }

      return result;
    }
  }

  /*
  #include <cstdint>
  #include <math/calculus/integral.h>
  #include <math/constants/constants.h>

  template<typename float_type>
  float_type cyl_bessel_j(const std::uint_fast8_t n,
                          const float_type& x)
  {
    const float_type epsilon =
      std::numeric_limits<float_type>::epsilon();

    const float_type tol =
      std::sqrt(epsilon);

    const float_type jn = math::integral(
        float_type(0),
        math::constants::pi<float_type>(),
        tol,
        [&x, &n](const float_type& t) -> float_type
        {
          return std::cos(x * std::sin(t) - (t * n));
        })
      / math::constants::pi<float_type>();

    return jn;
  }

  // Compute y = cyl_bessel_j(2, 1.23) = 0.16636938378681407351267852431513159437103348245333
  // N[BesselJ[2, 123/100], 50]
  const float j2 = cyl_bessel_j(UINT8_C(2), 1.23F);
  */

#endif // INTEGRAL_2012_01_09_H_
