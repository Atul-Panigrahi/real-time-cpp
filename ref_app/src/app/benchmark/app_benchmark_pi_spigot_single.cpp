///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <algorithm>
#include <cstdint>

#include <app/benchmark/app_benchmark.h>

#if(APP_BENCHMARK_TYPE == APP_BENCHMARK_TYPE_PI_SPIGOT_SINGLE)

#include <math/constants/pi_spigot_single.h>
#include <mcal_memory/mcal_memory_progmem_array.h>

namespace
{
  const mcal::memory::progmem::array<std::uint8_t, 101U> app_benchmark_pi_spigot_control MY_PROGMEM =
  {{
    3U, 1U, 4U, 1U, 5U, 9U, 2U, 6U, 5U, 3U,
    5U, 8U, 9U, 7U, 9U, 3U, 2U, 3U, 8U, 4U,
    6U, 2U, 6U, 4U, 3U, 3U, 8U, 3U, 2U, 7U,
    9U, 5U, 0U, 2U, 8U, 8U, 4U, 1U, 9U, 7U,
    1U, 6U, 9U, 3U, 9U, 9U, 3U, 7U, 5U, 1U,
    0U, 5U, 8U, 2U, 0U, 9U, 7U, 4U, 9U, 4U,
    4U, 5U, 9U, 2U, 3U, 0U, 7U, 8U, 1U, 6U,
    4U, 0U, 6U, 2U, 8U, 6U, 2U, 0U, 8U, 9U,
    9U, 8U, 6U, 2U, 8U, 0U, 3U, 4U, 8U, 2U,
    5U, 3U, 4U, 2U, 1U, 1U, 7U, 0U, 6U, 7U,
    9U
  }};

  using pi_spigot_type = math::constants::pi_spigot_single<21U, 9U>;

  std::array<std::uint32_t, pi_spigot_type::get_input__static_size()> app_benchmark_pi_spigot_in_;
  std::array<std::uint8_t,  pi_spigot_type::get_output_static_size()> app_benchmark_pi_spigot_out;

  pi_spigot_type app_benchmark_pi_spigot_object;

  std::uint32_t app_benchmark_pi_spigot_digits;
}

bool app::benchmark::run_pi_spigot_single()
{
  app_benchmark_pi_spigot_object.calculate(app_benchmark_pi_spigot_in_.data(),
                                           app_benchmark_pi_spigot_out.data());

  bool result_is_ok = true;

  for( ; app_benchmark_pi_spigot_digits < app_benchmark_pi_spigot_object.get_output_digit_count(); ++app_benchmark_pi_spigot_digits)
  {
    const std::uint8_t by_calculated = app_benchmark_pi_spigot_out    [app_benchmark_pi_spigot_digits];
    const std::uint8_t by_control    = app_benchmark_pi_spigot_control[app_benchmark_pi_spigot_digits];

    const bool next_digit_is_ok = (by_calculated == by_control);

    result_is_ok &= next_digit_is_ok;
  }

  return result_is_ok;
}

#endif // APP_BENCHMARK_TYPE_PI_SPIGOT_SINGLE
