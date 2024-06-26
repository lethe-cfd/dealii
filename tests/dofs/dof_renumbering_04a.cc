// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2007 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// an extract of the _04 test that produced different (integer)
// results on different machines, inexplicably


#include <deal.II/base/function_lib.h>

#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_renumbering.h>

#include <deal.II/fe/fe_dgp.h>
#include <deal.II/fe/fe_dgq.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/fe_values.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_iterator.h>

#include <deal.II/lac/vector.h>

#include "../tests.h"



template <int dim>
void
print_dofs(const DoFHandler<dim> &dof)
{
  std::vector<types::global_dof_index> v(dof.get_fe().dofs_per_cell);

  for (typename DoFHandler<dim>::active_cell_iterator cell = dof.begin_active();
       cell != dof.end();
       ++cell)
    {
      cell->get_dof_indices(v);
      deallog << "cell=" << cell << std::endl;
      for (unsigned int i = 0; i < v.size(); ++i)
        deallog << v[i] << std::endl;
    }
}



template <int dim>
void
check()
{
  Triangulation<dim> tr;
  GridGenerator::hyper_cube(tr, -1., 1.);

  FESystem<dim>   fe(FE_Q<dim>(2), 2);
  DoFHandler<dim> dof(tr);

  dof.distribute_dofs(fe);
  std::vector<types::global_dof_index> new_dofs(dof.n_dofs());
  DoFRenumbering::boost::compute_Cuthill_McKee(new_dofs, dof);

  for (unsigned int i = 0; i < new_dofs.size(); ++i)
    deallog << new_dofs[i] << std::endl;
}


int
main()
{
  initlog();
  deallog << std::setprecision(2) << std::fixed;

  check<2>();
}
