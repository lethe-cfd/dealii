// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2010 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// check reinit and copy_from self-assignment for LA::MPI::SparseMatrix

#include <deal.II/base/index_set.h>

#include <deal.II/lac/generic_linear_algebra.h>

#include <iostream>
#include <vector>

#include "../tests.h"

#include "gla.h"

template <class LA>
void
test()
{
  unsigned int myid    = Utilities::MPI::this_mpi_process(MPI_COMM_WORLD);
  unsigned int numproc = Utilities::MPI::n_mpi_processes(MPI_COMM_WORLD);

  if (myid == 0)
    deallog << "numproc=" << numproc << std::endl;

  // each processor owns 2 indices and all
  // are ghosting Element 1 (the second)

  IndexSet local_active(numproc * 2);
  local_active.add_range(myid * 2, myid * 2 + 2);
  IndexSet local_relevant = local_active;
  local_relevant.add_range(0, 1);

  DynamicSparsityPattern csp(local_relevant);

  for (unsigned int i = 0; i < 2 * numproc; ++i)
    if (local_relevant.is_element(i))
      csp.add(i, i);

  csp.add(0, 1);


  typename LA::MPI::SparseMatrix mat;
  mat.reinit(local_active, local_active, csp, MPI_COMM_WORLD);

  Assert(mat.n() == numproc * 2, ExcInternalError());
  Assert(mat.m() == numproc * 2, ExcInternalError());

  // set local values
  mat.set(myid * 2, myid * 2, myid * 2.0);
  mat.set(myid * 2 + 1, myid * 2 + 1, myid * 2.0 + 1.0);

  mat.compress(VectorOperation::insert);

  mat.add(0, 1, 1.0);

  mat.compress(VectorOperation::add);

  deallog << "l1-norm: " << mat.l1_norm() << std::endl;
  if (myid == 0)
    deallog << "mat(0,1): " << get_real_assert_zero_imag(mat(0, 1))
            << std::endl;

  mat.copy_from(mat);
  deallog << "l1-norm: " << mat.l1_norm() << std::endl;
  if (myid == 0)
    deallog << "mat(0,1): " << get_real_assert_zero_imag(mat(0, 1))
            << std::endl;

  mat.reinit(mat);
  deallog << "l1-norm: " << mat.l1_norm() << std::endl;
  if (myid == 0)
    deallog << "mat(0,1): " << get_real_assert_zero_imag(mat(0, 1))
            << std::endl;

  if (myid == 0)
    deallog << "OK" << std::endl;
}



int
main(int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
  MPILogInitAll                    log;
  {
    deallog.push("PETSc");
    test<LA_PETSc>();
    deallog.pop();
    deallog.push("Trilinos");
    test<LA_Trilinos>();
    deallog.pop();
  }

  // compile, don't run
  // if (myid==9999)
  //  test<LA_Dummy>();
}
