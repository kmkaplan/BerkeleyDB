# Adapted manually from Makefile template for Macintosh
# @(#)Makefile.in	8.68 (Sleepycat) 2/24/97

DB185       =   :::db.1.85:
# Uncomment to build Tcl test tool
# Tcl		=   ::::tcl7.6:
# Tcl68K	=	$(Tcl)mac:Tcl-68k.lib
# TclPPC	=	$(Tcl)mac:Tcl-PPC.lib
# TclInc	= 	-i $(Tcl)generic:
# DBTEST	=   dbtest
TclInc		=	
CInc		=	-i : -i ::btree: -i ::clib: -i ::common: -i ::db: -i ::hash: 	\
				-i ::hsearch: -i ::include: -i ::lock: -i ::log: -i ::mp: 		\
				-i ::mutex: -i ::txn: -i ::os:											\
				 -cwd source -nosyspath -enum int
COpt		=	-sym on -w off ${CInc}
C68K		=	MWC68K ${COpt} -mc68020 -model far -mbg on
CPPC		=	MWCPPC ${COpt} -traceback
ROptions 	= 	-i :
Lib68K		=	MWLink68K -xm library -sym on
LibPPC		=	MWLinkPPC -xm library -sym on
LOpt		= 	-sym on -w -xm application
Link68K		=	MWLink68K ${LOpt} -model far
LinkPPC		=	MWLinkPPC ${LOpt} 
ToolLOpt	=	-sym on -w -xm mpwtool
Tool68K		=	MWLink68K ${ToolLOpt} -model far 
ToolPPC		=	MWLinkPPC ${ToolLOpt}  
Rez			=	Rez -a -t MPST -c 'MPS '

LibFiles68K	=	"{{MW68KLibraries}}MacOS.Lib"			\
				"{{MW68KLibraries}}GUSIDispatch.Lib.68K"	\
				"{{MW68KLibraries}}SIOUX.68K.Lib"		\
				"{{MW68KLibraries}}ANSI (N/4i/8d) C.68K.Lib"	\
				"{{MW68KLibraries}}MathLib68K (4i/8d).Lib"	\
				"{{MW68KLibraries}}GUSI.Lib.68K"		\
				"{{MW68KLibraries}}CPlusPlus.lib"		\
				"{{MW68KLibraries}}ToolLibs.o"			\
				"{{MW68KLibraries}}PLStringFuncs.glue.lib"

ToolFiles68K=	"{{MW68KLibraries}}MacOS.Lib"			\
				"{{MW68KLibraries}}GUSIDispatch.Lib.68K"	\
				"{{MW68KLibraries}}GUSIMPW.Lib.68K"		\
				"{{MW68KLibraries}}ToolLibs.o"			\
				"{{MW68KLibraries}}PLStringFuncs.glue.lib"	\
				"{{MW68KLibraries}}CPlusPlus.lib"		\
				"{{MW68KLibraries}}MPWRuntime.68K.Lib"			\
				"{{MW68KLibraries}}MPW ANSI (4i/8d) C.68K.Lib"	\
				"{{MW68KLibraries}}MathLib68K (4i/8d).Lib"	\
				"{{MW68KLibraries}}GUSI.Lib.68K"

LibFilesPPC	=	"{{MWPPCLibraries}}GUSI.Lib.PPC"		\
				"{{MWPPCLibraries}}SIOUX.PPC.Lib"		\
				"{{MWPPCLibraries}}MWMPWCRuntime.Lib"		\
				"{{MWPPCLibraries}}InterfaceLib"		\
				"{{MWPPCLibraries}}ANSI (NL) C++.PPC.Lib"	\
				"{{MWPPCLibraries}}ANSI (NL) C.PPC.Lib"		\
				"{{MWPPCLibraries}}PPCToolLibs.o"		\
				"{{MWPPCLibraries}}StdCLib"			\
				"{{MWPPCLibraries}}MathLib"

