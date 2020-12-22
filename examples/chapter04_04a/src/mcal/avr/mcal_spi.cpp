///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_port.h>
#include <mcal_reg.h>
#include <mcal_spi.h>

#include <mcal_spi/mcal_spi_software_port_driver.h>

void mcal::spi::init(const mcal::spi::config_type*)
{
}

util::communication_base& mcal::spi::spi0()
{
  using local_port_pin_sck__type = mcal::port::port_pin<std::uint8_t, std::uint8_t, mcal::reg::portc, 3U>;
  using local_port_pin_miso_type = mcal::port::port_pin<std::uint8_t, std::uint8_t, mcal::reg::portc, 2U>;
  using local_port_pin_mosi_type = mcal::port::port_pin<std::uint8_t, std::uint8_t, mcal::reg::portc, 1U>;

  using local_port_pin_csn0_type = mcal::port::port_pin<std::uint8_t, std::uint8_t, mcal::reg::portc, 4U>;

  using mcal_spi_channel0_type =
    mcal::spi::spi_software_port_driver<local_port_pin_sck__type,
                                        local_port_pin_miso_type,
                                        local_port_pin_mosi_type,
                                        local_port_pin_csn0_type,
                                        0U>;

  static mcal_spi_channel0_type com0;

  return com0;
}
