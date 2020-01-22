/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

double sub(double x)
{
    int iter, maxiter;
    int i, npts;
    double val = 0.0;

    npts = 1000000;
    maxiter = 1000;

    for (iter=0; iter<maxiter; iter++) {
      for (i=0; i<npts; i++) val += x * (double) (i%10);
    }

    return val;
}
