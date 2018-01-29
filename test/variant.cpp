//  (C) Copyright 2016 - 2018 Christopher Beck

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <strict_variant/multivisit.hpp>
#include <strict_variant/variant.hpp>
#include <strict_variant/variant_stream_ops.hpp>

#include "test_harness/test_harness.hpp"

#include <algorithm>
#include <string>
#include <type_traits>

namespace strict_variant {

////////////////////////////////
// SAFELY_CONSTRUCTIBLE TESTS //
////////////////////////////////

namespace {

static_assert(safely_constructible<bool, bool>::value, "failed a unit test");
static_assert(!safely_constructible<bool, int>::value, "failed a unit test");
static_assert(!safely_constructible<int, bool>::value, "failed a unit test");
static_assert(safely_constructible<int, int>::value, "failed a unit test");

static_assert(!safely_constructible<bool, signed char>::value, "failed a unit test");
static_assert(!safely_constructible<signed char, bool>::value, "failed a unit test");
static_assert(safely_constructible<signed char, signed char>::value, "failed a unit test");

static_assert(!safely_constructible<int, signed char>::value, "failed a unit test");
static_assert(!safely_constructible<signed char, int>::value, "failed a unit test");

static_assert(safely_constructible<char, signed char>::value, "failed a unit test");
static_assert(safely_constructible<unsigned char, signed char>::value, "failed a unit test");
static_assert(safely_constructible<unsigned char, char>::value, "failed a unit test");
static_assert(!safely_constructible<signed char, char>::value, "failed a unit test");
static_assert(!safely_constructible<signed char, unsigned char>::value, "failed a unit test");
static_assert(!safely_constructible<char, unsigned char>::value, "failed a unit test");

static_assert(!safely_constructible<unsigned int, unsigned long>::value, "failed a unit test");
static_assert(safely_constructible<unsigned long, unsigned int>::value, "failed a unit test");
static_assert(safely_constructible<unsigned long, unsigned long>::value, "failed a unit test");

static_assert(!safely_constructible<unsigned int, unsigned char>::value, "failed a unit test");
static_assert(!safely_constructible<unsigned char, unsigned int>::value, "failed a unit test");
static_assert(safely_constructible<unsigned char, unsigned char>::value, "failed a unit test");

static_assert(!safely_constructible<bool, unsigned char>::value, "failed a unit test");
static_assert(!safely_constructible<unsigned char, bool>::value, "failed a unit test");

static_assert(safely_constructible<float, float>::value, "failed a unit test");
static_assert(safely_constructible<double, float>::value, "failed a unit test");
static_assert(!safely_constructible<float, double>::value, "failed a unit test");
static_assert(safely_constructible<double, double>::value, "failed a unit test");

static_assert(!safely_constructible<bool, float>::value, "failed a unit test");
static_assert(!safely_constructible<float, bool>::value, "failed a unit test");

static_assert(!safely_constructible<bool, double>::value, "failed a unit test");
static_assert(!safely_constructible<double, bool>::value, "failed a unit test");

static_assert(!safely_constructible<int, float>::value, "failed a unit test");
static_assert(!safely_constructible<float, int>::value, "failed a unit test");

static_assert(!safely_constructible<signed int, unsigned int>::value, "failed a unit test");
static_assert(safely_constructible<unsigned int, signed int>::value, "failed a unit test");

static_assert(!safely_constructible<signed long, unsigned long>::value, "failed a unit test");
static_assert(safely_constructible<unsigned long, signed long>::value, "failed a unit test");

static_assert(!safely_constructible<signed char, unsigned int>::value, "failed a unit test");
static_assert(!safely_constructible<unsigned int, signed char>::value, "failed a unit test");

static_assert(!safely_constructible<double, int>::value, "failed a unit test");
static_assert(!safely_constructible<int, double>::value, "failed a unit test");

static_assert(!safely_constructible<double, const int &>::value, "failed a unit test");
static_assert(!safely_constructible<int, const double &>::value, "failed a unit test");

// Pointer to int checks and such

static_assert(!safely_constructible<double, const int *>::value, "failed a unit test");
static_assert(!safely_constructible<int, const double *>::value, "failed a unit test");

static_assert(!safely_constructible<int, const char *>::value, "failed a unit test");
static_assert(!safely_constructible<const char *, int>::value, "failed a unit test");
static_assert(safely_constructible<const char *, const char *>::value, "failed a unit test");

static_assert(!safely_constructible<int &, const char *>::value, "failed a unit test");
static_assert(!safely_constructible<const char * const, int &>::value, "failed a unit test");

static_assert(!safely_constructible<int *, const char *>::value, "failed a unit test");
static_assert(!safely_constructible<const char *, int *>::value, "failed a unit test");
static_assert(safely_constructible<int *, int *>::value, "failed a unit test");

static_assert(!safely_constructible<bool, decltype("asdf")>::value, "failed a unit test");
static_assert(!safely_constructible<int, decltype("asdf")>::value, "failed a unit test");
static_assert(!safely_constructible<char, decltype("asdf")>::value, "failed a unit test");
static_assert(!safely_constructible<float, decltype("asdf")>::value, "failed a unit test");
static_assert(safely_constructible<const char *, decltype("asdf")>::value, "failed a unit test");
static_assert(!safely_constructible<const bool *, decltype("asdf")>::value, "failed a unit test");
static_assert(!safely_constructible<const int *, decltype("asdf")>::value, "failed a unit test");
static_assert(!safely_constructible<const float *, decltype("asdf")>::value, "failed a unit test");

// Pointer const conversions
static_assert(safely_constructible<const char *, char *>::value, "failed a unit test");
static_assert(!safely_constructible<char *, const char *>::value, "failed a unit test");
static_assert(safely_constructible<const char *, char * const>::value, "failed a unit test");
static_assert(!safely_constructible<char *, const char * const>::value, "failed a unit test");

//[ strict_variant_safely_constructible_examples
static_assert(safely_constructible<unsigned char, char>::value, "");
static_assert(!safely_constructible<char, unsigned char>::value, "");

static_assert(safely_constructible<unsigned int, int>::value, "");
static_assert(!safely_constructible<int, char>::value, "");
static_assert(!safely_constructible<int, bool>::value, "");
static_assert(!safely_constructible<char, bool>::value, "");

static_assert(safely_constructible<double, float>::value, "");
static_assert(!safely_constructible<float, double>::value, "");
static_assert(!safely_constructible<int, double>::value, "");
static_assert(!safely_constructible<double, int>::value, "");

static_assert(!safely_constructible<bool, const char *>::value, "");
static_assert(!safely_constructible<const void *, const char *>::value, "");

static_assert(safely_constructible<const char *, char *>::value, "");
static_assert(!safely_constructible<char *, const char *>::value, "");

static_assert(safely_constructible<const char *, decltype("foo")>::value, "");
static_assert(!safely_constructible<char *, decltype("foo")>::value, "");

static_assert(safely_constructible<std::string, const char *>::value, "");
static_assert(safely_constructible<std::string, decltype("foo")>::value, "");
static_assert(!safely_constructible<const char *, std::string>::value, "");
//]

} // end anonymous namespace

/////////////////////////
// TYPELIST OPERATIONS //
/////////////////////////

struct _dummy_t_1 {};
struct _dummy_t_2 {};

static_assert(
  std::is_same<_dummy_t_1, mpl::Index_At<mpl::TypeList<_dummy_t_1, _dummy_t_2>, 0>>::value,
  "unit test failed");

static_assert(
  std::is_same<_dummy_t_2, mpl::Index_At<mpl::TypeList<_dummy_t_1, _dummy_t_2>, 1>>::value,
  "unit test failed");

/////////////////////////
// VARIANT TYPE TRAITS //
/////////////////////////

// Check the proper_subvariant trait, which is used to enable "generalizing"
// ctor without interfering with special member functions.
static_assert(
  detail::proper_subvariant<variant<int, double>, variant<int, double, std::string>>::value,
  "failed a unit test");

static_assert(detail::proper_subvariant<variant<int, std::string, double>,
                                        variant<int, double, std::string>>::value,
              "failed a unit test");

static_assert(
  !detail::proper_subvariant<variant<int, std::string, double>, variant<int, double>>::value,
  "failed a unit test");

static_assert(!detail::proper_subvariant<variant<int, double>, variant<int, double>>::value,
              "failed a unit test");

// Check nothrow status
static_assert(std::is_nothrow_destructible<variant<int, double>>::value, "failed a unit test");
static_assert(std::is_nothrow_move_constructible<variant<int, double>>::value,
              "failed a unit test");

// Check core traits that enable construction from other types
template <typename U, typename V>
struct allow_variant_construction : safely_constructible<unwrap_type_t<U>, V> {};

static_assert(allow_variant_construction<std::string, const char *>::value, "failed a unit test");
static_assert(allow_variant_construction<std::string, const std::string &>::value,
              "failed a unit test");
static_assert(allow_variant_construction<std::string, std::string &&>::value, "failed a unit test");
static_assert(!allow_variant_construction<int, const char *>::value, "failed a unit test");
static_assert(allow_variant_construction<int, int>::value, "failed a unit test");
static_assert(!allow_variant_construction<int, double>::value, "failed a unit test");
static_assert(!allow_variant_construction<double, int>::value, "failed a unit test");
static_assert(allow_variant_construction<double, double>::value, "failed a unit test");

static_assert(allow_variant_construction<int, const int &>::value, "failed a unit test");
static_assert(!allow_variant_construction<int, const double &>::value, "failed a unit test");
static_assert(!allow_variant_construction<double, const int &>::value, "failed a unit test");
static_assert(allow_variant_construction<double, const double &>::value, "failed a unit test");

static_assert(allow_variant_construction<int, int &&>::value, "failed a unit test");
static_assert(!allow_variant_construction<int, double &&>::value, "failed a unit test");
static_assert(!allow_variant_construction<double, int &&>::value, "failed a unit test");
static_assert(allow_variant_construction<double, double &&>::value, "failed a unit test");

static_assert(allow_variant_construction<float, float &&>::value, "failed a unit test");
static_assert(!allow_variant_construction<float, double &&>::value, "failed a unit test");
static_assert(allow_variant_construction<double, float &&>::value, "failed a unit test");

static_assert(allow_variant_construction<std::string, std::string>::value, "failed a unit test");
static_assert(
  allow_variant_construction<recursive_wrapper<std::string>, const std::string &>::value,
  "failed a unit test");
static_assert(allow_variant_construction<recursive_wrapper<std::string>, const std::string>::value,
              "failed a unit test");

// Test noexcept annotations

struct some_visitor {
  double operator()(int i) const noexcept { return i; }
  double operator()(float f) const noexcept { return f; }
};

struct some_other_visitor {
  double operator()(int i) const noexcept { return i; }
  double operator()(float f) const noexcept(false) { return f; }
};

// TODO: Debug this
// static_assert(noexcept(apply_visitor(some_visitor{}, std::declval<variant<int, float>>())),
// "Apply visitor noexcept annotation not working!");
static_assert(!noexcept(apply_visitor(some_other_visitor{}, std::declval<variant<int, float>>())),
              "Apply visitor noexcept annotation not working!");

// Check that variant is resolving "ambiguous" constructions as expected
UNIT_TEST(ambiguous_string) {
  const char * test_string = "asdf";
  {
    typedef variant<std::string, const char *> Var_t;
    Var_t a{std::string{test_string}};
    TEST_EQ(a.which(), 0);
    TEST_TRUE(a.get<std::string>());
    TEST_FALSE(a.get<const char *>());

    // TODO: Should a = test_string be okay here? How would that work?
    a.emplace<const char *>(test_string);
    TEST_EQ(a.which(), 1);
    TEST_FALSE(a.get<std::string>());
    TEST_TRUE(a.get<const char *>());
  }
  {
    typedef variant<const char *, std::string> Var_t;
    Var_t a{std::string{test_string}};
    TEST_EQ(a.which(), 1);
    TEST_TRUE(a.get<std::string>());
    TEST_FALSE(a.get<const char *>());

    a.emplace<const char *>(test_string);
    TEST_EQ(a.which(), 0);
    TEST_FALSE(a.get<std::string>());
    TEST_TRUE(a.get<const char *>());
  }
}

// Test that variants using integral types are working as expected
UNIT_TEST(ambiguous_number) {
  {
    typedef variant<int, long> Var_t;
    Var_t a{5};
    TEST_EQ(a.which(), 0);
    a = 5l;
    TEST_EQ(a.which(), 1);
  }

  {
    typedef variant<long, long long> Var_t;
    Var_t a{5};
    TEST_EQ(a.which(), 0);
    a = 5l;
    TEST_EQ(a.which(), 0);
    a = 5ll;
    TEST_EQ(a.which(), 1);
  }

  {
    typedef variant<float, double> Var_t;
    Var_t a{5.0f};
    TEST_EQ(a.which(), 0);
    a = 5.0;
    TEST_EQ(a.which(), 1);
  }

  {
    typedef variant<double, float, int> Var_t;
    Var_t a{5};
    TEST_EQ(a.which(), 2);
    TEST_FALSE(a.get<double>());
    TEST_TRUE(a.get<int>());
    Var_t b{10.0f};
    TEST_EQ(b.which(), 1);
    TEST_TRUE(b.get<float>());
    TEST_FALSE(b.get<int>());
    TEST_FALSE(b.get<double>());
    Var_t c{10.0};
    TEST_EQ(c.which(), 0);
    TEST_TRUE(c.get<double>());
  }

  {
    typedef variant<int, float, double> Var_t2;
    Var_t2 a{5};
    TEST_EQ(a.which(), 0);
    TEST_TRUE(a.get<int>());
    Var_t2 b{10.0f};
    TEST_EQ(b.which(), 1);
    TEST_TRUE(b.get<float>());
    Var_t2 c{10.0};
    TEST_EQ(c.which(), 2);
    TEST_TRUE(c.get<double>());
  }

  {
    typedef variant<bool, int, float> Var_t3;
    Var_t3 a{true};
    TEST_EQ(a.which(), 0);
    TEST_TRUE(a.get<bool>());
    Var_t3 b{1};
    TEST_EQ(b.which(), 1);
    TEST_FALSE(b.get<bool>());
    TEST_TRUE(b.get<int>());
    Var_t3 c{10.0f};
    TEST_EQ(c.which(), 2);
    TEST_TRUE(c.get<float>());
  }

  {
    typedef variant<int, bool, float> Var_t4;
    Var_t4 a{true};
    TEST_EQ(a.which(), 1);
    TEST_FALSE(a.get<int>());
    TEST_TRUE(a.get<bool>());
    Var_t4 b{1};
    TEST_EQ(b.which(), 0);
    TEST_TRUE(b.get<int>());
    Var_t4 c{10.0f};
    TEST_EQ(c.which(), 2);
    TEST_TRUE(c.get<float>());
  }
}

// Test assignment

UNIT_TEST(assignment) {
  typedef variant<int, float, double, std::string> Var_t;

  Var_t a{5};
  TEST_TRUE(a.get<int>());
  TEST_FALSE(a.get<double>());

  a = "1234";
  TEST_FALSE(a.get<int>());
  TEST_FALSE(a.get<double>());
  TEST_TRUE(a.get<std::string>());

  a = 1234.0;
  TEST_TRUE(a.get<double>());

  a = 1234;
  TEST_EQ(a.which(), 0);
  TEST_TRUE(a.get<int>());

  a = 1234.0f;
  TEST_EQ(a.which(), 1);
  TEST_TRUE(a.get<float>());

  a = 1234.0;
  TEST_EQ(a.which(), 2);
  TEST_TRUE(a.get<double>());
}

// Test assignment with recursive_wrapper

UNIT_TEST(assign_wrapper) {
  typedef variant<int, float, double, recursive_wrapper<std::string>> Var_t;

  Var_t a{5};
  TEST_TRUE(a.get<int>());
  TEST_FALSE(a.get<double>());

  a = "1234";
  TEST_FALSE(a.get<int>());
  TEST_FALSE(a.get<double>());
  TEST_TRUE(a.get<std::string>());

  a = 1234.0;
  TEST_TRUE(a.get<double>());

  a = 1234;
  TEST_EQ(a.which(), 0);
  TEST_TRUE(a.get<int>());

  a = 1234.0f;
  TEST_EQ(a.which(), 1);
  TEST_TRUE(a.get<float>());

  a = 1234.0;
  TEST_EQ(a.which(), 2);
  TEST_TRUE(a.get<double>());
}

// Test indexed get
UNIT_TEST(get_index) {
  typedef variant<int, float, double, std::string> Var_t;

  Var_t a{5};
  TEST_TRUE(a.get<0>());
  TEST_FALSE(a.get<2>());

  a = "1234";
  TEST_FALSE(a.get<0>());
  TEST_FALSE(a.get<2>());
  TEST_TRUE(a.get<3>());

  a = 1234.0;
  TEST_TRUE(a.get<2>());

  a = 1234;
  TEST_EQ(a.which(), 0);
  TEST_TRUE(a.get<0>());
  TEST_EQ(1234, *a.get<0>());

  a = 1234.0f;
  TEST_EQ(a.which(), 1);
  TEST_TRUE(a.get<1>());
  TEST_EQ(1234.0f, *a.get<1>());

  a = 1234.0;
  TEST_EQ(a.which(), 2);
  TEST_TRUE(a.get<2>());
  TEST_EQ(1234.0, *a.get<2>());
}

// Test Emplace function

UNIT_TEST(emplace) {
  typedef variant<int, float, double, std::string> Var_t;

  Var_t a{5};
  TEST_TRUE(a.get<int>());

  a.emplace<std::string>("1234");
  TEST_TRUE(a.get<std::string>());

  a.emplace<double>(1234.0);
  TEST_TRUE(a.get<double>());

  a.emplace<int>(1234);
  TEST_TRUE(a.get<int>());

  a.emplace<float>(1234.0f);
  TEST_TRUE(a.get<float>());

  a.emplace<double>(1234.0);
  TEST_TRUE(a.get<double>());

  a.emplace<float>(1234);
  TEST_TRUE(a.get<float>());

  a.emplace<double>(1234);
  TEST_TRUE(a.get<double>());
  TEST_EQ(a, Var_t(1234.0));

  a.emplace<int>(1234.0);
  TEST_TRUE(a.get<int>());
  TEST_EQ(a, Var_t(1234));
}

// Test equality

UNIT_TEST(equality) {
  typedef variant<int, float, double, std::string> Var_t;

  Var_t a{5};

  TEST_EQ(a, Var_t{5});
  a.emplace<float>(5);
  TEST_EQ(a, Var_t{5.0f});
  TEST_NE(a, Var_t{5});
  TEST_NE(a, Var_t{"asdf"});
  a = "asdf";
  TEST_EQ(a, Var_t{"asdf"});

  typedef variant<bool, int, float, std::string> Var_t2;
  Var_t2 b{true};
  TEST_NE(b, Var_t2{1});
  TEST_NE(b, Var_t2{1.0f});
  TEST_NE(b, Var_t2{"true"});
  TEST_EQ(b, Var_t2{true});
  b = false;
  TEST_EQ(b, Var_t2{false});
  TEST_NE(b, Var_t2{true});
  TEST_NE(b, Var_t2{0});
  TEST_NE(b, Var_t2{0.0f});
  TEST_NE(b, Var_t2{"false"});
  b = "false";
  TEST_EQ(b, Var_t2{"false"});
  TEST_NE(b, Var_t2{false});
  TEST_NE(b, Var_t2{0});
  TEST_NE(b, Var_t2{0.0f});
  TEST_NE(b, Var_t2{true});
  TEST_NE(b, Var_t2{1});
  TEST_NE(b, Var_t2{1.0f});
  TEST_NE(b, Var_t2{"true"});
}

// Test recrusive wrapper, promotion, etc.
struct dummy;
struct crummy;

typedef variant<recursive_wrapper<dummy>, recursive_wrapper<crummy>> variant_type_2;

struct dummy {
  const char * str;
};

struct crummy {
  int val;
};

typedef variant<int, dummy> variant_type_1;

UNIT_TEST(recursive_wrapper) {
  variant_type_1 v1(5);

  TEST_EQ(v1.which(), 0);

  {
    int * temp = get<int>(&v1);
    TEST_TRUE(temp);
    TEST_EQ(*temp, 5);
  }

  const char * foo_str = "asdf";
  dummy foo{foo_str};

  v1 = foo;

  TEST_EQ(v1.which(), 1);

  {
    dummy * temp = get<dummy>(&v1);
    TEST_TRUE(temp);
    TEST_EQ(temp->str, foo_str);
  }

  v1 = 10;

  TEST_EQ(v1.which(), 0);

  {
    int * temp = get<int>(&v1);
    TEST_TRUE(temp);
    TEST_EQ(*temp, 10);
  }

  variant_type_2 v2{crummy{5}};
  TEST_EQ(v2.which(), 1);
  {
    crummy * temp = get<crummy>(&v2);
    TEST_TRUE(temp);
    TEST_EQ(temp->val, 5);
  }
}

typedef variant<dummy, crummy> variant_type_3;
typedef variant<dummy, crummy, double> variant_type_4;
typedef variant<int, crummy, dummy> variant_type_5;

// Test that the variant "promotion" constructor works
UNIT_TEST(promotion) {
  crummy c{7};

  // TODO: Enable this?
  //  variant_type_3 v3{c};
  //  variant_type_4 v4{v3};
  //  variant_type_5 v5{v3};

  {
    variant_type_4 v4{variant_type_3{c}};
    variant_type_5 v5{variant_type_3{c}};

    crummy * c4 = get<crummy>(&v4);
    crummy * c5 = get<crummy>(&v5);

    TEST_TRUE(c4);
    TEST_TRUE(c5);
    TEST_EQ(c4->val, c5->val);
  }
}

namespace value_type_test {
struct foo {};
struct bar {};

struct visitor {
  mutable int flag_ = 0;