ToolFilesPPC=	"{{MWPPCLibraries}}GUSIMPW.Lib.PPC"		\
				"{{MWPPCLibraries}}GUSI.Lib.PPC"		\
				"{{MWPPCLibraries}}MWMPWCRuntime.Lib"		\
				"{{MWPPCLibraries}}InterfaceLib"		\
				"{{MWPPCLibraries}}ANSI (NL) C++.PPC.Lib"	\
				"{{MWPPCLibraries}}ANSI (NL) C.PPC.Lib"		\
				"{{MWPPCLibraries}}PPCToolLibs.o"		\
				"{{MWPPCLibraries}}StdCLib"			\
				"{{MWPPCLibraries}}MathLib"

.SOURCE.c	:	::btree: ::clib: ::common: ::db: ::db_archive: \
				::db_checkpoint: ::db_deadlock: ::db_dump: \
				::db_dump185: ::db_load: ::db_printlog: \
				::db_recover: ::dbm: ::examples: ::hash: ::hsearch: \
				::include: ::lock: ::log: ::mp: ::mutex: ::test: ::txn:
.SOURCE.o	:	":Obj:"
.SOURCE.68K	:	":Obj:"
.SOURCE.PPC	:	":Obj:"

%.c.68K.o		:	%.c
	Set Echo 0
	Set Src68K "{{Src68K}} $<"
%.c.PPC.o		:	%.c
	Set Echo 0
	Set SrcPPC "{{SrcPPC}} $<"
%.c.o.68K		:	%.c
	$(C68K) $(VER_FLG) -t -ext .o.68K $< -o :Obj:
%.c.o.PPC		:	%.c
	$(CPPC) $(VER_FLG) -t -ext .o.PPC $< -o :Obj:

OBJS=   bt_close.o bt_compare.o bt_conv.o bt_cursor.o bt_delete.o bt_get.o \
	bt_open.o bt_page.o bt_put.o bt_rec.o bt_rsearch.o bt_search.o \
	bt_split.o bt_stat.o btree_auto.o db.o db_appinit.o db_auto.o \
	db_byteorder.o db_conv.o db_dispatch.o db_dup.o db_err.o \
	db_log2.o db_os_abs.o db_os_dir.o db_os_fid.o db_os_lseek.o \
	db_os_mmap.o db_os_open.o db_os_rw.o db_os_sleep.o db_os_stat.o \
	db_os_unlink.o db_overflow.o db_pr.o db_rec.o db_region.o db_ret.o \
	db_salloc.o db_shash.o hash.o hash_auto.o hash_conv.o hash_debug.o \
	hash_dup.o hash_func.o hash_page.o hash_rec.o hash_stat.o lock.o \
	lock_conflict.o lock_util.o log.o log_auto.o log_compare.o \
	log_get.o log_put.o log_rec.o log_register.o mp_bh.o mp_fget.o \
	mp_fopen.o mp_fput.o mp_fset.o mp_open.o mp_pr.o mp_region.o \
	mp_sync.o mutex.o txn.o txn_auto.o txn_rec.o dbm.o hsearch.o
OBJ68K	= $(OBJS:s/.o/.c.68K.o/)
OBJPPC	= $(OBJS:s/.o/.c.PPC.o/)

PROGS=	db_archive db_checkpoint db_deadlock db_dump db_load \
	db_recover 

all: libdb db_archive db_checkpoint db_deadlock db_dump db_load db_printlog db_recover 

LOBJS = snprintf.o strsep.o strdup.o fsync.o mmap.o getpid.o
POBJS = err.o getlong.o getopt.o

libdb: db.Lib.PPC db.Lib.68K
db.Lib.PPC: ObjectsPPC $(LOBJS:s/.o/.c.o.PPC/)
	$(LibPPC) -o $@ :Obj:{$(OBJPPC) $(LOBJS:s/.o/.c.o.PPC/)}
db.Lib.68K: Objects68K $(LOBJS:s/.o/.c.o.68K/)
	$(Lib68K) -o $@ :Obj:{$(OBJ68K) $(LOBJS:s/.o/.c.o.68K/)}

