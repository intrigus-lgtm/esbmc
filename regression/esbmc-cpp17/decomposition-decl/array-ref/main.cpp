#include <cassert>

int main()
{
  int foo[3] = {1, 2, 3};
  auto &[aa, bb, cc] = foo;  auto &[aaa, bbb, ccc] = foo;
  assert(aa == 1);
  assert(bb == 2);
  assert(cc == 3);
  foo[0] = 44;
  foo[1] = 55;
  cc = 66;
  ccc = 77; // cc and ccc refer to the same element
  assert(aa == 44);
  assert(bb == 55);
  assert(cc == 77);

  return 0;
}
