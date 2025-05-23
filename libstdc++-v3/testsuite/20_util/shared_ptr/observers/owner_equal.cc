// { dg-do run { target c++26 } }
// { dg-require-effective-target hosted }

// Copyright (C) 2008-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// 20.3.2.2 Template class shared_ptr [util.smartptr.shared]

#include <memory>
#include <testsuite_hooks.h>

struct A
{
  int i;
  virtual ~A() { }
};

struct B : A
{
};

// 20.3.2.2.6 shared_ptr observers [util.smartptr.weak.obs]

void
test01()
{
  // test empty shared_ptrs compare equivalent
  std::shared_ptr<A> p1;
  std::shared_ptr<B> p2;
  VERIFY( p1.owner_equal(p2) && p2.owner_equal(p1) );
}


// Construction from pointer
void
test02()
{
  std::shared_ptr<A> a0;

  std::shared_ptr<A> a1(new A);
  VERIFY( !a1.owner_equal(a0) && !a0.owner_equal(a1) );

  std::shared_ptr<B> b1(new B);
  VERIFY( !a1.owner_equal(b1) && !b1.owner_equal(a1) );

  std::shared_ptr<A> a2(a1);
  VERIFY( a1.owner_equal(a2) && a2.owner_equal(a1) );
  a2 = b1;
  VERIFY( b1.owner_equal(a2) && a2.owner_equal(b1) );

  std::weak_ptr<A> w1(a1);
  VERIFY( a1.owner_equal(w1) && w1.owner_equal(a1) );
  std::weak_ptr<A> w2(a2);
  VERIFY( !b1.owner_equal(w2) && !w2.owner_equal(b1) );

  static_assert( noexcept(a1.owner_equal(a0)), "" );
  static_assert( noexcept(a1.owner_equal(b1)), "" );
  static_assert( noexcept(b1.owner_equal(a1)), "" );
  static_assert( noexcept(a1.owner_equal(w1)), "" );
  static_assert( noexcept(b1.owner_equal(w1)), "" );
}

// Aliasing
void
test03()
{
  std::shared_ptr<A> p1(new A());
  std::shared_ptr<int> p2(p1, &p1->i);
  VERIFY( p1.owner_equal(p2) && p2.owner_equal(p1) );
}

int
main()
{
  test01();
  test02();
  test03();
  return 0;
}
