/*************************************************************************/
/*                                                                       */
/*             FILE   :  MYRANDOM.H                                      */
/*             AUTHOR :  J.J. COLLINS                                    */
/*             DATE   :  AUG - SEPT 1996                                 */
/*             SCHOOL :  Dept. Computer Science                          */
/*                       University of Limerick                          */
/*                                                                       */
/*  -------------------------------------------------------------------  */
/*    FUNCTIONALITY :                                                    */
/* Linear Congruential method for generating random numbers as in book   */
/* "Algorithms in C by Robert Sedgewick", Addison-Wesley 1990, p509-512. */
/*  -------------------------------------------------------------------  */
/*     BUGS : todate, none to report.                                    */
/*                                                                       */
/*************************************************************************/

#ifndef _MYRANDOM_H
#define _MYRANDOM_H

#include <boost/random.hpp>


// int seed;
typedef boost::mt19937 base_generator_type;
typedef boost::uniform_int<> distribution_type;
typedef boost::variate_generator<base_generator_type&,distribution_type> gen_type;


/*************************************************************************/

  void myrandomize();
  int mult(int p, int q);
  int myrandom();
  long randomint(long r);
  int randombool();
  double randreal();
  float chisquare(int N, int r, int &s);
  int probably(const double pos);

/*************************************************************************/
#endif

