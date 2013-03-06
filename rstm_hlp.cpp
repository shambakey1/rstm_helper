#include <rstm_hlp.hpp>

/*
 * Define global data
 */
vector<double> m_set_objs;       //Holds accessed objects by executing transactions
vector<void*> n_set;             //Holds non executing transactions
pthread_mutex_t m_set_mutx;
bool mu=false;
string sync_tech[]={"ECM","RCM","LCM","PNF","FBLT","OMLP","RNLP","LOCK_FREE"};
bool STM_CHECKPOINT=false;
string sync_alg="";

/*
 * Define global methods
 */
void mu_init(){
    if(!mu){
    	pthread_mutex_init(&m_set_mutx,NULL);
        mu=true;
    }
}

void mu_lock(){
	pthread_mutex_lock(&m_set_mutx);
}

void mu_unlock(){
	pthread_mutex_unlock(&m_set_mutx);
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

bool isSTM(string s){
	s=upperStr(s);
	if(!s.compare("ECM") || !s.compare("RCM") || !s.compare("LCM") || !s.compare("PNF") ||
			!s.compare("FBLT")){
		// In case of stm synchronization
		return true;
	}
	return false;
}
