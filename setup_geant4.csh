#!/bin/sh

#--------------------------------------------------------------------------------------------

#
# Setup script for GEANT4 + ROOT6 with /cvmfs/
#
# 1) setup your geant4 version
# 2) use Nikhef setup for ROOT - NOTE: should be improved for other sites to use it as well
#
# A.P.Colijn 

#--------------------------------------------------------------------------------------------
# setup GEANT4
setenv BASEDIR `pwd`
source /cvmfs/geant4.cern.ch/etc/login.csh
cd /cvmfs/geant4.cern.ch/geant4/10.1.p02/x86_64-slc6-gcc48-opt/bin/
source geant4.csh
cd ${BASEDIR}

#--------------------------------------------------------------------------------------------
# setup root
source /project/atlas/nikhef/cvmfs/setup.csh
setupATLAS
# localSetupROOT 5.34.25-x86_64-slc6-gcc48-opt
localSetupROOT 

#--------------------------------------------------------------------------------------------

# setup gcc to make sure you still use the right version of gcc and g++
setenv CXX `which g++`
setenv CC  `which gcc`
