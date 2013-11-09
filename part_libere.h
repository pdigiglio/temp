#define N 10	/* numero particelle */
#define S .01	/* raggio particelle */
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
		struct {
			float x[3]; /* posizione della particella */
			float v[3]; /* velocità della particella */
		} p[N];

		/* matrice dei tempi di collisione */
		float **ct = NULL;

//	protected:
}; /* -----  end of class Sistema  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */
