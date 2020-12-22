///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2020.
//  Copyright Joel Winarske 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_port.h>
#include <mcal_reg.h>


void mcal::port::init(const config_type*)
{
  // Enable GPIO Clock.
  mcal::reg::reg_access_static<std::uint32_t, std::uint32_t, 
                               mcal::reg::syscon_sysahbclkctrl,
                               mcal::reg::syscon_clock_gpio>::reg_or();
}
