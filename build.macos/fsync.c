/*
 * fsync.c - Emulate fsync() routine.
 *
 * $Log: fsync.c,v $
 * Revision 1.1  1997/06/08 23:42:54  neeri
 * Mac 2.0.6 compiles
 *
 */
 
#include <Files.h>
#include <sys/ioctl.h>
#include <ioctl.h>

int fsync(int fd)
{
	ParamBlockRec	pb;
	FCBPBRec		fcb;
	
	if (ioctl(fd, FIOREFNUM, (long *)&pb.ioParam.ioRefNum))
		return -1;
	PBFlushFileSync(&pb);
	fcb.ioFCBIndx = 0;
	fcb.ioNamePtr = nil;
	fcb.ioRefNum  = pb.ioParam.ioRefNum;
	if (PBGetFCBInfoSync(&fcb))
		return 0;
	pb.ioParam.ioNamePtr = nil;
	pb.ioParam.ioVRefNum = fcb.ioFCBVRefNum;
	PBFlushVolSync(&pb);
	
	return 0;
}
