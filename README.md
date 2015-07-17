# cossim
GEANT4 simulation code 

Simple Geant4 simulation of a four layer silicon telescope + iron tube

To setup the code after downloading from github (on stoomboot @ Nikhef):

1. make directory "cossim-x" in same directory where you cloned cossim. 

2. cd cossim-x

3. source ../cossim/setup_geant4.csh 

4. cmake -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/10.1.p02/x86_64-slc6-gcc48-opt/bin ../cossim

5. make

To run in batch mode:

1. cd cossim/macros

2. source ../setup_geant4.csh 

3. ../../cossim-x/G4simu -p G4simu_preinit.mac -f G4simu_run.mac -n <number_of_events>

To run interactively:

1. cd cossim/macros

2. source ../setup_geant4.csh

3. ../../cossim-x/G4simu -p G4simu_preinit.mac -f G4simu_run.mac -i

4. Now you get to the GEANT4 command line:

5. /control/exectute G4simu_vis.mac

6. /run/beamOn <number_of_events>
