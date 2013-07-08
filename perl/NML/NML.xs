#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "nml_wrapper.h"


MODULE = NML		PACKAGE = NML		


int
getVolumeDetail(arg0, arg1, arg2, arg3)
	const char *	arg0
	const char *	arg1
	char *	arg2
	char *	arg3

int
getVolumes(arg0, arg1)
	const char *	arg0
	char *	arg1
