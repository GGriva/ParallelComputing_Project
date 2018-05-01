#include "pp.h"
#include <math.h>

#define MAGIC_VALUE 1

//global variables
tw_lpid g_vp_per_proc =0; // set in main
tw_lpid g_cells_per_vp_x = NUM_CELLS_X/NUM_VP_X;
tw_lpid g_cells_per_vp_y = NUM_CELLS_Y/NUM_VP_Y;
tw_lpid g_cells_per_vp = (NUM_CELLS_X/NUM_VP_X)*(NUM_CELLS_Y/NUM_VP_Y);

/*Determine the neighbor lp-id in a designated direction*/
tw_lpid Cell_ComputeMove( tw_lpid lpid, int direction )
{
  tw_lpid lpid_x, lpid_y; //initialize x and y coordinates for current lp 
  tw_lpid n_x, n_y; //initialize delta-x and delta-y of neighbor lp
  tw_lpid dest_lpid; //initalize destination lp

  //determine x and y coordinates for current lp
  lpid_y = lpid / NUM_CELLS_X;
  lpid_x = lpid - (lpid_y * NUM_CELLS_X);

  /*switch statement to set delta-x and delta-y depending on direction*/
  switch( direction )
    {
    case 0: // SOUTH
      n_x = ((lpid_x - 1) + NUM_CELLS_X) % NUM_CELLS_X;
      n_y = lpid_y;
      break;

    case 1: // NORTH
      n_x = (lpid_x + 1) % NUM_CELLS_X;
      n_y = lpid_y;
      break;

    case 2: // EAST
      n_x = lpid_x;
      n_y = ((lpid_y - 1) + NUM_CELLS_Y) % NUM_CELLS_Y;
      break;

    case 3: // WEST
      n_x = lpid_x;
      n_y = (lpid_y + 1) % NUM_CELLS_Y;
      break;

    default:
      tw_error( TW_LOC, "Bad direction value \n");
    }
  //calculate destination lp based on current lp and delta-x / delta-y
  dest_lpid = (tw_lpid) (n_x + (n_y * NUM_CELLS_X));
  return( dest_lpid );
}

/*Cell Mapping of Logical Process to Processing Event*/
tw_peid CellMapping_lp_to_pe(tw_lpid lpid)
{
  long lp_x = lpid % NUM_CELLS_X;
  long lp_y = lpid / NUM_CELLS_X;
  long vp_num_x = lp_x/g_cells_per_vp_x;
  long vp_num_y = lp_y/g_cells_per_vp_y;
  long vp_num = vp_num_x + (vp_num_y*NUM_VP_X);
  tw_peid peid = vp_num/g_vp_per_proc;
  return peid;
}


/*This code was adapted from Prof. Carothers code in order to provide a Cell Mapping to LP*/
tw_lp *CellMapping_to_lp(tw_lpid lpid)
{
  tw_lpid lp_x = lpid % NUM_CELLS_X; //lpid -> (lp_x,lp_y)
  tw_lpid lp_y = lpid / NUM_CELLS_X;
  tw_lpid vp_index_x = lp_x % g_cells_per_vp_x;
  tw_lpid vp_index_y = lp_y % g_cells_per_vp_y;
  tw_lpid vp_index = vp_index_x + (vp_index_y * (g_cells_per_vp_x));
  tw_lpid vp_num_x = lp_x/g_cells_per_vp_x;
  tw_lpid vp_num_y = lp_y/g_cells_per_vp_y;
  tw_lpid vp_num = vp_num_x + (vp_num_y*NUM_VP_X);
  vp_num = vp_num % g_vp_per_proc;
  tw_lpid index = vp_index + vp_num*g_cells_per_vp;

#ifdef ROSS_runtime_check
  if( index >= g_tw_nlp )
    tw_error(TW_LOC, "index (%llu) beyond g_tw_nlp (%llu) range \n", index, g_tw_nlp);
#endif /* ROSS_runtime_check */

  return g_tw_lp[index];
}

