#include <cassert>

int main()
{
  int foo[3] = {1, 2, 3};
  auto &[aa, bb, cc] = foo;
  assert(aa == 1);
  assert(bb == 2);
  assert(cc == 3);
  foo[0] = 44;
  assert(aa == 1);

  return 0;
}
