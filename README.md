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

A demonstration bundle of the Core Flight System including the cFE, OSAL, and PSP can be obtained at https://github.com/nasa/cfs

For information about a mission ready cFS bundle, see: https://github.com/nasa/cFS#cfs-gov-mission-ready-version

## Known issues

See all [open issues](https://github.com/nasa/FM/issues) and closed to milestones later than this version.

## Getting Help

For best results, submit issues:questions or issues:help wanted requests at <https://github.com/nasa/cFS>.

Official cFS page: <http://cfs.gsfc.nasa.gov>

