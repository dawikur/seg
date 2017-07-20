// Copyright 2017, Dawid Kurek, <dawikur@gmail.com>

#include "lel/variadic.hpp"

#include "gtest/gtest.h"

TEST(variadic_test, variadic_get_returns_proper_value) {
  ASSERT_EQ(11, LeL::Variadic::Get<0>::From(11, 22, 33, 44, 55));
  ASSERT_EQ(22, LeL::Variadic::Get<1>::From(11, 22, 33, 44, 55));
  ASSERT_EQ(53, LeL::Variadic::Get<2>::From(11, 22, 53, 46, 55));
  ASSERT_EQ(46, LeL::Variadic::Get<3>::From(11, 22, 33, 46, 55));
}
