#include<bits/stdc++.h>
using namespace std;
#define MX 1001
#define ELEV_CNT 4

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> urd(0.0, 1.0);
binomial_distribution<int> binomial(5,0.5);
default_random_engine generator;
uniform_int_distribution<> uid(2, 12);

int simulation_termination;
int number_of_floors, elevators, capacity, batch_size;
int door_holding_time, inter_floor_traveling_time, opening_time, closing_time;
int embarking_time, disembarking_time;
double mean_interarrival_time;


double TIME = 0.0;
double delivery[MX],wait[MX],between[MX];
double ret[ELEV_CNT + 1], stop[ELEV_CNT + 1],operate[ELEV_CNT + 1];
int cur_customers = 0;
int first[ELEV_CNT + 1];
int flr_vec[ELEV_CNT + 1][13];
int sel_vec[ELEV_CNT + 1][13];
int occupy[ELEV_CNT + 1];
int dest_flr[MX];


double get_expo(double mean){
    double u = urd(gen);
    double e = -(mean*log(u));
    return e;
}

int get_binomial(){
    int number = binomial(generator);
    return number + 1;
}

int get_floor_selection(){
    return uid(gen);
}

void read_input(){
    ifstream in;
    in.open("input.txt");

    in>>simulation_termination;
    in>>number_of_floors>>elevators>>capacity>>batch_size;
    in>>door_holding_time>>inter_floor_traveling_time>>opening_time>>closing_time;
    in>>embarking_time>>disembarking_time;
    in>>mean_interarrival_time;

    in.close();

    // cout<<simulation_termination<<endl;
    // cout<<number_of_floors<<elevators<<capacity<<batch_size<<endl;
    // cout<<door_holding_time<<inter_floor_traveling_time<<opening_time<<closing_time<<endl;
    // cout<<embarking_time<<disembarking_time<<endl;
    // cout<<mean_interarrival_time<<endl;    
}

int select_elev(){
    for(int i=1;i<=ELEV_CNT;i++){
        if(TIME >= ret[i]){
            return i;
        }
    }
    return -1;
}

void simulate(){
    int limit = 0;


    cur_customers = 1;
    delivery[cur_customers] = door_holding_time;
    between[cur_customers] = get_expo(mean_interarrival_time);
    dest_flr[cur_customers] = get_floor_selection();
    TIME = between[cur_customers];

    for(int i=1;i<=ELEV_CNT;i++){
        ret[i] = TIME;
    }

    //4
    while(TIME <= simulation_termination){
        //5
        int selected_elev = select_elev();

        if(selected_elev != -1){

            //6
            first[selected_elev] = cur_customers;
            occupy[selected_elev] = 0;
            memset(flr_vec[0],0,sizeof(flr_vec[0]));
            memset(sel_vec[0],0,sizeof(sel_vec[0]));

            //7
            step_7:
            sel_vec[selected_elev][dest_flr[cur_customers]] = 1;
            flr_vec[selected_elev][dest_flr[cur_customers]] += 1;
            occupy[selected_elev] += 1;

            //8
            cur_customers += 1;
            between[cur_customers] = get_expo(mean_interarrival_time);
            dest_flr[cur_customers] = get_floor_selection();
            TIME += between[cur_customers];
            delivery[cur_customers] = door_holding_time;

            //9
            for(int i=1;i<=ELEV_CNT;i++){
                if(TIME >= ret[i]){
                    ret[i] = TIME;
                }
            }

            //10
            if(between[cur_customers] <= door_holding_time && occupy[selected_elev] < capacity){
                for(int i=first[selected_elev];i<cur_customers;i++){
                    delivery[i] += between[cur_customers];
                }
                goto step_7;
            }
            else{
                limit = cur_customers-1;
            }

            //11
            


        }
        else{

        }
        
    }

}

int main(){

    read_input();
     

    return 0;
}