  void operator()(const foo &) const { flag_ = 1; }

  void operator()(const bar &) const { flag_ = 4; }

  void operator()(foo &) const { flag_ = 2; }

  void operator()(bar &) const { flag_ = 5; }

  void operator()(foo &&) const { flag_ = 3; }

  void operator()(bar &&) const { flag_ = 6; }
};

struct visitor2 {
  mutable int flag_ = 0;

  void operator()(foo &) const { flag_ = 1; }
  void operator()(bar &) const { flag_ = 2; }
};
} // end namespace value_type_test

UNIT_TEST(apply_visitor_value_type) {
  using namespace value_type_test;

  value_type_test::visitor vis;

  using var_t = variant<foo, bar>;
  var_t x;

  apply_visitor(vis, x);
  TEST_EQ(vis.flag_, 2);

  // TODO:
  apply_visitor(vis, std::move(x));
  TEST_EQ(vis.flag_, 3);

  apply_visitor(vis, static_cast<const var_t &>(x));
  TEST_EQ(vis.flag_, 1);

  x = bar{};

  apply_visitor(vis, x);
  TEST_EQ(vis.flag_, 5);

  apply_visitor(vis, std::move(x));
  TEST_EQ(vis.flag_, 6);

  apply_visitor(vis, static_cast<const var_t &>(x));
  TEST_EQ(vis.flag_, 4);

  /// Test that it works if the visitor is restricted in what types it can accept

  x = foo{};

  value_type_test::visitor2 vis2;

  TEST_EQ(vis2.flag_, 0);
  apply_visitor(vis2, x);
  TEST_EQ(vis2.flag_, 1);

  x = bar{};
  apply_visitor(vis2, x);
  TEST_EQ(vis2.flag_, 2);
}

UNIT_TEST(visit_value_type) {
  using namespace value_type_test;

  visitor vis;

  using var_t = variant<foo, bar>;
  var_t x;

  x.visit(vis);
  TEST_EQ(vis.flag_, 2);

  // std::move(x).visit(vis);
  var_t{}.visit(vis);
  TEST_EQ(vis.flag_, 3);

  static_cast<const var_t &>(x).visit(vis);
  TEST_EQ(vis.flag_, 1);

  x = bar{};

  x.visit(vis);
  TEST_EQ(vis.flag_, 5);

  std::move(x).visit(vis);
  TEST_EQ(vis.flag_, 6);

  static_cast<const var_t &>(x).visit(vis);
  TEST_EQ(vis.flag_, 4);

  /// Test that it works if the visitor is restricted in what types it can accept
  // TODO: Fix this!
  /*
  x = foo{};

  value_type_test::visitor2 vis2;

  TEST_EQ(vis2.flag_, 0);
  x.visit(vis2);
  TEST_EQ(vis2.flag_, 1);

  x = bar{};
  x.visit(vis2);
  TEST_EQ(vis2.flag_, 2);
  */
}

struct test_visitor {
  mutable int flag_ = 0;

