#! /usr/bin/awk
#
# @(#)manfoot.awk	10.1 (Sleepycat) 7/27/97
#
# Remove empty blank lines from the ends of flat-text man pages.
{
	if (/^$/)
		++empty;
	else {
		for (; empty > 0; --empty)
			printf "\n";
		print $0
	}
}
