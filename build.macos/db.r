/*********************************************************************************
 * db.r  -- Resources for DB based code
 *
 * $Log: db.r,v $
 * Revision 1.1  1997/06/08 23:42:52  neeri
 * Mac 2.0.6 compiles
 *
 */

#define GUSI_PREF_VERSION '0180'

#include "GUSI.r"

resource 'GU·I' (GUSIRsrcID) {
	text, mpw, autoSpin, useChdir, approxStat, noTCPDaemon, noUDPDaemon, 
	noConsole, autoInitGraf, sharedOpen, noSigPipe, {}
};
