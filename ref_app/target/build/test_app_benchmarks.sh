#!/usr/bin/env bash
#
#  Copyright Christopher Kormanyos 2020 - 2021.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

if [[ "$1" != "" ]]; then
    GCC="$1"
else
    GCC=g++
fi

if [[ "$2" != "" ]]; then
    STD="$2"
else
    STD=c++11
fi

mkdir -p bin

rm -f ./bin/*.*

echo run benchmarks with GCC=$GCC STD=$STD

$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_COMPLEX             -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_complex.cpp              -o ./bin/app_benchmark_complex.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_CRC                 -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_crc.cpp                  -o ./bin/app_benchmark_crc.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_FAST_MATH           -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_fast_math.cpp            -o ./bin/app_benchmark_fast_math.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_FILTER              -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_filter.cpp               -o ./bin/app_benchmark_filter.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_FIXED_POINT         -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_fixed_point.cpp          -o ./bin/app_benchmark_fixed_point.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_FLOAT               -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_float.cpp                -o ./bin/app_benchmark_float.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_HASH                -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_hash.cpp                 -o ./bin/app_benchmark_hash.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_NONE                -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_none.cpp                 -o ./bin/app_benchmark_none.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_PI_AGM              -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_pi_agm.cpp               -o ./bin/app_benchmark_pi_agm.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_PI_SPIGOT           -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_pi_spigot.cpp            -o ./bin/app_benchmark_pi_spigot.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_PI_SPIGOT_SINGLE    -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_pi_spigot_single.cpp     -o ./bin/app_benchmark_pi_spigot_single.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_WIDE_DECIMAL        -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_wide_decimal.cpp         -o ./bin/app_benchmark_wide_decimal.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_WIDE_INTEGER        -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_wide_integer.cpp         -o ./bin/app_benchmark_wide_integer.exe
$GCC -std=$STD -Wall -Werror -O3 -march=native -fsanitize=address -I./src/mcal/host -I./src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_TRAPEZOID_INTEGRAL  -DAPP_BENCHMARK_STANDALONE_MAIN ./src/app/benchmark/app_benchmark_trapezoid_integral.cpp   -o ./bin/app_benchmark_trapezoid_integral.exe

./bin/app_benchmark_complex.exe
result_var_complex=$?

./bin/app_benchmark_crc.exe
result_var_crc=$?

./bin/app_benchmark_fast_math.exe
result_var_fast_math=$?

./bin/app_benchmark_filter.exe
result_var_filter=$?

./bin/app_benchmark_fixed_point.exe
result_var_fixed_point=$?

./bin/app_benchmark_float.exe
result_var_float=$?

./bin/app_benchmark_hash.exe
result_var_hash=$?

./bin/app_benchmark_none.exe
result_var_none=$?

./bin/app_benchmark_pi_agm.exe
result_var_pi_agm=$?

./bin/app_benchmark_pi_spigot.exe
result_var_pi_spigot=$?

./bin/app_benchmark_pi_spigot_single.exe
result_var_pi_spigot_single=$?

./bin/app_benchmark_trapezoid_integral.exe
result_var_trapezoid_integral=$?

./bin/app_benchmark_wide_decimal.exe
result_var_wide_decimal=$?

./bin/app_benchmark_wide_integer.exe
result_var_wide_integer=$?

echo "result_var_complex           : "  "$result_var_complex"
echo "result_var_crc               : "  "$result_var_crc"
echo "result_var_fast_math         : "  "$result_var_fast_math"
echo "result_var_filter            : "  "$result_var_filter"
echo "result_var_fixed_point       : "  "$result_var_fixed_point"
echo "result_var_float             : "  "$result_var_float"
echo "result_var_hash              : "  "$result_var_hash"
echo "result_var_none              : "  "$result_var_none"
echo "result_var_pi_agm            : "  "$result_var_pi_agm"
echo "result_var_pi_spigot         : "  "$result_var_pi_spigot"
echo "result_var_pi_spigot_single  : "  "$result_var_pi_spigot_single"
echo "result_var_trapezoid_integral: "  "$result_var_trapezoid_integral"
echo "result_var_wide_decimal      : "  "$result_var_wide_decimal"
echo "result_var_wide_integer      : "  "$result_var_wide_integer"

result_total=$((result_var_complex+result_var_crc+result_var_fast_math+result_var_filter+result_var_fixed_point+result_var_float+result_var_hash+result_var_none+result_var_pi_agm+result_var_pi_spigot+result_var_pi_spigot_single+result_var_trapezoid_integral+result_var_wide_decimal+result_var_wide_integer))

echo "result_total                 : "  "$result_total"

exit $result_total
