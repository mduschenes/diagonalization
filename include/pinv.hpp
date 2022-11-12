#ifndef _PINV_
#define _PINV_


#include <iostream>
#include <cstddef>
#include <vector> 
#include <algorithm>
#include <mkl_lapacke.h>

#include "itensor/all.h"
#include "itensor/util/print_macro.h"



void pinv(double * a, double * b, int _m, int _n, int _nrhs);

#endif
