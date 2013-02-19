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
#include <chronos/chronos.h>

#define RUN_PRIO 40
#define END_PRIO 45
#define MAIN_PRIO 99
/*********************** PNF START ******************************/
#define PNF_M_PRIO 46   //Used with PNF CM for executing and initially checking transactions
#define PNF_N_PRIO 39   //Used with PNF CM for retrying transactions
/*********************** PNF END ******************************/
#define MILLION 1000000
#define BILLION 1000000000
//#define STM_CHECKPOINT
#define SH_OBJ_PER 1


using namespace std;

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
//extern pthread_mutex_t m_set_mutx;      //Mutex to check m_set for conflicting objects. Removal from m_set does not need mutex
extern chronos_mutex_t m_set_mutx;      //Mutex to check m_set for conflicting objects. Removal from m_set does not need mutex
extern bool mu;    //If m_set_mutx initialized, then it is true

extern void mu_init();         //Initialize mutex and set mu_init to true
extern void mu_lock();         //lock mu_init
extern void mu_unlock();       //unlock mu_init
extern string upperStr(string s);	//Change string s to uppercase
extern bool check_sync(string s);	//Checks whether input synchronization technique already exists

#endif	/* RSTM_HLP_HPP */
