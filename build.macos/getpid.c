/*
 * getpid.c - Emulate getpid() routine.
 *
 * $Log: getpid.c,v $
 * Revision 1.1  1997/06/08 23:42:55  neeri
 * Mac 2.0.6 compiles
 *
 */

#include <sys/types.h> 
#include <Processes.h>

pid_t getpid()
{
	ProcessSerialNumber	psn;
	
	GetCurrentProcess(&psn);
	
	return (pid_t)psn.lowLongOfPSN;
}
