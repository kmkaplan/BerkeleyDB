#! /usr/bin/awk
#
# @(#)manpb.awk	10.2 (Sleepycat) 8/14/97
#
# Remove 6 line headers and footers from the flat-text man pages, except that
# the first page of each man page keeps its header.
#
# If a page other than the first begins with a capital letter followed by a
# whitespace or lower-case letter, add in an extra line, it's (hopefully) a
# paragraph that happened to be on a page break, and we want a separating line.
BEGIN {
	hdsz=6
	ftsz=6
	pgsz=66
	txtsz=pgsz - (hdsz + ftsz)

	lineno=1
	pageno=1
}
{
	if (lineno <= hdsz) {
		if (pageno == 1)
			print $0;
	} else if (lineno > hdsz && lineno <= hdsz + txtsz) {
		if (pageno > 1 && 
		    lineno == hdsz + 1 && $0 ~ /^[ 	]*[A-Z][a-z 	]/)
			printf "\n"
		print $0;
	}
	if (lineno++ == pgsz) {
		++pageno;
		lineno = 1;
	}
}
