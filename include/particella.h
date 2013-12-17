/* standard header */
#include <stdio.h>

#include "dimensions.h"

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

		virtual ~Particella (void) { /* dtor */
			fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
					ANSI_YELLOW "%s" ANSI_RESET "] destructed.\n", __func__ );
		};

	protected:
		/* record che rappresenta una particella */
		struct ptcl {
			double x[D]; /* posizione della particella */
			double v[D]; /* velocità della particella */
			double t_last = (double) 0;
		} *p = NULL;


		/* Scalar product between two vectors, a and b, in D dimensions */
		inline double sp ( const double *a, const double *b ) {
			double tmp = (double) 0;
			for ( unsigned short int d = 0; d < D; d ++ )
				tmp += *( a + d ) * *( b + d );

			return tmp;
		};

		/*
		 * The same as above: if only 'a' is given, it returns his
		 * square modulus
		 */
		inline double sp ( const double *a ) {
			return Particella::sp( a, a );
		};

}; /* -----  end of class Particella  ----- */

#endif   /* ----- #ifndef particella_INC  ----- */
