/*
 *  ----------------------------------------------------------------------------
 *  This file was automatically generated by SWIG (http://www.swig.org).
 *  Version: 1.3.19
 *
 *  Do not make changes to this file unless you know what you are doing--modify
 *  the SWIG interface file instead.
 *  -----------------------------------------------------------------------------
 */
package com.sleepycat.db;

/**
 *  The locking interfaces for the Berkeley DB database environment are methods
 *  of the {@link com.sleepycat.db.DbEnv DbEnv} handle. The DbLock object is the
 *  handle for a single lock, and has no methods of its own.</p>
 *
 */
public class DbLock {
    private long swigCPtr;
    /**
     */
    protected boolean swigCMemOwn;


    /**
     *  Constructor for the DbLock object
     *
     */
    protected DbLock(long cPtr, boolean cMemoryOwn) {
        swigCMemOwn = cMemoryOwn;
        swigCPtr = cPtr;
    }


    /**
     *  Constructor for the DbLock object
     */
    protected DbLock() {
        this(0, false);
    }


    /**
     */
    protected void finalize() {
        try {
            delete();
        } catch (Exception e) {
            System.err.println("Exception during finalization: " + e);
            e.printStackTrace(System.err);
        }
    }


    /**
     */
    void delete() {
        if (swigCPtr != 0 && swigCMemOwn) {
            db_javaJNI.delete_DbLock(swigCPtr);
            swigCMemOwn = false;
        }
        swigCPtr = 0;
    }


    /**
     *  Gets the cPtr attribute of the DbLock class
     *
     *@return      The cPtr value
     */
    protected static long getCPtr(DbLock obj) {
        return (obj == null) ? 0 : obj.swigCPtr;
    }

}