/*This code was adapted from Prof. Carothers code in order to provide a Cell Mapping to Local Index*/
tw_lpid CellMapping_to_local_index(tw_lpid lpid)
{
  tw_lpid lp_x = lpid % NUM_CELLS_X; //lpid -> (lp_x,lp_y)
  tw_lpid lp_y = lpid / NUM_CELLS_X;
  tw_lpid vp_index_x = lp_x % g_cells_per_vp_x;
  tw_lpid vp_index_y = lp_y % g_cells_per_vp_y;
  tw_lpid vp_index = vp_index_x + (vp_index_y * (g_cells_per_vp_x));
  tw_lpid vp_num_x = lp_x/g_cells_per_vp_x;
  tw_lpid vp_num_y = lp_y/g_cells_per_vp_y;
  tw_lpid vp_num = vp_num_x + (vp_num_y*NUM_VP_X);
  vp_num = vp_num % g_vp_per_proc;
  tw_lpid index = vp_index + vp_num*g_cells_per_vp;

  if( index >= g_tw_nlp )
    tw_error(TW_LOC, "index (%llu) beyond g_tw_nlp (%llu) range \n", index, g_tw_nlp);

  return( index );
}

/*Cell Initalization. Initializes the state */
void Cell_Init(struct State *SV, tw_lp * lp)
{ 
  tw_stime  ts; //initialze delta-t
  
  struct Msg_Data TMsg;
  struct Msg_Data * TWMsg;
  tw_event *CurEvent;

  //Initialzie all state variables
  SV->CellLocationX = lp->gid % NUM_CELLS_X;
  SV->CellLocationY = lp->gid / NUM_CELLS_X;
  SV->Predator = tw_rand_integer(lp->rng, 4, 6);
  SV->Prey = tw_rand_integer(lp->rng, 4, 6);
  SV->Grass = tw_rand_integer(lp->rng, 4, 6);
  SV->Predator_in = 0;
  SV->Predator_out = 0;
  SV->Prey_in = 0;
  SV->Prey_out = 0;

  if (SV->CellLocationX >= NUM_CELLS_X ||
      SV->CellLocationY >= NUM_CELLS_Y)
    {
      tw_error(TW_LOC, "Cell_Init: Bad CellLocations %d %d \n",
	       SV->CellLocationX, SV->CellLocationY);
    }

  //send message for first evet
  ts = tw_rand_exponential(lp->rng, CELL_TICK);
  CurEvent = tw_event_new(lp->gid, ts, lp);
  TWMsg = tw_event_data(CurEvent);
  TWMsg->MethodName = UPDATE_METHOD;
  tw_event_send(CurEvent);
}

/*Event Handler - This function runs for every Cell event in the simulation*/
void Cell_EventHandler(struct State *SV, tw_bf * CV, struct Msg_Data *M, tw_lp * lp)
{
  *(int *)CV = (int)0;
  tw_stime          ts;
  struct Msg_Data * TWMsg;
  tw_event *CurEvent; //current event
  tw_event *UpdateAnimal; //update an animal
  int dest_index = 0; //initialize the destination index
  tw_lpid dest_lpid; //initialize the destination lp

  switch (M->MethodName)
    {
    case UPDATE_METHOD:
      /*Evaluate the cell - each tick*/

      if (SV->Prey <= 0)
      {
          SV->Predator -= MAGIC_VALUE; //starvation
          SV->Prey = 0;
      }
      else if (SV->Predator <= 0)
      {
          SV->Predator = 0; //Remains nothing
      }
      //Prey 2x Predator
      else if (SV->Prey / SV->Predator > 2)
      {
          SV->Prey -= MAGIC_VALUE; //eaten
          SV->Predator += MAGIC_VALUE; //reproduction
      }
      //Predator 2x Prey
      else if (SV->Predator / SV->Prey > 2)
      {
          SV->Predator -= MAGIC_VALUE; //Predator starvation
          SV->Prey -= MAGIC_VALUE; //Prey eaten
      }

      //If more prey than grass
      if(SV->Prey > SV->Grass)
      {
          SV->Prey -= MAGIC_VALUE; //Prey Starvation
          SV->Grass -= MAGIC_VALUE; //grass eaten
      }
      else
      {
          SV->Grass -= MAGIC_VALUE; //Prey eat Grass
          SV->Prey += MAGIC_VALUE; //reproduction
      }

      //Increment grass by Magic value
      SV->Grass += MAGIC_VALUE; 

      //Determine a direction to go (based on rng)
      dest_index = tw_rand_integer(lp->rng, 0, 3);
      dest_lpid = Cell_ComputeMove(lp->gid, dest_index);

      //If num. pred > num prey - send predator to random direction
      if (SV->Predator > SV->Prey) 
      {
        //Send Predator Message
        ts = tw_rand_exponential(lp->rng, CELL_TICK);
        UpdateAnimal = tw_event_new(dest_lpid, ts, lp);
        TWMsg = tw_event_data(UpdateAnimal); 
        TWMsg->MethodName = PREDATOR_MSG;
        tw_event_send(UpdateAnimal);
        //Decrement the predator that left
        SV->Predator --; 
        //Increment the counter variable for num. predators out of cell
        SV->Predator_out++; 
      }
      //Else num. prey > num pred - send pray to random direction
      else
      {
        //Send Prey Message
        ts = tw_rand_exponential(lp->rng, CELL_TICK);
        UpdateAnimal = tw_event_new(dest_lpid, ts, lp);
        TWMsg = tw_event_data(UpdateAnimal);
        TWMsg->MethodName = PREY_MSG;
        tw_event_send(UpdateAnimal);
        //Decrement the prey that left
        SV->Prey--;
        //Increment the counter variable for num. prey out of cell
        SV->Prey_out++;
      }

      //Update Self
      ts = tw_rand_exponential(lp->rng, CELL_TICK);
      CurEvent = tw_event_new(lp->gid, ts, lp);
      TWMsg = tw_event_data(CurEvent);
      TWMsg->MethodName = UPDATE_METHOD;
      tw_event_send(CurEvent);
      break;

    case PREDATOR_MSG:
      //Processes Predator Message
      SV->Predator++; //increment number of predator
      SV->Predator_in++; //increment counter for number of predator in cell
      break;

    case PREY_MSG:
      //Process Prey Message
      SV->Prey++; //increment number of prey
      SV->Prey_in++; //increment counter for number of prey in cell
      break;

    default:
      tw_error(TW_LOC, "APP_ERROR (%d): InValid MethodName(%d)\n",
	       lp->gid, M->MethodName);
    }
}

