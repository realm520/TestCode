#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "nml_wrapper.h"


MODULE = NML		PACKAGE = NML		


int
getVolumeDetail(arg0, arg1, arg2, arg3)
	char *	arg0
	char *	arg1
	char *	arg2
	char *	arg3
OUTPUT:
    arg2
    arg3

int
getVolumes(arg0, arg1)
	char *	arg0
	char *	arg1
OUTPUT:
    arg1
