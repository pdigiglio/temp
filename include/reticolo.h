
#ifndef  reticolo_INC
#define  reticolo_INC

/* lattice side */
#define	L	10

/*
 * ==================================================================
 *        Class: Reticolo
 *  Description: 
 * ==================================================================
 */
class Reticolo {
	public:
		Reticolo (void); /* ctor */
//		Reticolo (const Reticolo &other); /* copy ctor */
		~Reticolo (void); /* dtor */

		double get_E ( void );

		/* assignment operator */
//		Reticolo& operator = (const Reticolo &other);

	protected:
		/* short-cut to extend do Potts model */
		typedef bool Sito;

		Sito x[L][L] = {};

		/* energy (initially 4L^2) */
		double E = (double) 4 * L * L;

		/* return energy of site (i,j) */
		double single_E ( unsigned int i, unsigned int j );

	private:
}; /* -----  end of class Reticolo  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: get_E
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Reticolo::get_E ( void ) {
	return (double) E;
} /* -----  end of method Reticolo::get_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: single_E
 * Description: TODO rendere elegante e efficiente!
 * ------------------------------------------------------------------
 */
inline double
Reticolo::single_E ( unsigned int i, unsigned int j ) {
	double temp = (double) 0;

	i --;
	j --;

	register unsigned short int f;

	Sito *ptr = *( x + i ) + j;
	for ( f = 0; f < 3; f += 2 ) {
		temp += *( ptr + f );
		temp += *( *( x + i + f ) + j );
	}

	temp *= (double) 2;
	temp -= 4;

	i ++;
	j ++;

	temp *= (double) 2 * *( *( x + i ) + j ) - 1;
	return temp;
} /* -----  end of method Reticolo::single_E  ----- */

#endif   /* ----- #ifndef reticolo_INC  ----- */
