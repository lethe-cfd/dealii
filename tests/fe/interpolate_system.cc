// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2005 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


#include <deal.II/base/quadrature_lib.h>

#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_system.h>

#include "interpolate_common.h"


// FE_Q<dim>::interpolate(...)

template <int dim>
void
check1(const Function<dim> &f,
       const unsigned int   degree,
       const unsigned int   comp)
{
  FE_Q<dim>     feq(degree);
  FESystem<dim> fe(feq, comp);
  deallog << fe.get_name() << ' ';

  std::vector<double> dofs(fe.dofs_per_cell);

  std::vector<Vector<double>> values(fe.get_generalized_support_points().size(),
                                     Vector<double>(comp));
  f.vector_value_list(fe.get_generalized_support_points(), values);
  fe.convert_generalized_support_point_values_to_dof_values(values, dofs);
  deallog << " vector " << vector_difference(fe, dofs, f, 0) << std::endl;
}

template <int dim>
void
check3(const Function<dim> &f,
       const unsigned int   degree,
       const unsigned int   comp1,
       const unsigned int   comp2,
       const unsigned int   comp3)
{
  FE_Q<dim>     feq1(degree);
  FE_Q<dim>     feq2(degree + 1);
  FE_Q<dim>     feq3(degree + 2);
  FESystem<dim> fe(feq1, comp1, feq2, comp2, feq3, comp3);
  deallog << fe.get_name() << ' ';

  std::vector<double> dofs(fe.dofs_per_cell);

  std::vector<Vector<double>> values(fe.get_generalized_support_points().size(),
                                     Vector<double>(f.n_components));
  f.vector_value_list(fe.get_generalized_support_points(), values);
  fe.convert_generalized_support_point_values_to_dof_values(values, dofs);
  deallog << " vector " << vector_difference(fe, dofs, f, 0) << std::endl;
}

int
main()
{
  initlog();

  Q1WedgeFunction<1, 1, 2> w1;
  check1(w1, 1, 2);
  check1(w1, 2, 2);
  check1(w1, 3, 2);
  Q1WedgeFunction<2, 2, 3> w2;
  check1(w2, 2, 3);
  check1(w2, 3, 3);
  Q1WedgeFunction<3, 1, 3> w3;
  check1(w3, 1, 3);
  check1(w3, 2, 3);
  Q1WedgeFunction<2, 1, 9> www2;
  check3(www2, 1, 2, 3, 4);
  Q1WedgeFunction<3, 1, 9> www3;
  check3(www3, 1, 2, 3, 4);
}
