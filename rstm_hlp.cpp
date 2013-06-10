#include <rstm_hlp.hpp>

/*
 * Define global data
 */
vector<double> m_set_objs;       //Holds accessed objects by executing transactions
vector<void*> n_set;             //Holds non executing transactions
pthread_mutex_t m_set_mutx;
pthread_mutex_t new_tx_released_mutex;	//Held by a new released Tx that requires notifying centralized CM like PNF
pthread_mutex_t new_tx_committed_mutex;	//Held by a committed Tx that requires notifying centralized CM like PNF
pthread_mutexattr_t m_set_mutx_attr;
bool mu=false;
string sync_tech[]={"ECM","RCM","LCM","PNF","FBLT","OMLP","RNLP","LOCK_FREE"};
bool STM_CHECKPOINT=false;	//Default is no checkpointing
bool TRANSITIVE=false;		//Default is no transitive retry
bool CALIBRATION=false;		//Default is non-calibration mode
double sh_lev=1;			//Default is all objects are available for sharing
string sync_alg="";
volatile unsigned long cm_stop=0;	//Used with centralized CMs like PNF. If 0, then the main service of CM stops
/*
 * Define global methods
 */
void mu_init(){
	pthread_mutexattr_init(&m_set_mutx_attr);
	pthread_mutexattr_setprotocol(&m_set_mutx_attr,PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&m_set_mutx,&m_set_mutx_attr);
    /*
     * Current implemenation uses the same pthread_mutex_attr for m_set_mutex, new_tx_released_mutex and
     * new_tx_committed_mutex
     */
    pthread_mutex_init(&new_tx_released_mutex,&m_set_mutx_attr);
    pthread_mutex_init(&new_tx_committed_mutex,&m_set_mutx_attr);
    mu=true;
}

void mu_lock(){
	pthread_mutex_lock(&m_set_mutx);
}

void mu_unlock(){
	pthread_mutex_unlock(&m_set_mutx);
}

void new_tx_released_lock(){
	//lock new_tx_released_mutex
	pthread_mutex_lock(&new_tx_released_mutex);
}

void new_tx_released_unlock(){
	//unlock new_tx_released_mutex
	pthread_mutex_unlock(&new_tx_released_mutex);
}

void new_tx_committed_lock(){
	//lock new_tx_committed_mutex
	pthread_mutex_lock(&new_tx_committed_mutex);
}

void new_tx_committed_unlock(){
	//unlock new_tx_committed_mutex
	pthread_mutex_unlock(&new_tx_committed_mutex);
}

void mu_destroy(){
	pthread_mutex_destroy(&m_set_mutx);
	pthread_mutex_destroy(&new_tx_released_mutex);
	pthread_mutex_destroy(&new_tx_committed_mutex);
	pthread_mutexattr_destroy(&m_set_mutx_attr);
	mu=false;
}

string upperStr(string s){
	for(int i=0;i<s.length();i++){
		s[i]=toupper(s[i]);
	}
	return s;
}

bool check_sync(string s){
	for(int i=0;i<sizeof(sync_tech)/sizeof(string);i++){
		s=upperStr(s);
		if(!s.compare(upperStr(sync_tech[i]))){
			return true;
		}
	}
	return false;
}

void setCheckpoint(bool set_cp){
	STM_CHECKPOINT=set_cp;
}

void setTransitiveRetry(bool set_str){
	/*
	 * If true, then objects are generated to introduce transitive retry patter
	 */
	TRANSITIVE=set_str;
}

void setCalibration(bool set_cal){
	/*
	 * If true, then experiment runs in calibration mode. In this mode, each task runs individually
	 * without interference from other tasks. This is used to determine overhead of different
	 * synchronization techniques
	 */
	CALIBRATION=set_cal;
}

bool isSTM(string s){
	s=upperStr(s);
	if(!s.compare("ECM") || !s.compare("RCM") || !s.compare("LCM") || !s.compare("PNF") ||
			!s.compare("FBLT")){
		// In case of stm synchronization
		return true;
	}
	return false;
}


