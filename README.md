# core Flight System (cFS) File Manager Application (FM) 

## Introduction

The File Manager application (FM) is a core Flight System (cFS) application 
that is a plug-in to the Core Flight Executive (cFE) component of the cFS.  
  
The FM application provides onboard file system management services by 
processing ground commands for copying, moving, and renaming files, 
decompressing files, creating directories, deleting files and directories, 
providing file and directory informational telemetry messages, and providing 
open file and directory listings.

The FM application is written in C and depends on the cFS Operating System 
Abstraction Layer (OSAL) and cFE components. There is additional FM application 
specific configuration information contained in the application user's guide.
 
User's guide information can be generated using Doxygen (from top mission directory):
```
  make prep
  make -C build/docs/fm-usersguide fm-usersguide
```

## Software Required

cFS Framework (cFE, OSAL, PSP)

An integrated bundle including the cFE, OSAL, and PSP can
be obtained at https://github.com/nasa/cfs

## About cFS

The cFS is a platform and project independent reusable software framework and
set of reusable applications developed by NASA Goddard Space Flight Center.
This framework is used as the basis for the flight software for satellite data
systems and instruments, but can be used on other embedded systems.  More
information on the cFS can be found at http://cfs.gsfc.nasa.gov
