/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997, 1998
 *	Sleepycat Software.  All rights reserved.
 */

#include "config.h"

#ifndef lint
static const char sccsid[] = "@(#)os_spin.c	10.4 (Sleepycat) 4/14/98";
#endif /* not lint */

/*
 * __os_spin --
 *	Return the number of default spins before blocking.
 *
 * PUBLIC: int __os_spin __P((void));
 */
int
__os_spin()
{
	extern int __db_tsl_spins;
	static long sys_val;
        SYSTEM_INFO SystemInfo;

	/* If the application specified the spins, use its value. */
	if (__db_tsl_spins != 0)
		return (__db_tsl_spins);

	/* If we've already figured this out, return the value. */
	if (sys_val != 0)
		return (sys_val);

	/* Get the number of processors */
        GetSystemInfo(&SystemInfo);

	/*
	 * Spin 50 times per processor -- we have anecdotal evidence that this
	 * is a reasonable value.
	 */
	if (SystemInfo.dwNumberOfProcessors > 1)
		sys_val = 50 * SystemInfo.dwNumberOfProcessors;
	else
		sys_val = 1;
	return (sys_val);
}
