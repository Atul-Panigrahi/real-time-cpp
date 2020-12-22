///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2017 - 2018.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// chapter05_03-002_communication_template.cpp

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

class communication
{
public:
  communication() { }

  virtual ~communication() { }

  virtual bool send_byte(const std::uint8_t b) const
  {
    // Simulate sending a byte on the PC.
    std::cout << "Sending: "
              << std::hex
              << std::showbase
              << unsigned(b)
              << std::endl;

    const std::ios::iostate cout_state = std::cout.rdstate();

    const bool send_byte_is_ok = ((cout_state & (std::ios::badbit | std::ios::failbit)) == 0);

    return send_byte_is_ok;
  }

  // Add a template send_uint function.
  template<typename unsigned_type>
  bool send_uint(const unsigned_type& u) const
  {
    constexpr bool type_is_signed
      = std::numeric_limits<unsigned_type>::is_signed;

    // Ensure that unsigned_type is unsigned.
    static_assert(type_is_signed == false,
                  "error: type must be unsigned");

    constexpr std::size_t count =
      std::numeric_limits<unsigned_type>::digits / 8;

    std::size_t i;

    for(i = 0U; i < count; i++)
    {
      const std::uint8_t by(u >> (i * 8U));

      if(send_byte(by) == false)
      {
        break;
      }
    }

    return (i == count);
  }

  std::uint8_t recv_byte() const
  {
    return recv_buffer;
  }

private:
  std::uint8_t recv_buffer;
};

int main()
{
  communication com;

  const bool send_uint_is_ok = com.send_uint(std::uint32_t(UINT32_C(0x11223344)));

  std::cout << "Result of send_uint: "
            << std::boolalpha
            << send_uint_is_ok
            << std::endl;
}
