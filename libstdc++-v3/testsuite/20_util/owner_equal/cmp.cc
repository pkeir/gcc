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

// 20.3.2.6 Struct owner_equal [util.smartptr.owner.equal]

#include <memory>
#include <algorithm>
#include <testsuite_hooks.h>

struct A { };

struct B { A a[2]; };

int
test01()
{
  // test empty shared_ptrs compare equivalent
  std::owner_equal eq;
  std::shared_ptr<A> p1;
  std::shared_ptr<A> p2;
  VERIFY( eq(p1, p2) && eq(p2, p1) );
  std::weak_ptr<A> p3;
  VERIFY( eq(p1, p3) && eq(p3, p1) );
  VERIFY( eq(p1, p3) && eq(p3, p1) );
  return 0;
}


// Construction from pointer
int
test02()
{
  std::owner_equal eq;

  std::shared_ptr<A> empty;

  std::shared_ptr<A> a1(new A);
  VERIFY( !eq(empty, a1) && !eq(a1, empty) );

  std::shared_ptr<A> a2(new A);
  VERIFY( !eq(a1, a2) && !eq(a2, a1) );

  std::weak_ptr<A> w1(a1);
  VERIFY( eq(a1, w1) && eq(w1, a1) );

  std::weak_ptr<A> w2(a2);
  VERIFY( !eq(w1, w2) && !eq(w2, w1) );

  a1.reset();
  VERIFY( eq(empty, a1) && eq(a1, empty) );
  VERIFY( !eq(a1, w1) && !eq(w1, a1) );

  a2.reset();
  VERIFY( eq(a2, a1) && eq(a1, a2) );

  return 0;
}

// aliasing
int
test03()
{
  std::owner_equal eq;

  std::shared_ptr<B> b(new B);
  std::shared_ptr<A> a0(b, &b->a[0]);
  std::shared_ptr<A> a1(b, &b->a[1]);
  // values are different but owners are equivalent:
  VERIFY( a0 < a1 && eq(a0, a1) && eq(b, a0) && eq(b, a1) );

  std::weak_ptr<A> w0(a0);
  std::weak_ptr<A> w1(a1);
  VERIFY( eq(w0, w1) && eq(w1, w0) );
  VERIFY( eq(a0, w1) && eq(w1, a0) );
  VERIFY( eq(w0, a1) && eq(a1, w0) );

  return 0;
}

// as binary predicate
int
test04()
{
  std::owner_equal eq;

  std::shared_ptr<B> b(new B);
  std::shared_ptr<A> a0(b, &b->a[0]);
  std::shared_ptr<A> a1(b, &b->a[1]);
  std::shared_ptr<A> c(new A);
  std::weak_ptr<A> a[3]{a0, a1, c};
  std::weak_ptr<A>* p = std::unique(a, a+3, eq);
  VERIFY( p == &a[2] );

  return 0;
}

int
main()
{
  test01();
  test02();
  test03();
  test04();
  return 0;
}
