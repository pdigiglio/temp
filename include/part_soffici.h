
#ifndef  part_soffici_INC
#define  part_soffici_INC

/*
 * ==================================================================
 *        Class: Soft
 *  Description: 
 * ==================================================================
 */
class Soft: public Particella {
	public:
		Soft (void); /* ctor */
//		Soft (const Soft &other); /* copy ctor */
		~Soft (void); /* dtor */

		/* volume side (assuming sphere diameter = 1) */
		const static double L = pow( Particella::nMax * 4 * M_PI / 3, (double) 1 / D );

		/* assignment operator */
//		Soft& operator = (const Soft &other);

//	protected:
	private:
}; /* -----  end of class Soft  ----- */

#endif   /* ----- #ifndef part_soffici_INC  ----- */
