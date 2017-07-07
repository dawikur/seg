// Copyright 2017, Dawid Kurek, <dawikur@gmail.com>

#ifndef INCLUDE_LEL_IMPL_HPP_
#define INCLUDE_LEL_IMPL_HPP_

#include <utility>

#include "lel/context.hpp"
#include "lel/variadic.hpp"

namespace LeL {

template <class Context, class IDs>
struct Impl;

template <class Context, char... IDs>
struct Impl<Context, Box<char, IDs...>> {
  using MyIDs = Box<char, IDs...>;

 public:
  constexpr Impl() : left(), right() {}

  constexpr Impl(typename Context::ViewL left, typename Context::ViewR right)
    : left(std::move(left)), right(std::move(right)) {}

  template <class... Values>
  constexpr decltype(auto) operator()(Values &&... values) const {
    static_assert(sizeof...(IDs) == sizeof...(values),
                  "Incorrect number of arguments");

    return call(typename Context::Mode(), std::forward<Values>(values)...);
  }

  // () function call
  // -> member access ?

  template <class Value>
  constexpr decltype(auto) operator=(Value value) const {
    return Impl<LeL::Context<Impl<Context, MyIDs>, Value, Assign, Left>, MyIDs>{
      *this, std::move(value)};
  }

  template <class RestV, class IDV>
  constexpr decltype(auto) operator=(Impl<RestV, IDV> viewV) const {
    return Impl<LeL::
                  Context<Impl<Context, MyIDs>, Impl<RestV, IDV>, Assign, Fold>,
                Merge<MyIDs, IDV>>{*this, std::move(viewV)};
  }

  template <class Value>
  constexpr decltype(auto) operator[](Value value) const {
    return Impl<LeL::Context<Impl<Context, MyIDs>, Value, Subscript, Left>,
                MyIDs>{*this, std::move(value)};
  }

  template <class RestV, class IDV>
  constexpr decltype(auto) operator[](Impl<RestV, IDV> viewV) const {
    return Impl<LeL::Context<Impl<Context, MyIDs>,
                             Impl<RestV, IDV>,
                             Subscript,
                             Fold>,
                Merge<MyIDs, IDV>>{*this, std::move(viewV)};
  }

 private:
  template <class... Values>
  constexpr decltype(auto) call(Single, Values &&... values) const {
    return typename Context::Func()(left(std::forward<Values>(values)...));
  }

  template <class Value>
  constexpr decltype(auto) call(Left, Value &&value) const {
    return typename Context::Func()(left(std::forward<Value>(value)), right);
  }

  template <class Value>
  constexpr decltype(auto) call(Right, Value &&value) const {
    return typename Context::Func()(left, right(std::forward<Value>(value)));
  }

  template <class... Values>
  constexpr decltype(auto) call(Fold, Values &&... values) const {
    return typename Context::Func()(left.slice(MyIDs(), values...),
                                    right.slice(MyIDs(), values...));
  }

  template <char... Slice, class... Values>
  constexpr decltype(auto) slice(Box<char, Slice...>,
                                 Values &&... values) const {
    using Indexes = typename Box<char, Slice...>::template IndexesOf<IDs...>;

    return slice(Indexes(), std::forward<Values>(values)...);
  }

  template <int... Indexes, class... Values>
  constexpr decltype(auto) slice(Box<int, Indexes...>, Values &&... values) const {
    return call(typename Context::Mode(),
                Variadic().Get<Indexes>(values...)...);
  }

  typename Context::ViewL const left;
  typename Context::ViewR const right;

  template <class ContextF, class IDsF>
  friend struct Impl;
};

}  // namespace LeL

#endif  // INCLUDE_LEL_IMPL_HPP_
