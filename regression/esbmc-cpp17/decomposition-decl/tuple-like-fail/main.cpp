#include <cassert>
#include <tuple>
int main()
{
  float x{};
  char y{};
  int z{};

  std::tuple<float &, char &&, int> tpl(x, std::move(y), z);
  const auto &[a, b, c] = tpl;
  assert(a == 1234.0f);
  return 0;
}
