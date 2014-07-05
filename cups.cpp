#include <cstdio>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
typedef long long ll;
using namespace std;

class CupState
{
public:
    CupState(const ll & vol_1 = 0 , const ll & vol_2 = 0);
    static void initializeEvent(const ll & vol_1 , const ll & vol_2 , const ll & target_vol);
    ll getCupVolume(const short & cup_id);
    ll getCupMaxVolume(const short & cup_id);
    bool isEmpty(const short & cup_id);
    CupState pour(const short & source_id , const short & target_id);
    CupState fill(const short & cup_id);
    CupState empty(const short & cup_id);
    short isTargetVolume();
    void setParentPtr(CupState * new_ptr_parent);
    CupState * getParentPtr();
    string hash();
    static ll target_volume;
    static ll cup_1_max_volume;
    static ll cup_2_max_volume;
private:
    ll cup_1_volume;
    ll cup_2_volume;
    CupState * ptr_parent;
};

ll CupState::target_volume;
ll CupState::cup_1_max_volume;
ll CupState::cup_2_max_volume;

CupState::CupState(const ll & vol_1 , const ll & vol_2 ){
    cup_1_volume = vol_1;
    cup_2_volume = vol_2;
    ptr_parent = NULL;
}

void CupState::initializeEvent(const ll & vol_1 , const ll & vol_2 , const ll & target_vol){

    CupState::cup_1_max_volume = vol_1;
    CupState::cup_2_max_volume = vol_2;
    CupState::target_volume = target_vol;
}

ll CupState::getCupVolume(const short & cup_id){
    if(cup_id == 1) return cup_1_volume;
    else if (cup_id == 2) return cup_2_volume;
    else return 0;
}

ll CupState::getCupMaxVolume(const short & cup_id){
    if(cup_id == 1) return CupState::cup_1_max_volume;
    else if (cup_id == 2) return CupState::cup_2_max_volume;
    else throw "invalid cup id!";
}

bool CupState::isEmpty(const short & cup_id){
    if(cup_id == 1) return cup_1_volume == 0;
    else if(cup_id == 2) return cup_2_volume == 0;
    else throw "invalid cup id!";
}

CupState CupState::pour(const short & source_id , const short & target_id){
    if(source_id == target_id) throw -1;

    if(source_id == 1 && target_id == 2){
        ll new_cup_2_volume = cup_2_volume;
        ll new_cup_1_volume = cup_1_volume;
        new_cup_2_volume += new_cup_1_volume;
        new_cup_2_volume = min(new_cup_2_volume , CupState::cup_2_max_volume);
        new_cup_1_volume -= (new_cup_2_volume - cup_2_volume);

        return CupState(new_cup_1_volume , new_cup_2_volume);
    }
    else if(source_id == 2 && target_id == 1){
        ll new_cup_1_volume = cup_1_volume;
        ll new_cup_2_volume = cup_2_volume;
        new_cup_1_volume += new_cup_2_volume;
        
        new_cup_1_volume = min(new_cup_1_volume , CupState::cup_1_max_volume);
        new_cup_2_volume -= (new_cup_1_volume - cup_1_volume);

        return CupState(new_cup_1_volume , new_cup_2_volume);
    }
    else
        throw "invalid cup id!";
}

CupState CupState::fill(const short & cup_id){
    if(cup_id == 1) return CupState(CupState::cup_1_max_volume , cup_2_volume);
    else if (cup_id == 2) return CupState(cup_1_volume , CupState::cup_2_max_volume);
    else throw "invalid cup id!";
}

CupState CupState::empty(const short & cup_id){
    if(cup_id == 1) return CupState(0 , cup_2_volume);
    else if (cup_id == 2) return CupState(cup_1_volume , 0);
    else throw "invalid cup id!";
}

short CupState::isTargetVolume(){
    if(cup_1_volume == CupState::target_volume) return 1;
    else if(cup_2_volume == CupState::target_volume) return 2;
    else return 0;
}

string CupState::hash(){
    stringstream ss;
    ss << cup_1_volume << "," << cup_2_volume;
    return ss.str();
}

void CupState::setParentPtr(CupState * new_ptr_parent){
    ptr_parent = new_ptr_parent;
}

inline CupState * CupState::getParentPtr(){
    return ptr_parent;
}

int main(int argc , char * argv[])
{
	
    try{
        long long vol_1_tmp = argc >= 1 ? strtol(argv[1] , NULL, 10) : 90;
        long long vol_2_tmp  = argc >= 2 ? strtol(argv[2] , NULL, 10) : 40;
        long long target_vol_tmp = argc >= 3 ? strtol(argv[3] , NULL, 10) : 20;
        CupState::initializeEvent(vol_1_tmp , vol_2_tmp , target_vol_tmp);

        printf("Your target volume is %lld , max volume 1 is %lld whereas max volume 2 is %lld\n", CupState::target_volume , CupState::cup_1_max_volume , CupState::cup_2_max_volume );

        map<string , bool> visited_states;
        queue<CupState> bfs;
        bfs.push(CupState(0,0));
        bool found_volume = false;
        CupState * ptr_current_state;

        while(!bfs.empty()){
            
            ptr_current_state = new CupState;
            *ptr_current_state = bfs.front();
            bfs.pop();

            if(visited_states[ptr_current_state->hash()])
                continue;
            
            visited_states[ptr_current_state->hash()] = true;

            if(ptr_current_state->isTargetVolume()){
                found_volume = true;
                break;
            }

            CupState next;
            for(int i = 0 ; i < 6 ; i++){
                switch(i) {
                    case 0 : next = ptr_current_state->pour(1,2); break;
                    case 1 : next = ptr_current_state->pour(2,1); break;
                    case 2 : next = ptr_current_state->empty(1); break;
                    case 3 : next = ptr_current_state->empty(2); break;
                    case 4 : next = ptr_current_state->fill(1); break;
                    case 5 : next = ptr_current_state->fill(2); break;
                }
                if(!visited_states[next.hash()]){
                    next.setParentPtr(ptr_current_state);
                    bfs.push(next);
                }
            }
        }
        if(found_volume) {
            do{
                printf("Volume is %lld and %lld\n" , ptr_current_state->getCupVolume(1) , ptr_current_state->getCupVolume(2));
                ptr_current_state = ptr_current_state->getParentPtr();
            }while(ptr_current_state);
            
        }
        else
            puts("No possible solution!\n");
    }
    catch(const char* e){
        printf("%s " , e);
    }

    return 0;
}