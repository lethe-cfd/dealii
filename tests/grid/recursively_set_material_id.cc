// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2009 - 2018 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// TriaAccessor::recursively_set_material_id had a bug in that it only
// set the material_id for two of its children, not all

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>

#include "../tests.h"



template <int dim>
void
test()
{
  for (unsigned int t = 0; t < 2; ++t)
    {
      deallog << "dim=" << dim << ", test=" << t << std::endl;

      Triangulation<dim> tr;
      if (dim > 1)
        {
          GridGenerator::hyper_ball(tr);
          tr.reset_manifold(0);
        }
      else
        GridGenerator::hyper_cube(tr);

      tr.refine_global(1);

      // for test 1, refine a second
      // time isotropically. for test
      // 2, cut in only one direction
      if (t == 0)
        tr.refine_global(1);
      else
        {
          for (typename Triangulation<dim>::active_cell_iterator cell =
                 tr.begin_active();
               cell != tr.end();
               ++cell)
            cell->set_refine_flag(RefinementCase<dim>::cut_x);
          tr.execute_coarsening_and_refinement();
        }

      deallog << tr.n_active_cells() << std::endl;

      // now call the function once
      // for one of the cells on
      // level 1
      tr.begin(1)->recursively_set_material_id(1);

      // and then count how many
      // active cells inherited this
      // flag
      unsigned int n = 0;
      for (typename Triangulation<dim>::active_cell_iterator cell =
             tr.begin_active();
           cell != tr.end();
           ++cell)
        if (cell->material_id() == 1)
          ++n;

      deallog << n << std::endl;

      if (t == 0)
        {
          AssertThrow(n == GeometryInfo<dim>::max_children_per_cell,
                      ExcInternalError());
        }
      else
        {
          AssertThrow(n == 2, ExcInternalError());
        }
    }
}


int
main()
{
  initlog();
  deallog << std::setprecision(2);

  test<1>();
  test<2>();
  test<3>();
}
