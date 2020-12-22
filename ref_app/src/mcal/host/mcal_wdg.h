///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_WDG_2010_04_10_H_
  #define MCAL_WDG_2010_04_10_H_

  namespace sys { namespace idle { void task_func(); } }

  namespace mcal
  {
    namespace wdg
    {
      typedef void config_type;

      void init(const config_type*);

      class secure final
      {
        static void trigger();

        friend void ::sys::idle::task_func();
      };
    }
  }

#endif // MCAL_WDG_2010_04_10_H_
