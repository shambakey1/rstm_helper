/* 
 * File:   rstm_hlp.hpp
 * Author: root
 *
 * Created on February 21, 2012, 4:42 PM
 */

#ifndef RSTM_HLP_HPP
#define	RSTM_HLP_HPP

#include <vector>
#include <pthread.h>
#include <string>
#include <pthread.h>
#include <chronos/chronos.h>

#define RUN_PRIO 40
#define END_PRIO 45
#define MAIN_PRIO 99
/*********************** PNF START ******************************/
#define PNF_M_PRIO 46   //Used with PNF CM for executing and initially checking transactions
#define PNF_N_PRIO 39   //Used with PNF CM for retrying transactions
#define CM_MAIN_SERVICE 47	//Priority of centralized CM main service (i.e., pnf_main)
/*********************** PNF END ******************************/
#define MILLION 1000000
#define BILLION 1000000000
#define SH_OBJ_PER 1


using namespace std;

/*
 * Declare global data
 */
struct task_in_param{
    struct timespec* time_param;
    int gen_eta;                                //Maximum number of times each transaction can be aborted
                                                //Used in FBLT
    int task_run_prio;
    int task_end_prio;
    int task_util;
    struct timespec* task_deadline;
    struct timespec* task_period;
    unsigned long task_unlocked;
    unsigned long task_locked;
};

extern string sync_tech[];	//different synchronization techniques
extern vector<double> m_set_objs;       //Holds accessed objects by executing transactions
extern vector<void*> n_set;             //Holds non executing transactions
extern pthread_mutex_t m_set_mutx;      //Mutex to check m_set for conflicting objects. Removal from m_set does not need mutex
extern pthread_mutexattr_t m_set_mutx_attr;//Attributes for m_set_mutex
extern bool mu;    //If m_set_mutx initialized, then it is true
extern bool STM_CHECKPOINT;	//If true, then checkpointing is enabled.
extern bool TRANSITIVE;	//If true, then objects are generated in a pattern that introduces transitive retry
extern bool CALIBRATION;	//If true, then experiment runs in calibration mode. In this mode, each task runs individually without interference from other tasks. This is used to determine overhead of different synchronization techniques
extern string sync_alg;	//synchronization technique. If using STM, just name the "cm". If using locking
					//, then name the locking protocol (e.g., "OMLP" or "RNLP"). If using "lock_free",
					//, then say "lock_free"
extern double sh_lev;			//Default is all objects are available for sharing (i.e., sh_lev=1)
extern unsigned long cm_stop;	//Used with centralized CMs like PNF. If 0, then the main service of CM stops
/*
 * Declare global methods
 */
extern void mu_init();         //Initialize mutex and set mu_init to true
extern void mu_lock();         //lock mu_init
extern void mu_unlock();       //unlock mu_init
extern void mu_destroy();		//Destroy mutex
extern string upperStr(string s);	//Change string s to uppercase
extern bool check_sync(string s);	//Checks whether input synchronization technique already exists
extern void setCheckpoint(bool set_cp);	//If set_cp=true, then we use checkpointing
extern void setTransitiveRetry(bool set_tr);	//If true, then objects are generated to introduce transitive retry patter
extern void setCalibration(bool set_cal);	//If true, then experiment runs in calibration mode. In this mode, each task runs individually without interference from other tasks. This is used to determine overhead of different synchronization techniques
extern bool isSTM(string s);	//Return true if synchronization technique uses STM

#endif	/* RSTM_HLP_HPP */

