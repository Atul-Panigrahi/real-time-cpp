///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2018.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_WDT_2010_04_10_H_
  #define MCAL_WDT_2010_04_10_H_

  extern "C" void __my_startup() __attribute__((section(".startup"), used, noinline));

  namespace sys { namespace idle { void task_func(); } }

  namespace util { template<typename unsigned_tick_type> class timer; }

  namespace mcal
  {
    namespace wdg
    {
      typedef void config_type;

      void init(const config_type*);

      struct secure final
      {
      private:
        friend void ::sys::idle::task_func();
        friend void ::__my_startup();

        template<typename unsigned_tick_type>
        friend class util::timer;

        static void trigger() { asm volatile("wdr"); }
      };
    }
  }

#endif // MCAL_WDT_2010_04_10_H_