  template <typename T>
  void operator()(T &&) & {
    flag_ = 1;
  }

  template <typename T>
  void operator()(T &&) const & {
    flag_ = 2;
  }

  template <typename T>
  void operator()(T &&) && {
    flag_ = 3;
  }
};

UNIT_TEST(visitor_value_type) {
  using var_t = variant<std::string, int>;
  var_t x;

  test_visitor vis;

  TEST_EQ(0, vis.flag_);
  apply_visitor(vis, x);
  TEST_EQ(1, vis.flag_);
  apply_visitor(std::move(vis), x);
  TEST_EQ(3, vis.flag_);
  apply_visitor(vis, x);
  TEST_EQ(1, vis.flag_);

  const test_visitor & vis_cref = vis;
  apply_visitor(vis_cref, x);
  TEST_EQ(2, vis.flag_);
}

UNIT_TEST(lambda_visitation) {
  using var_t = variant<int, double>;
  var_t x{5};

  TEST_EQ(10.0, (apply_visitor([](double d) -> double { return d * 2; }, x)));

  x = 7.7;

  TEST_EQ(15.4, (apply_visitor([](double d) -> double { return d * 2; }, x)));
}

UNIT_TEST(move) {
  using var_t = variant<std::string, int>;
  var_t x;

  TEST_EQ(x.which(), 0);

  var_t y{5};

  TEST_EQ(y.which(), 1);

  x = std::move(y);

  TEST_EQ(x.which(), 1);
  TEST_EQ(y.which(), 1);

  x = var_t{var_t{var_t{var_t{"asdf"}}}};

  TEST_EQ(x.which(), 0);
  TEST_EQ(y.which(), 1);

  x = var_t{var_t{var_t{var_t{std::move(y)}}}};

  TEST_EQ(x.which(), 1);
  TEST_EQ(y.which(), 1);
}

template <int i>
struct test_nocopy {
  test_nocopy() = default;
  test_nocopy(const test_nocopy &) = delete;
  test_nocopy(test_nocopy &&) = default;
  test_nocopy & operator=(test_nocopy &&) = default;
};

UNIT_TEST(noncopyable) {
  using test_a = test_nocopy<0>;
  using test_b = test_nocopy<1>;
  using var_t = variant<test_a, test_b>;

  var_t v;
  TEST_EQ(v.which(), 0);
  v = test_b{};
  TEST_EQ(v.which(), 1);
  v = test_a{};
  TEST_EQ(v.which(), 0);
  v = test_b{};
  TEST_EQ(v.which(), 1);
  v = test_a{};
  TEST_EQ(v.which(), 0);
}

template <int i>
struct test_throwmove {
  test_throwmove() = default;
  test_throwmove(const test_throwmove &) noexcept(false) {}
  test_throwmove(test_throwmove &&) noexcept(false) {}
  test_throwmove & operator=(test_throwmove &&) noexcept(false) { return *this; }
  ~test_throwmove() = default;
};

UNIT_TEST(throwing_move) {
  using test_a = test_throwmove<0>;
  using test_b = test_throwmove<1>;
  using var_t = variant<test_a, test_b>;

  var_t v;
  TEST_EQ(v.which(), 0);
  v.emplace<test_b>();
  TEST_EQ(v.which(), 1);
  v.emplace<test_a>();
  TEST_EQ(v.which(), 0);
  v.emplace<test_b>();
  TEST_EQ(v.which(), 1);
  v.emplace<test_a>();
  TEST_EQ(v.which(), 0);

  v.emplace<1>();
  TEST_EQ(v.which(), 1);
  v.emplace<0>();
  TEST_EQ(v.which(), 0);
}

UNIT_TEST(easy_variant) {
  using test_a = test_throwmove<0>;
  using test_b = test_throwmove<1>;
  using var_t = easy_variant<test_a, test_b>;

  var_t v;
  TEST_EQ(v.which(), 0);
  v = test_b{};
  TEST_EQ(v.which(), 1);
  v = test_a{};
  TEST_EQ(v.which(), 0);
  v = test_b{};
  TEST_EQ(v.which(), 1);
  v = test_a{};
  TEST_EQ(v.which(), 0);

  v.emplace<0>();
  TEST_EQ(v.which(), 0);
  v.emplace<1>();
  TEST_EQ(v.which(), 1);
}

struct test_eq {
  template <typename T, typename U>
  bool operator()(const T & t, const U & u) const {
    return t == u;
  }
};

UNIT_TEST(multivisit) {
  using var1_t = variant<int, double>;
  using var2_t = variant<bool, double>;

  var1_t v1{10};
  var2_t v2{true};

  TEST_EQ(false, apply_visitor(test_eq{}, v1, v2));
  TEST_EQ(true, apply_visitor(test_eq{}, v1, v1));
  TEST_EQ(true, apply_visitor(test_eq{}, v2, v2));

  v1 = 1;

  TEST_EQ(true, apply_visitor(test_eq{}, v1, v2));

  v2 = 10.0;

  TEST_EQ(false, apply_visitor(test_eq{}, v1, v2));

  v1 = 10;

  TEST_EQ(v1.which(), 0);
  TEST_EQ(true, apply_visitor(test_eq{}, v1, v2));
}

UNIT_TEST(generalizing_ctor) {
  using var_1_t = variant<int, bool>;
  using var_2_t = variant<bool, int>;
  using var_3_t = variant<bool, std::string, recursive_wrapper<int>>;
  using var_4_t = variant<bool, int, recursive_wrapper<double>, recursive_wrapper<std::string>>;

  var_1_t x;
  var_2_t y{x};
  var_1_t z{y};
  var_3_t w{x};
  var_3_t v{w};
  var_3_t u{y};
  var_4_t q{u};
  var_4_t r{x};
  var_4_t s{y};
  s = x;
  s = y;
  s = z;
  v = z;
  y = x;
  x = y;
}

namespace {

template <int i>
struct test_struct;

using test_var_t = variant<recursive_wrapper<test_struct<0>>, recursive_wrapper<test_struct<1>>>;

test_var_t
test_func() {
  return {};
}

template <int i>
struct test_struct {
  int baz;
};

} // end anonymous namespace

UNIT_TEST(incomplete_default_ctor) {
  auto v = test_func();
  auto w = test_func();
}

static_assert(std::is_same<mpl::ulist<0>,
                           typename filter_overloads<int, mpl::TypeList<int, float>>::type>::value,
              "");

UNIT_TEST(forwarding_reference_ctor_examples) {

  // clang-format off
  //[strict_variant_forwarding_reference_ctor_examples
  //<-
  {
    //->
    variant<short, long> v{10};                   // Selects `long`, `short` is not safely constructible from `int`.
    //<-
    TEST_EQ(v.which(), 1);
  }
  {
    //->
    variant<int, long> v{10};                     // Selects `int`, it is lower rank. (`int` dominates `long`.)
    //<-
    TEST_EQ(v.which(), 0);
  }
  {
    //->
    variant<long, long long> v{10};               // Selects `long`, it is lower rank. (dominates)
    //<-
    TEST_EQ(v.which(), 0);
  }
  {
    //->
    variant<int, unsigned int> v{10};             // Selects `int`, it has a better conversion (identity)
    //<-
    TEST_EQ(v.which(), 0);
  }
  {
    //->
    //=variant<long, unsigned int> v{10};            // Neither dominates.
    //=variant<long long, unsigned long> v{10};      // Neither dominates.
    //<-
    static_assert(filter_overloads<int&&, mpl::TypeList<long, unsigned int>>::type::size == 2, "");
    static_assert(filter_overloads<int&&, mpl::TypeList<long long, unsigned long>>::type::size == 2, "");
  } {
    //->
    variant<unsigned int, unsigned long> v{10u};  // Selects `unsigned int`, it is lower rank.
    //<-
    TEST_EQ(v.which(), 0);
  }
  {
    //->
    variant<long, unsigned long long> v{10u};     // `unsigned -> signed long` is considered unsafe.
    //]
    TEST_EQ(v.which(), 1);
  }

  // clang-format on
}

// Test combining emplace ctor with a recursive_wrapper
UNIT_TEST(emplace_recursive_wrapper) {
  struct A {
    int x;
  };
  struct B {
    int y;
  };
  struct C {
    int z;
  };
  struct D {
    explicit D(A, B, C) noexcept {}
    D(const D &) = delete;
  };

  variant<float, recursive_wrapper<D>> v;

  v.emplace<D>(A(), B(), C());
}

// Test combining emplace ctor with a recursive_wrapper
UNIT_TEST(copy_assign_recursive_wrapper) {
  struct D {
    D() noexcept {}
    D(const D &) noexcept {};
    // D & operator =(const D &) = delete;
  };

  using var_t = variant<float, recursive_wrapper<D>>;
  var_t v, u;

  v.emplace<D>();
  u = static_cast<const var_t &>(v);
}

UNIT_TEST(variant_assignment) {
  struct D {
    D() noexcept {}
    D(const D &) noexcept {};
    // D & operator =(const D &) = delete;
  };

  using var_t = variant<float, recursive_wrapper<D>>;
  var_t v, u;

  v.emplace<D>();
  u = static_cast<const var_t &>(v);
  v = 5.5f;
  u = 5.0f;
  v = D();
}

UNIT_TEST(variant_operator_eq) {
  using var_t = variant<float, std::string>;
  var_t u{5.0f};
  TEST_EQ(u, var_t{5.0f});
  TEST_EQ(var_t{5.0f}, u);
  TEST_NE(u, var_t{6.0f});
  TEST_NE(var_t{6.0f}, u);
  TEST_NE(u, var_t{"asdf"});
  TEST_NE(var_t{"asdf"}, u);
  TEST_NE(u, var_t{"jkl"});
  TEST_NE(var_t{"kjl"}, u);

  u = "asdf";
  TEST_NE(u, var_t{5.0f});
  TEST_NE(var_t{5.0f}, u);
  TEST_NE(u, var_t{6.0f});
  TEST_NE(var_t{6.0f}, u);
  TEST_EQ(u, var_t{"asdf"});
  TEST_EQ(var_t{"asdf"}, u);
  TEST_NE(u, var_t{"jkl"});
  TEST_NE(var_t{"kjl"}, u);
}

UNIT_TEST(variant_moving_with_recursive_wrapper) {
  using var_t = variant<recursive_wrapper<int>, recursive_wrapper<std::string>>;
  var_t x{5};
  var_t y = "foo";
  x = std::move(y);
  TEST_EQ(x.which(), 1);
  TEST_EQ(y.which(), 1);
  TEST_TRUE(get<std::string>(&x));
  TEST_TRUE(get<std::string>(&y));
  TEST_NE(get<std::string>(&x), get<std::string>(&y));
  TEST_NE(*get<std::string>(&x), *get<std::string>(&y));
  TEST_EQ(*get<std::string>(&x), "foo");
}

UNIT_TEST(variant_swap) {
  {
    using var_t = variant<int, std::string>;

    var_t x{5};
    var_t y{"foo"};
    TEST_EQ(x.which(), 0);
    TEST_EQ(y.which(), 1);

    swap(x, y);
    TEST_EQ(x.which(), 1);
    TEST_EQ(y.which(), 0);
    TEST_EQ("foo", *get<std::string>(&x));
    TEST_EQ(5, *get<int>(&y));
  }

  {
    using var_t = variant<recursive_wrapper<int>, recursive_wrapper<std::string>>;

    var_t x{5};
    var_t y{"foo"};
    TEST_EQ(x.which(), 0);
    TEST_EQ(y.which(), 1);

    swap(x, y);
    TEST_EQ(x.which(), 1);
    TEST_EQ(y.which(), 0);
    TEST_EQ("foo", *get<std::string>(&x));
    TEST_EQ(5, *get<int>(&y));
  }

  {
    using var_t = variant<int, std::string>;
    std::vector<var_t> vec;

    for (int i = 0; i < 100; ++i) {
      vec.push_back(i);
      vec.push_back(std::to_string(i));
    }

    for (int i = 0; i < 100; ++i) {
      TEST_EQ(i % 2, vec[i].which());
    }

    std::reverse(vec.begin(), vec.end());

    for (int i = 0; i < 100; ++i) {
      TEST_EQ(i % 2, 1 - vec[i].which());
    }

    std::reverse(vec.begin(), vec.end());

    for (int i = 0; i < 100; ++i) {
      TEST_EQ(i % 2, vec[i].which());
    }

    std::reverse(vec.begin(), vec.end());

    for (int i = 0; i < 100; ++i) {
      TEST_EQ(i % 2, 1 - vec[i].which());
    }
  }
}

} // end namespace strict_variant

int
main() {
  std::cout << "Variant tests:" << std::endl;
  return test_registrar::run_tests();
}
