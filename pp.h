#include <ross.h>

#define TW_MAX_NAME_LEN 31

/* 2D grid */
#define NUM_CELLS_X 32     //256
#define NUM_CELLS_Y 32     //256

/* overlapping grid that divides 2D grid above into sections 
each processor gets a section
ex. splitting x and y in half creates four quadrants, 
one quadrant for each processor */

#define NUM_VP_X 4 
#define NUM_VP_Y 4

typedef int     Channel_t;
typedef int     Min_t;
typedef int     MethodName_t;

/* METHOD CASE [NAME] [NUMBER]*/
#define UPDATE_METHOD 0
#define PREDATOR_MSG 1
#define PREY_MSG 2

#define CELL_TICK 1.0

/********* User Entity Classes ******************************************/

/* define state of cell*/
struct State
{
	int     CellLocationX;
	int     CellLocationY;
	int 	Predator;
	int 	Prey;
	int 	Grass;
	//counter variables for number of predator/prey in/out of cell
	int 	Predator_in;
	int 	Predator_out;
	int 	Prey_in;
	int 	Prey_out;
};

/* reverse computing */ 
struct RC
{
	int 	Predator;
	int 	Prey;
	int 	Grass;
	int 	Predator_in;
	int 	Predator_out;
	int 	Prey_in;
	int 	Prey_out;
};

/* ex. when prey or predator leaves space - send message
arrival prey / arrival predator and associated health factor */
struct Msg_Data
{
	MethodName_t    MethodName;
};

/* Cell Statistics */ 
struct CellStatistics
{
        int  	Grass;
        int 	Predator;
        int 	Prey;
        int 	Predator_in;
        int 	Predator_out;
  	int 	Prey_in;
  	int 	Prey_out;
  	double 	Avg_Pred_amount;
  	double 	Avg_Prey_amount;
  	double 	Avg_Pred_in;
  	double 	Avg_Pred_out;
  	double 	Avg_Prey_in;
  	double 	Avg_Prey_out;
};

/* Function Declaration */
tw_lpid         Cell_ComputeMove( tw_lpid lpid, int direction );
tw_peid         CellMapping_lp_to_pe(tw_lpid lpid);
tw_lp          *CellMapping_to_lp(tw_lpid lpid);
tw_lpid         CellMapping_to_local_index(tw_lpid lpid);
  
void            Cell_Init(struct State *SV, tw_lp * lp);
void            Cell_EventHandler(struct State *SV, tw_bf * CV, struct Msg_Data *M, tw_lp * lp);
void         RC_Cell_EventHandler(struct State *SV, tw_bf * CV, struct Msg_Data *M, tw_lp * lp);

void            CellStatistics_CollectStats(struct State *, tw_lp *lp);
void            CellStatistics_Compute();
void            CellStatistics_Print();

/* Declare CellStats - make it global to the model */
struct CellStatistics TWAppStats;
tw_lpid nlp_per_pe = 0; //global variable - determines how many LPs are there per MPI rank
