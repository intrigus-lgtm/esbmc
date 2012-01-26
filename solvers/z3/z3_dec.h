/*******************************************************************\

Module:

Author: Lucas Cordeiro, lcc08r@ecs.soton.ac.uk

\*******************************************************************/

#ifndef CPROVER_PROP_z3_DEC_H
#define CPROVER_PROP_z3_DEC_H

#include <fstream>

#include "z3_conv.h"

class z3_dect: public z3_convt
{
public:
  z3_dect(bool uw, bool int_encoding, bool smtlib)
    :z3_convt(uw, int_encoding, smtlib)
  {
    this->smtlib = smtlib;
  }

  virtual resultt dec_solve();
  void set_file(std::string file);
  void set_unsat_core(uint val);

protected:
  resultt read_z3_result();

private:
  bool smtlib;

};

#endif
