/*
 * mmap.c - Emulate mmap() and munmap().
 *
 * $Log: mmap.c,v $
 * Revision 1.1  1997/06/08 23:42:56  neeri
 * Mac 2.0.6 compiles
 *
 */

#include <Memory.h>
#include <Files.h>
#include <TextUtils.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <ioctl.h>
#include <errno.h>
#include <sys/errno.h>

#include "sys/mman.h"

typedef struct {
	void * 	addr;
	size_t	len;
	short	fRef;
	Boolean	writeBack;
	off_t	offset;
} MMap, **MMapHdl;

static MMapHdl	gMMaps;
static int		gMMapCount;

static void flushmmaps()
{
	while (gMMapCount)
		munmap(gMMaps[0]->addr, gMMaps[0]->len);
}

void * mmap(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
#pragma unused(addr)
	ParamBlockRec	pb;
	Ptr				memory;
	MMap			map;
	
	if (ioctl(fd, FIOREFNUM, (long *)&pb.ioParam.ioRefNum)) {
		errno = EBADF;
		return 0;
	}
	if (!(memory = NewPtr(len))) {
		errno = ENOMEM;
		return 0;
	}
	pb.ioParam.ioBuffer	  	= memory;
	pb.ioParam.ioReqCount 	= len;
	pb.ioParam.ioPosMode	= fsFromStart;
	pb.ioParam.ioPosOffset	= offset;
	switch (PBReadSync(&pb)) {
	case 0:
	case -39: /* EOF */
		break;
	default:
		errno = EACCES;
		
		goto failed;
	}
	if (!gMMaps) {
		gMMaps = (MMapHdl)NewHandle(0);
		atexit(flushmmaps);
	}
	map.addr 		= memory;
	map.len	 		= len;
	map.fRef 		= pb.ioParam.ioRefNum;
	map.writeBack	= (prot & PROT_WRITE) != 0;
	map.offset		= offset;
	
	if (!gMMaps || PtrAndHand((Ptr) &map, (Handle)gMMaps, sizeof(MMap))) {
		errno = ENOMEM;
		goto failed;
	}
	++gMMapCount;
	
	return memory;
failed:
	DisposePtr(memory);
	
	return 0;
}

static void purgemap(const MMap map)
{
	if (map.writeBack) {
		ParamBlockRec	pb;

		pb.ioParam.ioRefNum	  	= map.fRef;
		pb.ioParam.ioBuffer	  	= map.addr;
		pb.ioParam.ioReqCount 	= map.len;
		pb.ioParam.ioPosMode	= fsFromStart;
		pb.ioParam.ioPosOffset	= map.offset;
		
		PBWriteSync(&pb);
	}
	DisposePtr(map.addr);
}

int munmap(void * addr, size_t len)
{
#pragma unused(len)
	int i;
	
	for (i = 0; i<gMMapCount; ++i)
		if (gMMaps[0][i].addr == addr) {
			purgemap(gMMaps[0][i]);
			Munger((Handle) gMMaps, i*sizeof(MMap), nil, sizeof(MMap), (Ptr)-1, 0);
			--gMMapCount;

			return 0;
		}
	errno = EINVAL;
	
	return -1;
}
