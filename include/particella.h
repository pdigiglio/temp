
/* TODO separare la classe "particella" da quella di "part_libere" */

#include "parameters.h"
#include <stdio.h>


#ifndef  particella_INC
#define  particella_INC

/*
 * ==================================================================
 *        Class: Particella
 *  Description: base class for system with more particles
 * ==================================================================
 */
class Particella {
	public:	
		Particella ( void ) {}; /* ctor */
		virtual ~Particella (void) {}; /* dtor */

	protected:
		/* record che rappresenta una particella */
		struct ptcl {
			double x[D]; /* posizione della particella */
			double v[D]; /* velocità della particella */
			double t_last = (double) 0;
//			unsigned int crash = 0;
//			double t[2] = {};
		} *p = NULL;

		/* scalar product */
		double sp ( const double *a, const double *b );
		/* square modulus */
		double sp ( const double *a );
}; /* -----  end of class Particella  ----- */

#endif   /* ----- #ifndef particella_INC  ----- */
