# cossim
GEANT4 simulation code 

Simple Geant4 simulation of a four layer silicon telescope + iron tube

To setup the code after downloading from github:

1. make directory "cossim-x" in same directory where you cloned cossim. 

2. cd cossim-x

3. cmake -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/10.1.p02/x86_64-slc6-gcc48-opt/bin ../cossim

4. make

To run in batch mode:

1. cd cossim/macros

2. ../../cossim-x/G4simu -p G4simu_preinit.mac -f G4simu_run.mac -n <number_of_events>

To run interactively:

1. cd cossim/macros

2. ../../cossim-x/G4simu -p G4simu_preinit.mac -f G4simu_run.mac -i

3. Now you get to the GEANT4 command line:

4. /control/exectute G4simu_vis.mac

5. /run/beamOn <number_of_events>
