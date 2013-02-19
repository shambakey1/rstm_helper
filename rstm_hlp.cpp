#include <rstm_hlp.hpp>

vector<double> m_set_objs;       //Holds accessed objects by executing transactions
vector<void*> n_set;             //Holds non executing transactions
//pthread_mutex_t m_set_mutx = PTHREAD_MUTEX_INITIALIZER; //Mutex to check m_set for conflicting objects. Removal from m_set does not need mutex
chronos_mutex_t m_set_mutx;
bool mu=false;
string sync_tech[]={"ECM","RCM","LCM","PNF","FBLT","OMLP","RNLP","LOCK_FREE"};

void mu_init(){
    if(!mu){
        chronos_mutex_init(&m_set_mutx);
        mu=true;
    }
}

void mu_lock(){
    chronos_mutex_lock(&m_set_mutx);
}

void mu_unlock(){
    chronos_mutex_unlock(&m_set_mutx);
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
	if(set_cp){
#define STM_CHECKPOINT;
	}
	else{
#undef STM_CHECKPOINT;
	}
}
