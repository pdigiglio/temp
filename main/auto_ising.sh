make_header () {
	echo "
#ifndef  beta_INC
#define  beta_INC

/* temperature */
#define B	${B}

#endif   /* ----- #ifndef beta_INC  ----- */" > ../include/beta.h
}
