
#ifndef  reticolo_INC
#define  reticolo_INC

#include <math.h>

/* lattice side */
#define	L	8
/* temperature */
#define B	.5

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
		double get_M ( void );

		void sweep ( void );
		void Sweep ( void );

		void print_lattice ( void );
		/* return energy of site (i,j) */
		short int single_E ( unsigned int i, unsigned int j );
		
		/* assignment operator */
//		Reticolo& operator = (const Reticolo &other);

	protected:
		/* short-cut to extend do Potts model */
		typedef bool Sito;

		/* useful constant */
		static const unsigned int L2 = L * L;
		const long double EB = (long double) 1 - expl( -B );

		Sito x[L][L] = {};

		bool ckd[L][L] = {};
		bool ckd_status = (bool) 0;
		
		/* auxiliary array to check nearest neighbours */
		const signed short int s[4][2] = {
			{ 0, 1 },
			{ 1, 0 },
			{ 0, -1 },
			{ -1, 0 }
		};

		const unsigned int head = 0;
		unsigned int tail = 0;
		unsigned int stack[L2][2];

		/* number of sweeps */
		unsigned int t = 0;

		/* energy (initially 4L^2) */
		double E = (double) - 4 * L2;

		/* current magnetization */
		double M;
		/* mean magnetization */
		double m[2] = {};

//		unsigned short int t[16] = {
//			1, 2, 4, 8,
//			16, 32, 64, 128,
//			256, 512, 1024, 2048,
//			4096, 8192, 16384, 32768
//		};



		/* return spin */
//		bool spin ( unsigned int i, unsigned int j );
		
		void cluster ( unsigned int i, unsigned int j );
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
 *      Method: get_M
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Reticolo::get_M ( void ) {
	/* normalize magnetization (divide by volume) */
	return (double) 2 * M / L2 - 1;
} /* -----  end of method Reticolo::get_M  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: single_E
 * Description: TODO rendere elegante e efficiente!
 * ------------------------------------------------------------------
 */
inline short int
Reticolo::single_E ( unsigned int i, unsigned int j ) {

	/* energy and counter */
	short int temp = 0, f;
	Sito *ptr = *( x + i );

	/* sum of spins { 0,1 } */
	for ( f = -1; f < 2; f += 2 ) {
		temp += *( ptr + ( L + j + f ) / L );
		temp += *( *( x + ( L + i + f ) / L ) + j );
	}

	/* spins { 0,1 } -> { -1, 1 } */
	temp *= 2;
	temp -= 4;

	temp *= 2 * *( *( x + i ) + j ) - 1;
	return - temp;
} /* -----  end of method Reticolo::single_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: spin
 * Description: 
 * ------------------------------------------------------------------
 */
//inline bool
//Reticolo::spin ( unsigned int i, unsigned int j ) {
//	return (bool) ( *( *( x + i) + j / 16 ) & *( t + j % 16 ) );
//} /* -----  end of method Reticolo::spin  ----- */

#endif   /* ----- #ifndef reticolo_INC  ----- */
