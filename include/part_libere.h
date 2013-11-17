#define D 3		/* dimensioni del problema */
#define N 50	/* sqrt[D]( numero particelle / 2 ) */
#define E .5	/* frazione d'impacchettamento $\eta$ */
//#define S .1	/* raggio particelle */

//#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef  part_libere_INC
#define  part_libere_INC

/*
 * ==================================================================
 *        Class: Sistema
 *  Description: 
 * ==================================================================
 */
class Sistema {
	public:
		Sistema (void); /* ctor */
//		Sistema (const Sistema &other); /* copy ctor */
		~Sistema (void); /* dtor */

		/* assignment operator */
//		Sistema& operator = (const Sistema &other);

	private:
		/* numero massimo di particelle nel volume */
		const unsigned long int nMax = (unsigned long) 2 * powl( N, D );
		/* 
		 * raggio delle sfere
		 *
		 * XXX Nel passaggio da 2-D e 3-D i coefficienti e le potenze
		 * cambiano (a causa del passaggio area -> volume).
		 */
		const float S = pow( (float) 3 * E / ( 4 * nMax * M_PI ), (float) 1 / D );
		/* rapporto (intero) tra la capienza e le particelle */
		unsigned int step;

		/* record che rappresenta una particella */
		struct ptcl {
			float x[3]; /* posizione della particella */
			float v[3]; /* velocità della particella */
		} *p = NULL;

		/* matrice dei tempi di collisione */
		float **ct = NULL;

		/* energia cinetica totale (m = 1) */
		float K = (float) 0;

		/* Checks if particles can be contained in volume */
		void capacity_check ( void );
		/* speed of center of mass system */
		void mass_center_speed ( void );
//	protected:
}; /* -----  end of class Sistema  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */
