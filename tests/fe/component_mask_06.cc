// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2012 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// tests for the ComponentMask class
//
// here: ComponentMask::n_selected_components


#include <deal.II/fe/component_mask.h>

#include "../tests.h"



void
test()
{
  // test for an initialized mask
  Assert(ComponentMask(12, true).n_selected_components() == 12,
         ExcInternalError());
  Assert(ComponentMask(12, true).n_selected_components(12) == 12,
         ExcInternalError());
  // test for an empty mask
  Assert(ComponentMask().n_selected_components(12) == 12, ExcInternalError());
  Assert(ComponentMask().n_selected_components(13) == 13, ExcInternalError());


  deallog << "OK" << std::endl;

  // this now must throw an exception,
  // though:
  try
    {
      Assert(ComponentMask(12, true).n_selected_components(13) == 12,
             ExcInternalError());
    }
  catch (ExceptionBase &e)
    {
      deallog << e.get_exc_name() << std::endl;
    }
}


int
main()
{
  deal_II_exceptions::disable_abort_on_exception();

  initlog();
  deallog << std::setprecision(4);

  test();
}
