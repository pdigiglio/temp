#include <stdio.h>

#ifndef  round_INC
#define  round_INC

void round ( long double val, long double err, FILE *stream = stdout );
void round ( double val, double err, FILE *stream = stdout );

// TODO
//void round ( double val, double err );
//void round ( float val, float err );

#endif   /* ----- #ifndef round_INC  ----- */
