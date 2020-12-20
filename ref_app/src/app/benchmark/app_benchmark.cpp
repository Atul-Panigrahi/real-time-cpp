
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2014.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_benchmark.h>

namespace app
{
  namespace benchmark
  {
    void task_init();
    void task_func();
  }
}

void app::benchmark::task_init()
{
  mcal::benchmark::benchmark_port_type::set_direction_output();
}

void app::benchmark::task_func()
{
  mcal::benchmark::benchmark_port_type::set_pin_high();

  mcal::benchmark::benchmark_port_type::set_pin_low();
}