/*Reverse Computation Event Handler - This is run whenever there is a rollback to reverse the event compuation*/
void RC_Cell_EventHandler(struct State *SV, tw_bf * CV, struct Msg_Data *M, tw_lp * lp)
{
#ifdef LPTRACEON
  long            seeds[4];
#endif

  switch (M->MethodName)
    {
    case UPDATE_METHOD:
      //Reverse life
       if (SV->Prey <= 0)
      {
          SV->Predator += MAGIC_VALUE; //reverse starvation
          SV->Prey = 0;
      }
      else if (SV->Predator <= 0)
      {
          SV->Predator = 0; //Remains nothing
      }
      //Prey 2x Predator
      else if (SV->Prey / SV->Predator > 2)
      {
          SV->Predator -= MAGIC_VALUE; //reverse reproduction
          SV->Prey += MAGIC_VALUE; //reverse eaten
      }
      //Predator 2x Prey
      else if (SV->Predator / SV->Prey > 2)
      {
          SV->Prey += MAGIC_VALUE; //bring prey back
          SV->Predator += MAGIC_VALUE; //reverse death
      }

      //If more prey than grass
      if(SV->Prey > SV->Grass)
      {
          SV->Grass += MAGIC_VALUE; //reverse eat
          SV->Prey += MAGIC_VALUE; //reverse death
      }
      else
      {
          SV->Prey -= MAGIC_VALUE; //reverse reproduction
          SV->Grass += MAGIC_VALUE; //reverse eat
      }

      //Reverse the increment of grass
      SV->Grass -= MAGIC_VALUE; 

      if (SV->Predator > SV->Prey) 
        {
          //Reverse the send predator
          SV->Predator_out--;
          SV->Predator++;
        }
      else 
      {
        //Reverse the send prey
        SV->Prey_out--;
        SV->Prey++;
      }
      //Reverse both random number generator calls
      tw_rand_reverse_unif(lp->rng);
      tw_rand_reverse_unif(lp->rng);
      break;

    case PREDATOR_MSG:
      //reverse receiving predator message
      SV->Predator--;
      SV->Predator_in--;
      break;

    case PREY_MSG:
      //reverse receiving prey message
      SV->Prey--;
      SV->Prey_in--;
      break;
    }
}

/****** Finalization Functions ******/

/*Collect Global Statistics*/
void CellStatistics_CollectStats(struct State *SV, tw_lp * lp)
{
  TWAppStats.Grass += SV->Grass;
  TWAppStats.Predator += SV->Predator;
  TWAppStats.Prey += SV->Prey;
  TWAppStats.Predator_in += SV->Predator_in;
  TWAppStats.Predator_out += SV->Predator_out;
  TWAppStats.Prey_in += SV->Prey_in;
  TWAppStats.Prey_out += SV->Prey_out;
}

