///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2018.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_PWM_BOARD_2017_10_13_H_
  #define MCAL_PWM_BOARD_2017_10_13_H_

  #include <algorithm>
  #include <cstdint>

  #include <mcal/mcal_pwm_base.h>
  #include <mcal_irq.h>
  #include <mcal_port.h>
  #include <util/utility/util_noncopyable.h>

  namespace mcal
  {
    namespace pwm
    {
      // A software PWM template for a pin at address addr and
      // bit-position bpos. The default resolution is 100 ticks.
      // These values can be changed as needed.

      template<typename port_pin_type>
      class pwm_board : public pwm_base
      {
      public:
        pwm_board(const duty_type resol,
                  const duty_type duty = 0U) : pwm_base(resol, duty)
        {
          // Set the port pin to output with logic level low.
          port_pin_type::set_pin_low();
          port_pin_type::set_direction_output();
        }

        virtual void service()
        {
          // Increment the counter.
          ++counter;

          if(counter <= duty_cycle)
          {
            // Output high if counter less than duty cycle.
            port_pin_type::set_pin_high();
          }
          else
          {
            // Output low if counter exceeds duty cycle.
            port_pin_type::set_pin_low();
          }

          if(counter >= resolution)
          {
            // Latch duty cycle from the shadow register.
            duty_cycle = shadow;

            // Reset the counter for a new PWM period.
            counter = 0U;
          }
        }
      };
    }
  }

#endif // MCAL_PWM_BOARD_2017_10_13_H_
