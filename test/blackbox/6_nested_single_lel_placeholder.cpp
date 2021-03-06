// Copyright 2017, Dawid Kurek, <dawikur@gmail.com>

#include "lel.hpp"

#include <memory>
#include <vector>

#include "gtest/gtest.h"

class nested_single_lel_placeholder_test : public ::testing::Test {
 protected:
  lel::placeholder<'x'> _x;
};

TEST_F(nested_single_lel_placeholder_test, indirection_and_cmparition) {
  auto is_ptr_less = *_x < 9;

  auto ptr = std::make_unique<int>(5);

  ASSERT_TRUE(is_ptr_less(ptr));
}

TEST_F(nested_single_lel_placeholder_test, indirection_and_subscription) {
  auto  get_second = (*_x)[2u];

  auto ptr = std::make_unique<std::vector<int>>();
  ptr->push_back(1);
  ptr->push_back(2);
  ptr->push_back(3);
  ptr->push_back(4);

  ASSERT_EQ(3, get_second(ptr));
}

TEST_F(nested_single_lel_placeholder_test, indirection_subscription_indirection) {
  auto get_get = *((*_x)[1u]);

  auto ptr = std::make_unique<std::vector<std::unique_ptr<int>>>();
  ptr->push_back(std::make_unique<int>(1));
  ptr->push_back(std::make_unique<int>(2));
  ptr->push_back(std::make_unique<int>(3));
  ptr->push_back(std::make_unique<int>(4));

  ASSERT_EQ(2, get_get(ptr));
}

TEST_F(nested_single_lel_placeholder_test, multiple_use_unique_ptr) {
  auto test = *_x < *_x + 1;

  auto ptr = std::make_unique<int>(5);

  ASSERT_TRUE(test(ptr));
}

TEST_F(nested_single_lel_placeholder_test, value_will_be_updated_in_multiple_places) {
  auto update = ++_x < ++_x;

  int value = 1;

  update(value);

  ASSERT_EQ(3, value);
}