/*Calculate Cell Statistics*/
void CellStatistics_Compute(struct CellStatistics *CS)
{
  CS->Avg_Pred_amount = ((double) CS->Predator) / ((double) (NUM_CELLS_X*NUM_CELLS_Y));
  CS->Avg_Prey_amount = ((double) CS->Prey) / ((double) (NUM_CELLS_X*NUM_CELLS_Y));
  CS->Avg_Pred_in = ((double) CS->Predator_in) / ((double) (NUM_CELLS_X*NUM_CELLS_Y));
  CS->Avg_Pred_out = ((double) CS->Predator_out) / ((double) (NUM_CELLS_X*NUM_CELLS_Y));
  CS->Avg_Prey_in = ((double) CS->Prey_in) / ((double) (NUM_CELLS_X*NUM_CELLS_Y));
  CS->Avg_Prey_out = ((double) CS->Prey_out) / ((double) (NUM_CELLS_X*NUM_CELLS_Y));
}

/*Reduce to the correct number of Prey/Predator/Grass in Rank 0*/
void CellStatistics_Reduce(struct CellStatistics *CS)
{

  // Create array for each rank to reduce for the cell satistics
  int reduce_arr[7];

  reduce_arr[0] = CS->Grass;
  reduce_arr[1] = CS->Predator;
  reduce_arr[2] = CS->Prey;
  reduce_arr[3] = CS->Predator_in;
  reduce_arr[4] = CS->Predator_out;
  reduce_arr[5] = CS->Prey_in;
  reduce_arr[6] = CS->Prey_out;

  int g_reduce_arr[7];

  // Puts the total number for each of the statistics in g_reduce_arr for rank 0
  MPI_Reduce(&reduce_arr, &g_reduce_arr, 7,MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);

  // Update the total for rank 0
  CS->Grass = g_reduce_arr[0];
  CS->Predator = g_reduce_arr[1];
  CS->Prey = g_reduce_arr[2];
  CS->Predator_in = g_reduce_arr[3];
  CS->Predator_out = g_reduce_arr[4];
  CS->Prey_in = g_reduce_arr[5];
  CS->Prey_out = g_reduce_arr[6];  

}

/*Print Cell Statistics*/
void CellStatistics_Print(struct CellStatistics *CS)
{
  printf("Total Grass Amount ................................%d\n",
	 CS->Grass);
  printf("Total Predator Amount .............................%d\n",
    CS->Predator);
  printf("Total Prey Amount .................................%d\n",
    CS->Prey);
  printf("Average Predator per cell..........................%f\n",
    CS->Avg_Pred_amount);
  printf("Average Prey per cell .............................%f\n",
    CS->Avg_Prey_amount);
  printf("Average Predator In-cell Movement .................%f\n",
    CS->Avg_Pred_in);
  printf("Average Predator Out-cell Movement ................%f\n",
    CS->Avg_Pred_out);
  printf("Average Prey In-cell Movement .....................%f\n",
    CS->Avg_Prey_in);
  printf("Average Prey Out-cell Movement ....................%f\n",
    CS->Avg_Prey_out);
}

/******** Initialize_Appl *************************************************/

#define	TW_CELL	1

tw_lptype       mylps[] =
  {
    {
      (init_f) Cell_Init,
      (pre_run_f) NULL,
      (event_f) Cell_EventHandler,
      (revent_f) RC_Cell_EventHandler,
      (commit_f) NULL,
      (final_f) CellStatistics_CollectStats,
      (map_f) CellMapping_lp_to_pe,
      sizeof(struct State)
    },
    {0},
  };

