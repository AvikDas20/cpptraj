// TrajinList
#include "TrajinList.h"
#include "CpptrajStdio.h"
#include "Trajin_Single.h"
#include "Trajin_Multi.h"

TrajinList::TrajinList() : debug_(0) { }

TrajinList::~TrajinList() {
  for (ListType::iterator traj = trajin_.begin(); traj != trajin_.end(); ++traj)
    delete *traj;
}

// TrajinList::AddTrajin()
/** Add trajectory to the trajectory list as an input trajectory. 
  * Associate the trajectory with one of the parm files in the 
  * TopologyList. 
  * trajin <filename> [start] [stop] [offset] [parm <parmfile> | parmindex <#>]
  *        [remdtraj remdtrajtemp <T>]
  */
int TrajinList::AddTrajin(ArgList& argIn, TopologyList& topListIn) {
  Trajin* traj = 0;
  if ( argIn.CommandIs("trajin") ) {
    if (argIn.hasKey("remdtraj"))
      traj = new Trajin_Multi();
    else
      traj = new Trajin_Single();
  } else if ( argIn.CommandIs("ensemble") ) {
    traj = new Trajin_Multi();
  } else
    return 1; // Command does not pertain to TrajinList
  if (traj==0) {
    mprinterr("Error: TrajinList::Add: Could not allocate memory for traj.\n");
    return 1;
  }
  // Get parm from TopologyList based on args
  Topology* tempParm = topListIn.GetParm( argIn );
  traj->SetDebug(debug_);
  if ( traj->SetupTrajRead(argIn.GetStringNext(), &argIn, tempParm) ) {
    mprinterr("Error: trajin: Could not set up trajectory.\n");
    delete traj;
    return 1;
  }

  // Add to trajectory file list
  trajin_.push_back(traj);

  return 0;
}

// TrajinList::SetupFrames()
/** Only called for input trajectories.
  * Loop over all trajectories and call their setup frames routine to calc
  * actual start and stop and how many frames total will be processed. Update 
  * the number of frames that will be read for the associated traj parm.
  * Return the total number of frames to be processed across all trajins.
  */
int TrajinList::SetupFrames() {
  int maxFrames = 0;

  for (ListType::iterator traj = trajin_.begin(); traj != trajin_.end(); ++traj) 
  {
    int trajFrames = (*traj)->TotalReadFrames();
    // If < 0 frames this indicates the number of frames could not be determined. 
    if (trajFrames < 0) {
      maxFrames = -1;
    } else if (maxFrames != -1) {
      // Only update # of frames if total # of frames so far is known.
      (*traj)->TrajParm()->IncreaseFrames( trajFrames );
      maxFrames += trajFrames;
    }
    // Print input traj information
    (*traj)->PrintInfo( 1 );
  }

  return maxFrames;
}

