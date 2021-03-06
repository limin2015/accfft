/*
 *  Copyright (c) 2014-2015, Amir Gholami, George Biros
 *  All rights reserved.
 *  This file is part of AccFFT library.
 *
 *  AccFFT is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  AccFFT is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AccFFT.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include <mpi.h>
#include <fftw3.h>
#include <omp.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include "accfft_common.h"
#define VERBOSE 0
#define PCOUT if(procid==0) std::cout
typedef double Complex[2];

void accfft_create_comm(MPI_Comm in_comm,int * c_dims,MPI_Comm *c_comm){

  int nprocs, procid;
  MPI_Comm_rank(in_comm, &procid);
  MPI_Comm_size(in_comm,&nprocs);

  if(c_dims[0]*c_dims[1]!=nprocs){
    PCOUT<<"ERROR c_dims!=nprocs --> "<<c_dims[0]<<"*"<<c_dims[1]<<" !="<<nprocs<<std::endl;
    c_dims[0]=0;c_dims[1]=0;
    MPI_Dims_create(nprocs,2, c_dims);
    std::swap(c_dims[0],c_dims[1]);
    PCOUT<<"Switching to c_dims_0="<< c_dims[0]<<" , c_dims_1="<<c_dims[1]<<std::endl;
  }

  /* Create Cartesian Communicator */
  int period[2], reorder;
  int coord[2];
  period[0]=0; period[1]=0;
  reorder=1;

  MPI_Cart_create(MPI_COMM_WORLD, 2, c_dims, period, reorder, c_comm);
  PCOUT<<"dim[0]= "<<c_dims[0]<<" dim[1]= "<<c_dims[1]<<std::endl;

  //MPI_Cart_coords(c_comm, procid, 2, coord);

  return;

}
int accfft_init(){
  return 0;
}
void* accfft_alloc(ptrdiff_t size){
  void * ptr=fftw_malloc(size);
  return ptr;
}
void accfft_free(void * ptr){
  fftw_free(ptr);
  return;
}
void accfft_cleanup()
{
  fftw_cleanup_threads();
  fftw_cleanup();
}
void accfft_cleanup_gpu()
{
}