/*This code was adapted from Prof. Carothers code in order to provide a pcs to a gird mapping*/
void pcs_grid_mapping()
{
  tw_lpid         x, y;
  tw_lpid         lpid, kpid;
  tw_lpid         num_cells_per_kp, vp_per_proc;
  tw_lpid         local_lp_count;

  num_cells_per_kp = (NUM_CELLS_X * NUM_CELLS_Y) / (NUM_VP_X * NUM_VP_Y);
  vp_per_proc = (NUM_VP_X * NUM_VP_Y) / ((tw_nnodes() * g_tw_npe)) ;
  g_tw_nlp = nlp_per_pe;
  g_tw_nkp = vp_per_proc;

  local_lp_count=0;
  for (y = 0; y < NUM_CELLS_Y; y++)
    {
      for (x = 0; x < NUM_CELLS_X; x++)
	{
	  lpid = (x + (y * NUM_CELLS_X));
	  if( g_tw_mynode == CellMapping_lp_to_pe(lpid) )
	    {

	      kpid = local_lp_count/num_cells_per_kp;
	      local_lp_count++; // MUST COME AFTER!! DO NOT PRE-INCREMENT ELSE KPID is WRONG!!

	      if( kpid >= g_tw_nkp )
		tw_error(TW_LOC, "Attempting to mapping a KPid (%llu) for Global LPid %llu that is beyond g_tw_nkp (%llu)\n",
			 kpid, lpid, g_tw_nkp );

	      tw_lp_onpe(CellMapping_to_local_index(lpid), g_tw_pe[0], lpid);
	      if( g_tw_kp[kpid] == NULL )
		tw_kp_onpe(kpid, g_tw_pe[0]);
	      tw_lp_onkp(g_tw_lp[CellMapping_to_local_index(lpid)], g_tw_kp[kpid]);
	      tw_lp_settype( CellMapping_to_local_index(lpid), &mylps[0]);
	    }
	}
    }
}

// Main to be run for each MPI rank
int main(int argc, char **argv)
{
  tw_lpid         num_cells_per_kp, vp_per_proc;
  unsigned int    additional_memory_buffers;

  // Intialize ROSS env
  tw_init(&argc, &argv);

  nlp_per_pe = (NUM_CELLS_X * NUM_CELLS_Y) / (tw_nnodes() * g_tw_npe);
  additional_memory_buffers = 2 * g_tw_mblock * g_tw_gvt_interval;

  g_tw_events_per_pe = (nlp_per_pe * 16) +  additional_memory_buffers;

  if( tw_ismaster() )
    {
      printf("Running simulation with following configuration: \n" );
      printf("    Buffers Allocated Per PE = %d\n", g_tw_events_per_pe);
      printf("\n\n");
    }

  num_cells_per_kp = (NUM_CELLS_X * NUM_CELLS_Y) / (NUM_VP_X * NUM_VP_Y);
  vp_per_proc = (NUM_VP_X * NUM_VP_Y) / ((tw_nnodes() * g_tw_npe)) ;
  g_vp_per_proc = vp_per_proc;
  g_tw_nlp = nlp_per_pe;
  g_tw_nkp = vp_per_proc;

  g_tw_mapping = CUSTOM;
  g_tw_custom_initial_mapping = &pcs_grid_mapping;
  g_tw_custom_lp_global_to_local_map = &CellMapping_to_lp;

  /*
   * Some some of the settings.
   */
  if( tw_ismaster() )
    {
      printf("\n\n");
      printf("/**********************************************/\n");
      printf("NUM CELLS X      = %d\n", NUM_CELLS_X);
      printf("NUM CELLS Y      = %d\n", NUM_CELLS_Y);
      printf("CELL_TICK        = %lf\n", CELL_TICK);
      printf("NUM KPs per PE   = %lu \n", g_tw_nkp);
      printf("NUM LPs per PE   = %lu \n", g_tw_nlp);
      printf("g_vp_per_proc    = %lu \n", g_vp_per_proc);
      printf("/**********************************************/\n");
      printf("\n\n");
      fflush(stdout);
    }

  tw_define_lps(nlp_per_pe, sizeof(struct Msg_Data));

  /*
   * Initialize App Stats Structure
   */
  TWAppStats.Grass = 0;
  TWAppStats.Predator = 0;
  TWAppStats.Prey = 0;
  TWAppStats.Predator_in = 0;
  TWAppStats.Predator_out = 0;
  TWAppStats.Prey_in = 0;
  TWAppStats.Prey_out = 0;
  TWAppStats.Avg_Pred_amount = 0.0;
  TWAppStats.Avg_Prey_amount = 0.0;
  TWAppStats.Avg_Pred_in = 0.0;
  TWAppStats.Avg_Pred_out = 0.0;
  TWAppStats.Avg_Prey_in = 0.0; 
  TWAppStats.Avg_Prey_out = 0.0;

  // Run the simulation using ROSS
  tw_run();

  //Reduce to the correct number of Prey/Predator/Grass in Rank 0
  CellStatistics_Reduce(&TWAppStats);

  if( tw_ismaster() )
    {
      // Compute overall cell statisitics
      CellStatistics_Compute(&TWAppStats);
      //Print the statisitics    
      CellStatistics_Print(&TWAppStats);
    }



  //End ROSS env/simulation
  tw_end();

  return 0;
}
