#include <cassert>
#include <tuple>
int main()
{
  float x{};
  char y{};
  int z{};

  std::tuple<float &, char &&, int> tpl(x, std::move(y), z);
  const auto &[a, b, c] = tpl;
  assert(&a == &x);
  assert(&b == &y);
  assert(&c == &z);
  assert(a == 0.0f);
  assert(b == 0);
  assert(c == 0);
  a = 11.0f;
  b = 22;
  assert(x == 11.0f);
  assert(y == 22);
  assert(z == 0);
  z = 33;
  assert(c == 33);

  return 0;
}
