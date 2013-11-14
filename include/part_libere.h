#define N 10	/* numero particelle */
#define S .1	/* raggio particelle */
#define L 1.	/* lunghezza del lato */
#define D 3		/* dimensioni del problema */

#include <stdio.h>
#include <stdlib.h>

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
		unsigned int nMax;
		/* rapporto (intero) tra la capienza e le particelle */
		unsigned int step;

		/* record che rappresenta una particella */
		struct {
			float x[3]; /* posizione della particella */
			float v[3]; /* velocità della particella */
		} p[N];

		/* matrice dei tempi di collisione */
		float **ct = NULL;

		/* energia cinetica totale (m = 1, da dividere per 2) */
		float K;

		/* Checks if particles can be contained in volume */
		void capacity_check ( void );

//	protected:
}; /* -----  end of class Sistema  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */
