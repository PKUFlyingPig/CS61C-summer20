

This directory contains exercises and solutions for a hands-on
OpenMP course.  Information about these programs can be found
in the comments and in the slides from the course.

To use these programs, copy the appropriate "def" file into
"make.def".  For example on a linux system running the gnu 
compilers, I'd type

  cp linux_gnu.def make.def

Then build the programs and test them

   make test

The solutions directory uses the same make.def file so to build
the solutions, just type "make test".  The directory extras
contains additional exercises for more advanced students.  These
have not been as carefully tested and may have problems building
and running on some systems.

We have tested these programs under Linux with the gnu and Intel compilers,
and Windows 7 with the intel compiler. We also tested these
on OS-X with the gnu environment loaded with Apple's xcode.   Apple's
OpenMP environment at this time does not support threadprivate
variables so the pi_mc solutions will not build (and needs to 
be commented out from the makefile).

We have used these programs with the PGI compiler (pgi.def) 
but we have not tested this case recently and it may need some work.

For windows users, to run these on Windows 7 we used the following
procedure.  First go to the start menu, and select 
   
    INtel parallel studio 2011/command prompt/ ia64 visual studio 2010 mode

cd to the appropriate folder.  Copy win_intel.def to make.def and then
use nmake to build

