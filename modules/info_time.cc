#include "info_time.h"
#include "colors.h"

#include <time.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: print_exe_time
 *  Description: prints execution time, given the variables 'begin'
 *  			 and 'end' supposed to contain clock times.
 * =====================================================================================
 */
void
print_exe_time ( unsigned int begin, unsigned int end ) {
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
			"Execution time: %u clock(s) = %g second(s).\n",
			end - begin, (double) ( end - begin ) / CLOCKS_PER_SEC );
}		/* -----  end of function print_exe_time  ----- */
