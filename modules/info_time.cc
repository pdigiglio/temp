#include "info_time.h"
#include "colors.h"

#include <stdio.h>
#include <time.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: print_exe_time
 *  Description: prints execution time, given the variable 'begin'. If
 *  			 the string 'func' is given, it's printed.
 * =====================================================================================
 */
void
print_exe_time ( unsigned int begin, const char *func ) {
	/* take current time */
	unsigned int end = clock();
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET );
	
	/* if the function name is given */
	if ( func )
		fprintf( stderr, ": " ANSI_YELLOW "%s" ANSI_RESET, func );

	fprintf( stderr, "] Execution time: %u clock(s) = %g second(s).\n",
			end - begin, (float) ( end - begin ) / CLOCKS_PER_SEC );
}		/* -----  end of function print_exe_time  ----- */
