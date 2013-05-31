/* DO NOT EDIT: automatically built by dist/s_java_const. */

package com.sleepycat.db.internal;

public interface DbConstants
{
    int DB_AFTER = 1;
    int DB_AGGRESSIVE = 0x00000001;
    int DB_APPEND = 2;
    int DB_ARCH_ABS = 0x00000001;
    int DB_ARCH_DATA = 0x00000002;
    int DB_ARCH_LOG = 0x00000004;
    int DB_ARCH_REMOVE = 0x00000008;
    int DB_ASSOC_IMMUTABLE_KEY = 0x00000001;
    int DB_ASSOC_CREATE = 0x00000002;
    int DB_AUTO_COMMIT = 0x00000100;
    int DB_BACKUP_CLEAN = 0x00000002;
    int DB_BACKUP_FILES = 0x00000008;
    int DB_BACKUP_NO_LOGS = 0x00000010;
    int DB_BACKUP_READ_COUNT = 1;
    int DB_BACKUP_READ_SLEEP = 2;
    int DB_BACKUP_SINGLE_DIR = 0x00000020;
    int DB_BACKUP_SIZE = 3;
    int DB_BACKUP_UPDATE = 0x00000040;
    int DB_BACKUP_WRITE_DIRECT = 4;
    int DB_BEFORE = 3;
    int DB_BOOTSTRAP_HELPER = 0x00000001;
    int DB_BTREE = 1;
    int DB_BUFFER_SMALL = -30999;
    int DB_CDB_ALLDB = 0x00000040;
    int DB_CHKSUM = 0x00000008;
    int DB_CONSUME = 4;
    int DB_CONSUME_WAIT = 5;
    int DB_CREATE = 0x00000001;
    int DB_CURRENT = 6;
    int DB_CURSOR_BULK = 0x00000001;
    int DB_DBT_BLOB = 0x1000;
    int DB_DBT_BULK = 0x0002;
    int DB_DBT_MALLOC = 0x0010;
    int DB_DBT_PARTIAL = 0x0040;
    int DB_DBT_READONLY = 0x0100;
    int DB_DBT_USERMEM = 0x0800;
    int DB_DIRECT_DB = 0x00000200;
    int DB_DSYNC_DB = 0x00000400;
    int DB_DUP = 0x00000010;
    int DB_DUPSORT = 0x00000002;
    int DB_EID_BROADCAST = -1;
    int DB_EID_INVALID = -2;
    int DB_EID_MASTER = -3;
    int DB_ENCRYPT = 0x00000001;
    int DB_ENCRYPT_AES = 0x00000001;
    int DB_EXCL = 0x00000004;
    int DB_FAST_STAT = 0x00000001;
    int DB_FIRST = 7;
    int DB_FLUSH = 0x00000002;
    int DB_FORCE = 0x00000001;
    int DB_FORCESYNC = 0x00000001;
    int DB_FOREIGN_ABORT = 0x00000001;
    int DB_FOREIGN_CASCADE = 0x00000002;
    int DB_FOREIGN_NULLIFY = 0x00000004;
    int DB_FREELIST_ONLY = 0x00000001;
    int DB_FREE_SPACE = 0x00000002;
    int DB_GET_BOTH = 8;
    int DB_GET_BOTH_RANGE = 10;
    int DB_GET_RECNO = 11;
    int DB_GROUP_CREATOR = 0x00000002;
    int DB_HASH = 2;
    int DB_HEAP = 6;
    int DB_HOTBACKUP_IN_PROGRESS = 0x00000800;
    int DB_IGNORE_LEASE = 0x00001000;
    int DB_IMMUTABLE_KEY = 0x00000002;
    int DB_INIT_CDB = 0x00000080;
    int DB_INIT_LOCK = 0x00000100;
    int DB_INIT_LOG = 0x00000200;
    int DB_INIT_MPOOL = 0x00000400;
    int DB_INIT_REP = 0x00001000;
    int DB_INIT_TXN = 0x00002000;
    int DB_INORDER = 0x00000020;
    int DB_JOINENV = 0x0;
    int DB_JOIN_ITEM = 12;
    int DB_JOIN_NOSORT = 0x00000001;
    int DB_KEYEMPTY = -30995;
    int DB_KEYEXIST = -30994;
    int DB_KEYFIRST = 13;
    int DB_KEYLAST = 14;
    int DB_LAST = 15;
    int DB_LEGACY = 0x00000004;
    int DB_LOCAL_SITE = 0x00000008;
    int DB_LOCKDOWN = 0x00004000;
    int DB_LOCK_DEFAULT = 1;
    int DB_LOCK_EXPIRE = 2;
    int DB_LOCK_GET = 1;
    int DB_LOCK_GET_TIMEOUT = 2;
    int DB_LOCK_IREAD = 5;
    int DB_LOCK_IWR = 6;
    int DB_LOCK_IWRITE = 4;
    int DB_LOCK_MAXLOCKS = 3;
    int DB_LOCK_MAXWRITE = 4;
    int DB_LOCK_MINLOCKS = 5;
    int DB_LOCK_MINWRITE = 6;
    int DB_LOCK_NOTGRANTED = -30992;
    int DB_LOCK_NOWAIT = 0x00000004;
    int DB_LOCK_OLDEST = 7;
    int DB_LOCK_PUT = 4;
    int DB_LOCK_PUT_ALL = 5;
    int DB_LOCK_PUT_OBJ = 6;
    int DB_LOCK_RANDOM = 8;
    int DB_LOCK_READ = 1;
    int DB_LOCK_TIMEOUT = 8;
    int DB_LOCK_WRITE = 2;
    int DB_LOCK_YOUNGEST = 9;
    int DB_LOG_AUTO_REMOVE = 0x00000001;
    int DB_LOG_BLOB = 0x00000002;
    int DB_LOG_DIRECT = 0x00000004;
    int DB_LOG_DSYNC = 0x00000008;
    int DB_LOG_IN_MEMORY = 0x00000010;
    int DB_LOG_ZERO = 0x00000020;
    int DB_MEM_LOCK = 1;
    int DB_MEM_LOCKOBJECT = 2;
    int DB_MEM_LOCKER = 3;
    int DB_MEM_LOGID = 4;
    int DB_MEM_TRANSACTION = 5;
    int DB_MEM_THREAD = 6;
    int DB_MPOOL_NOFILE = 0x00000001;
    int DB_MPOOL_UNLINK = 0x00000002;
    int DB_MULTIPLE = 0x00000800;
    int DB_MULTIPLE_KEY = 0x00004000;
    int DB_MULTIVERSION = 0x00000008;
    int DB_NEXT = 16;
    int DB_NEXT_DUP = 17;
    int DB_NEXT_NODUP = 18;
    int DB_NODUPDATA = 19;
    int DB_NOLOCKING = 0x00002000;
    int DB_NOMMAP = 0x00000010;
    int DB_NOORDERCHK = 0x00000002;
    int DB_NOOVERWRITE = 20;
    int DB_NOPANIC = 0x00004000;
    int DB_NOSYNC = 0x00000001;
    int DB_NOTFOUND = -30988;
    int DB_ORDERCHKONLY = 0x00000004;
    int DB_OVERWRITE = 0x00008000;
    int DB_PANIC_ENVIRONMENT = 0x00010000;
    int DB_POSITION = 22;
    int DB_PREV = 23;
    int DB_PREV_DUP = 24;
    int DB_PREV_NODUP = 25;
    int DB_PRINTABLE = 0x00000008;
    int DB_PRIORITY_DEFAULT = 3;
    int DB_PRIORITY_HIGH = 4;
    int DB_PRIORITY_LOW = 2;
    int DB_PRIORITY_VERY_HIGH = 5;
    int DB_PRIORITY_VERY_LOW = 1;
    int DB_PRIVATE = 0x00010000;
    int DB_QUEUE = 4;
    int DB_RDONLY = 0x00000400;
    int DB_READ_COMMITTED = 0x00000400;
    int DB_READ_UNCOMMITTED = 0x00000200;
    int DB_RECNO = 3;
    int DB_RECNUM = 0x00000040;
    int DB_RECOVER = 0x00000002;
    int DB_RECOVER_FATAL = 0x00020000;
    int DB_REGION_INIT = 0x00020000;
    int DB_REGISTER = 0x00040000;
    int DB_RENUMBER = 0x00000080;
    int DB_REPMGR_ACKS_ALL = 1;
    int DB_REPMGR_ACKS_ALL_AVAILABLE = 2;
    int DB_REPMGR_ACKS_ALL_PEERS = 3;
    int DB_REPMGR_ACKS_NONE = 4;
    int DB_REPMGR_ACKS_ONE = 5;
    int DB_REPMGR_ACKS_ONE_PEER = 6;
    int DB_REPMGR_ACKS_QUORUM = 7;
    int DB_REPMGR_CONF_2SITE_STRICT = 0x00000001;
    int DB_REPMGR_CONF_ELECTIONS = 0x00000002;
    int DB_REPMGR_CONNECTED = 1;
    int DB_REPMGR_DISCONNECTED = 2;
    int DB_REPMGR_NEED_RESPONSE = 0x00000001;
    int DB_REPMGR_ISPEER = 0x01;
    int DB_REPMGR_ISVIEW = 0x02;
    int DB_REPMGR_PEER = 0x00000010;
    int DB_REP_ACK_TIMEOUT = 1;
    int DB_REP_ANYWHERE = 0x00000001;
    int DB_REP_CHECKPOINT_DELAY = 2;
    int DB_REP_CLIENT = 0x00000001;
    int DB_REP_CONF_AUTOINIT = 0x00000004;
    int DB_REP_CONF_BULK = 0x00000010;
    int DB_REP_CONF_DELAYCLIENT = 0x00000020;
    int DB_REP_CONF_INMEM = 0x00000040;
    int DB_REP_CONF_LEASE = 0x00000080;
    int DB_REP_CONF_NOWAIT = 0x00000100;
    int DB_REP_CONNECTION_RETRY = 3;
    int DB_REP_DEFAULT_PRIORITY = 100;
    int DB_REP_ELECTION = 0x00000004;
    int DB_REP_ELECTION_RETRY = 4;
    int DB_REP_ELECTION_TIMEOUT = 5;
    int DB_REP_FULL_ELECTION_TIMEOUT = 6;
    int DB_REP_HEARTBEAT_MONITOR = 7;
    int DB_REP_HEARTBEAT_SEND = 8;
    int DB_REP_IGNORE = -30982;
    int DB_REP_ISPERM = -30981;
    int DB_REP_LEASE_TIMEOUT = 9;
    int DB_REP_MASTER = 0x00000002;
    int DB_REP_NEWSITE = -30977;
    int DB_REP_NOBUFFER = 0x00000002;
    int DB_REP_NOTPERM = -30976;
    int DB_REP_PERMANENT = 0x00000004;
    int DB_REP_REREQUEST = 0x00000008;
    int DB_REVSPLITOFF = 0x00000100;
    int DB_RMW = 0x00002000;
    int DB_SALVAGE = 0x00000040;
    int DB_SEQ_DEC = 0x00000001;
    int DB_SEQ_INC = 0x00000002;
    int DB_SEQ_WRAP = 0x00000008;
    int DB_SET = 26;
    int DB_SET_LOCK_TIMEOUT = 0x00000001;
    int DB_SET_RANGE = 27;
    int DB_SET_RECNO = 28;
    int DB_SET_TXN_TIMEOUT = 0x00000002;
    int DB_SNAPSHOT = 0x00000200;
    int DB_STAT_CLEAR = 0x00000001;
    int DB_STREAM_READ = 0x00000001;
    int DB_STREAM_SYNC_WRITE = 0x00000004;
    int DB_STREAM_WRITE = 0x00000002;
    int DB_SYSTEM_MEM = 0x00080000;
    int DB_THREAD = 0x00000020;
    int DB_TIMEOUT = -30971;
    int DB_TIME_NOTGRANTED = 0x00040000;
    int DB_TRUNCATE = 0x00040000;
    int DB_TXN_ABORT = 0;
    int DB_TXN_APPLY = 1;
    int DB_TXN_BACKWARD_ROLL = 3;
    int DB_TXN_BULK = 0x00000010;
    int DB_TXN_FORWARD_ROLL = 4;
    int DB_TXN_NOSYNC = 0x00000001;
    int DB_TXN_NOT_DURABLE = 0x00000004;
    int DB_TXN_NOWAIT = 0x00000002;
    int DB_TXN_PRINT = 7;
    int DB_TXN_SNAPSHOT = 0x00000004;
    int DB_TXN_SYNC = 0x00000008;
    int DB_TXN_TOKEN_SIZE = 20;
    int DB_TXN_WAIT = 0x00000080;
    int DB_TXN_WRITE_NOSYNC = 0x00000020;
    int DB_UNKNOWN = 5;
    int DB_UPGRADE = 0x00000001;
    int DB_USE_ENVIRON = 0x00000004;
    int DB_USE_ENVIRON_ROOT = 0x00000008;
    int DB_VERB_BACKUP = 0x00000001;
    int DB_VERB_DEADLOCK = 0x00000002;
    int DB_VERB_FILEOPS = 0x00000004;
    int DB_VERB_FILEOPS_ALL = 0x00000008;
    int DB_VERB_RECOVERY = 0x00000020;
    int DB_VERB_REGISTER = 0x00000040;
    int DB_VERB_REPLICATION = 0x00000080;
    int DB_VERB_REPMGR_CONNFAIL = 0x00000100;
    int DB_VERB_REPMGR_MISC = 0x00000200;
    int DB_VERB_REP_ELECT = 0x00000400;
    int DB_VERB_REP_LEASE = 0x00000800;
    int DB_VERB_REP_MISC = 0x00001000;
    int DB_VERB_REP_MSGS = 0x00002000;
    int DB_VERB_REP_SYNC = 0x00004000;
    int DB_VERB_REP_SYSTEM = 0x00008000;
    int DB_VERB_REP_TEST = 0x00010000;
    int DB_VERB_WAITSFOR = 0x00020000;
    int DB_VERIFY = 0x00000002;
    int DB_VERSION_MAJOR = 6;
    int DB_VERSION_MINOR = 0;
    int DB_VERSION_PATCH = 19;
    int DB_WRITECURSOR = 0x00000010;
    int DB_YIELDCPU = 0x00080000;
}

// end of DbConstants.java