DBA=	db_archive.o db_findckp.o $(POBJS)
db_archive: db_archive.PPC db_archive.68K
	FattenTool db_archive
db_archive.PPC: db.Lib.PPC $(DBA:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBA:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_archive.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_archive.68K: db.Lib.68K $(DBA:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBA:s/.o/.c.o.68K/)}
db_archive.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBB=	db_checkpoint.o  $(POBJS)
db_checkpoint: db_checkpoint.PPC db_checkpoint.68K
	FattenTool db_checkpoint
db_checkpoint.PPC: db.Lib.PPC $(DBB:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBB:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_checkpoint.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_checkpoint.68K: db.Lib.68K $(DBB:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBB:s/.o/.c.o.68K/)}
db_checkpoint.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBC=	db_deadlock.o $(POBJS)
db_deadlock: db_deadlock.PPC db_deadlock.68K
	FattenTool db_deadlock
db_deadlock.PPC: db.Lib.PPC $(DBC:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBC:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_deadlock.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_deadlock.68K: db.Lib.68K $(DBC:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBC:s/.o/.c.o.68K/)}
db_deadlock.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBD=	db_dump.o $(POBJS)
db_dump: db_dump.PPC db_dump.68K
	FattenTool db_dump
db_dump.PPC: db.Lib.PPC $(DBD:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBD:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_dump.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_dump.68K: db.Lib.68K $(DBD:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBD:s/.o/.c.o.68K/)}
db_dump.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBE=	db_dump185.o $(POBJS)
db_dump185: db_dump185.PPC db_dump185.68K
	FattenTool db_dump185
db_dump185.PPC: db.Lib.PPC $(DBE:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) $(DB185)db.Lib.PPC :Obj:{$(DBE:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_dump185.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_dump185.68K: db.Lib.68K $(DBE:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{$(DBE:s/.o/.c.o.68K/)} $(DB185)db.Lib.68K
db_dump185.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBF=	db_load.o $(POBJS)
db_load: db_load.PPC db_load.68K
	FattenTool db_load
db_load.PPC: db.Lib.PPC $(DBF:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBF:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_load.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_load.68K: db.Lib.68K $(DBF:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBF:s/.o/.c.o.68K/)}
db_load.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBG=	db_printlog.o $(POBJS)
db_printlog: db_printlog.PPC db_printlog.68K
	FattenTool db_printlog
db_printlog.PPC: db.Lib.PPC $(DBG:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBG:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_printlog.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_printlog.68K: db.Lib.68K $(DBG:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBG:s/.o/.c.o.68K/)}
db_printlog.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBH=	db_recover.o db_findckp.o $(POBJS)
db_recover: db_recover.PPC db_recover.68K
	FattenTool db_recover
db_recover.PPC: db.Lib.PPC $(DBH:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBH:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
db_recover.PPC:: db.r
	$(Rez) db.r -o $(@:f)
db_recover.68K: db.Lib.68K $(DBH:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBH:s/.o/.c.o.68K/)}
db_recover.68K:: db.r
	$(Rez) db.r -o $(@:f)

EPROGS=	ex_access ex_appinit ex_lock ex_mpool ex_tpcb
test : $(EPROGS) $(DBTEST)

DBI=	ex_access.o $(POBJS)
ex_access: ex_access.PPC ex_access.68K
	FattenTool ex_access
ex_access.PPC: db.Lib.PPC $(DBI:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBI:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
ex_access.PPC:: db.r
	$(Rez) db.r -o $(@:f)
ex_access.68K: db.Lib.68K $(DBI:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBI:s/.o/.c.o.68K/)}
ex_access.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBJ=	ex_appinit.o $(POBJS)
ex_appinit: ex_appinit.PPC ex_appinit.68K
	FattenTool ex_appinit
ex_appinit.PPC: db.Lib.PPC $(DBJ:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBJ:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
ex_appinit.PPC:: db.r
	$(Rez) db.r -o $(@:f)
ex_appinit.68K: db.Lib.68K $(DBJ:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBJ:s/.o/.c.o.68K/)}
ex_appinit.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBK=	ex_lock.o $(POBJS)
ex_lock: ex_lock.PPC ex_lock.68K
	FattenTool ex_lock
ex_lock.PPC: db.Lib.PPC $(DBK:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBK:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
ex_lock.PPC:: db.r
	$(Rez) db.r -o $(@:f)
ex_lock.68K: db.Lib.68K $(DBK:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBK:s/.o/.c.o.68K/)}
ex_lock.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBL=	ex_mpool.o $(POBJS)
ex_mpool: ex_mpool.PPC ex_mpool.68K
	FattenTool ex_mpool
ex_mpool.PPC: db.Lib.PPC $(DBL:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBL:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
ex_mpool.PPC:: db.r
	$(Rez) db.r -o $(@:f)
ex_mpool.68K: db.Lib.68K $(DBL:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBL:s/.o/.c.o.68K/)}
ex_mpool.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBM=	ex_tpcb.o $(POBJS)
ex_tpcb: ex_tpcb.PPC ex_tpcb.68K
	FattenTool ex_tpcb
ex_tpcb.PPC: db.Lib.PPC $(DBM:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBM:s/.o/.c.o.PPC/)} $(ToolFilesPPC)
ex_tpcb.PPC:: db.r
	$(Rez) db.r -o $(@:f)
ex_tpcb.68K: db.Lib.68K $(DBM:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBM:s/.o/.c.o.68K/)}
ex_tpcb.68K:: db.r
	$(Rez) db.r -o $(@:f)

DBT=	tclAppInit.o tcl_dbm.o tcl_hsearch.o tcl_lock.o tcl_log.o tcl_mpool.o \
	tcl_mutex.o tcl_ndbm.o tcl_txn.o utils.o $(POBJS)

dbtest: .dbtestrc
dbtest: dbtest.PPC dbtest.68K
	FattenTool dbtest
dbtest.PPC: db.Lib.PPC $(DBT:s/.o/.c.o.PPC/)
	$(ToolPPC) -o $(@:f) :Obj:{db.Lib.PPC $(DBT:s/.o/.c.o.PPC/)} $(ToolFilesPPC) $(TclPPC)
dbtest.PPC:: db.r
	$(Rez) db.r -o $(@:f)
dbtest.68K: db.Lib.68K $(DBT:s/.o/.c.o.68K/)
	$(Tool68K) -o $(@:f)  $(ToolFiles68K) :Obj:{db.Lib.68K $(DBT:s/.o/.c.o.68K/)} $(Tcl68K)
dbtest.68K:: db.r
	$(Rez) db.r -o $(@:f)

.dbtestrc: db.h ::test:test.tcl
	Delete -i -y $@
	catenate ::test:test.tcl > $@

clean:
	$(rm) -f $(OBJS) $(A) $(B) $(C) $(D) $(E) $(F) $(G) $(H) $(T)
	$(rm) -f $(PROGS) 
	$(rm) -f core *.core .tcldbrc $(libdb)

realclean distclean: clean
	$(rm) -f Makefile config.cache config.h config.log config.status
	$(rm) -f db.h db_int.h include.tcl

Obj :	
	NewFolder Obj

":sys/mman.h" : sys_mman.h
	Duplicate -y sys_mman.h 'sys/mman.h'

Objects68K	: Obj :sys/mman.h $(OBJ68K) 
	Set Echo 1
	If "{{Src68K}}" != "" 
		${C68K} -t -fatext {{Src68K}} -o :Obj:
	End
	Echo > Objects68K
	
ObjectsPPC	: Obj :sys/mman.h $(OBJPPC) 	 	
	Set Echo 1
	If "{{SrcPPC}}" != "" 
		${CPPC} -t -fatext {{SrcPPC}} -o :Obj:
	End
	Echo > ObjectsPPC
