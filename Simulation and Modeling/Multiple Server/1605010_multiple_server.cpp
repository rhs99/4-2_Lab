#include <bits/stdc++.h>
using namespace std;
#define MX 4000010
#define ELEV_CNT 4
#define BUILDING_SZ 12

random_device rd;
// mt19937 gen(rd());
// uniform_real_distribution<> urd(0.0, 1.0);
exponential_distribution<double> ed;
binomial_distribution<int> binomial(5, 0.5);
default_random_engine generator_expo, generator_bino, generator_flr;
uniform_int_distribution<> uid(2, 12);

int simulation_termination;
int number_of_floors, elevators, capacity, batch_size;
int door_holding_time, inter_floor_traveling_time, opening_time, closing_time;
int embarking_time, disembarking_time;
double mean_interarrival_time;

double TIME = 0.0;

int dest_flr[MX];
double delivery[MX], wait[MX], between[MX], elevator[MX], arrive[MX];

double ret[ELEV_CNT + 1], stop[ELEV_CNT + 1], operate[ELEV_CNT + 1];
int cur_customers = 0;
int first[ELEV_CNT + 1];
int flr_vec[ELEV_CNT + 1][BUILDING_SZ + 1];
int sel_vec[ELEV_CNT + 1][BUILDING_SZ + 1];
int eldel[ELEV_CNT + 1];
int occupy[ELEV_CNT + 1];

int quecust;
double startque;
int que, quetotal;
double DELTIME = 0, MAXDEL = 0, MAXELEV = 0, QUETIME = 0, ELEVTIME = 0, MAXQUE = 0;
int QUELEN = 0;

double get_expo()
{
    return ed(generator_expo);
}

int get_binomial()
{
    int number = binomial(generator_bino);
    return number + 1;
}

int get_floor_selection()
{
    return uid(generator_flr);
}

void read_input()
{
    ifstream in;
    in.open("input.txt");

    in >> simulation_termination;
    in >> number_of_floors >> elevators >> capacity >> batch_size;
    in >> door_holding_time >> inter_floor_traveling_time >> opening_time >> closing_time;
    in >> embarking_time >> disembarking_time;
    in >> mean_interarrival_time;

    mean_interarrival_time *= 60;
    ed = exponential_distribution<double>(mean_interarrival_time);

    in.close();

    // cout<<simulation_termination<<endl;
    // cout<<number_of_floors<<elevators<<capacity<<batch_size<<endl;
    // cout<<door_holding_time<<inter_floor_traveling_time<<opening_time<<closing_time<<endl;
    // cout<<embarking_time<<disembarking_time<<endl;
    // cout<<mean_interarrival_time<<endl;
}

void fill_between_and_dest_flr(){
    int b_sz,d_flr,cnt = 0;
    double btwn;
    for(int i=1;i<MX;)
    {
        b_sz = get_binomial();

        // if(cnt<5){
        //     cout<<"bt_sz "<<b_sz<<endl;
        //     cnt++;
        // }
        
        d_flr = get_floor_selection();
        btwn = get_expo();
        between[i] = btwn;
        dest_flr[i] = d_flr;
        for(int j=i+1;j<i+b_sz && j<MX;j++){
            between[j] = 0;
            d_flr = get_floor_selection();
            dest_flr[j] = d_flr;
        }
        i=i+b_sz;
    }
}

int select_elev()
{
    for (int i = 1; i <= ELEV_CNT; i++)
    {
        if (TIME >= ret[i])
        {
            return i;
        }
    }
    return -1;
}

void init(){
    TIME = 0.0;

    for(int i=0;i<MX;i++){
        dest_flr[i] = 0;
        delivery[i] = wait[i] = between[i] = elevator[i] = arrive[i] = 0.0;
    }

    for(int i=1;i<=ELEV_CNT;i++){
        ret[i] = stop[i] = operate[i] = first[i] = eldel[i] = occupy[i] = 0; 
    }

    cur_customers = 0;

    for(int i=1;i<=ELEV_CNT;i++)
    {
        for(int j=1;j<=BUILDING_SZ;j++){
            flr_vec[i][j] = sel_vec[i][j] = 0;
        }
    }

    quecust = 0;
    startque = 0;
    que = 0;
    quetotal = 0;
    DELTIME = 0, MAXDEL = 0, MAXELEV = 0, QUETIME = 0, ELEVTIME = 0, MAXQUE = 0;
    QUELEN = 0;
}

void simulate(int run_no, ofstream &out)
{
    int limit = 0, remain = 0, R = 0, N = 0,cnt = 0;
    int sel_sum = 0, Max = 0;
    int flr_sum = 0;
    int prev_disembark = 0;
    int prev_door_open_close = 0;
    int selected_elev = -1;

    cur_customers = 1;
    delivery[cur_customers] = door_holding_time;

    // between[cur_customers] = get_expo();
    // dest_flr[cur_customers] = get_floor_selection();

    // cout<<between[cur_customers]<<" "<<dest_flr[cur_customers]<<endl;

    TIME = between[cur_customers];

    for (int i = 1; i <= ELEV_CNT; i++)
    {
        ret[i] = TIME;
    }

    //4
    while (TIME <= simulation_termination)
    {
    //5
    step_5:
        selected_elev = select_elev();
        // if(selected_elev != -1){
        if (selected_elev == -1)
        {
            goto step_19;
        }
        //6
        first[selected_elev] = cur_customers;
        occupy[selected_elev] = 0;
        memset(flr_vec[selected_elev], 0, sizeof(flr_vec[selected_elev]));
        memset(sel_vec[selected_elev], 0, sizeof(sel_vec[selected_elev]));

    //7
    step_7:
        sel_vec[selected_elev][dest_flr[cur_customers]] = 1;
        flr_vec[selected_elev][dest_flr[cur_customers]] += 1;
        occupy[selected_elev] += 1;

    //8
    step_8:
        cur_customers += 1;

        // between[cur_customers] = get_expo();
        // dest_flr[cur_customers] = get_floor_selection();

        // if(cnt<5){
        //      cout<<between[cur_customers]<<" "<<dest_flr[cur_customers]<<endl;
        //      cnt++;
        // }

        TIME += between[cur_customers];
        delivery[cur_customers] = door_holding_time;

        //9
        for (int i = 1; i <= ELEV_CNT; i++)
        {
            if (TIME >= ret[i])
            {
                ret[i] = TIME;
            }
        }

        //10
        if (between[cur_customers] <= door_holding_time && occupy[selected_elev] < capacity)
        {
            for (int i = first[selected_elev]; i < cur_customers; i++)
            {
                delivery[i] += between[cur_customers];
            }
            goto step_7;
        }
        else
        {
            limit = cur_customers - 1;
            goto step_11;
        }

    //11
    step_11:
        for (int i = first[selected_elev]; i <= limit; i++)
        {

            //12
            N = dest_flr[i] - 1;
            prev_disembark = 0;
            for (int j = 1; j <= N; j++)
            {
                prev_disembark += flr_vec[selected_elev][j];
            }
            prev_door_open_close = 0;
            for (int j = 1; j <= N; j++)
            {
                prev_door_open_close += sel_vec[selected_elev][j];
            }
            elevator[i] = inter_floor_traveling_time * N +
                          disembarking_time * prev_disembark + disembarking_time +
                          (opening_time + closing_time) * prev_door_open_close +
                          opening_time;

            //13
            delivery[i] += elevator[i];

            //14
            DELTIME += delivery[i];

            //15
            if (delivery[i] > MAXDEL)
            {
                MAXDEL = delivery[i];
            }

            //16
            if (elevator[i] > MAXELEV)
            {
                MAXELEV = elevator[i];
            }
        }

        //17
        sel_sum = 0;
        for (int j = 1; j <= number_of_floors; j++)
        {
            sel_sum += sel_vec[selected_elev][j];
            if (sel_vec[selected_elev][j])
            {
                Max = j;
            }
        }
        stop[selected_elev] += sel_sum;

        flr_sum = 0;
        for (int j = 1; j <= number_of_floors; j++)
        {
            flr_sum += flr_vec[selected_elev][j];
        }

        eldel[selected_elev] = (inter_floor_traveling_time * 2) * (Max - 1) +
                               (disembarking_time * flr_sum) + (opening_time + closing_time) * sel_sum;

        ret[selected_elev] = TIME + eldel[selected_elev];
        operate[selected_elev] += eldel[selected_elev];

        //18
        goto step_5;
        // }
        // else{

    //19
    step_19:
        quecust = cur_customers;
        startque = TIME;
        que = 1;
        arrive[cur_customers] = TIME;

    //20
    step_20:
        cur_customers += 1;

        // between[cur_customers] = get_expo();
        // dest_flr[cur_customers] = get_floor_selection();

        // if(cnt < 5){
        //     cout<<between[cur_customers]<<" "<<dest_flr[cur_customers]<<endl;
        //     cnt++;
        // }

        TIME += between[cur_customers];
        arrive[cur_customers] = TIME;
        que += 1;

        //21
        selected_elev = select_elev();
        if (selected_elev != -1)
        {
            goto step_22;
        }
        else
        {
            goto step_20;
        }

    //22
    step_22:
        memset(flr_vec[selected_elev], 0, sizeof(flr_vec[selected_elev]));
        memset(sel_vec[selected_elev], 0, sizeof(sel_vec[selected_elev]));
        remain = que - capacity;

        //23
        if (remain <= 0)
        {
            R = cur_customers;
            occupy[selected_elev] = que;
        }
        else
        {
            R = quecust + capacity - 1;
            occupy[selected_elev] = capacity;
        }

        //24
        for (int i = quecust; i <= R; i++)
        {
            sel_vec[selected_elev][dest_flr[i]] = 1;
            flr_vec[selected_elev][dest_flr[i]] += 1;
        }

        //25
        if (que >= QUELEN)
        {
            QUELEN = que;
        }

        //26
        quetotal += occupy[selected_elev];
        for (int i = quecust; i <= R; i++)
        {
            QUETIME += (TIME - arrive[i]);
        }

        //27
        if (TIME - startque >= MAXQUE)
        {
            MAXQUE = TIME - startque;
        }

        //28
        first[selected_elev] = quecust;

        //29
        for (int i = first[selected_elev]; i <= R; i++)
        {
            delivery[i] = door_holding_time + TIME - arrive[i];
            wait[i] = TIME - arrive[i];
        }

        //30
        if (remain <= 0)
        {
            que = 0;
            goto step_8;
        }
        else
        {
            limit = R;
            for (int i = first[selected_elev]; i <= limit; i++)
            {
                //12
                N = dest_flr[i] - 1;
                prev_disembark = 0;
                for (int j = 1; j <= N; j++)
                {
                    prev_disembark += flr_vec[selected_elev][j];
                }
                prev_door_open_close = 0;
                for (int j = 1; j <= N; j++)
                {
                    prev_door_open_close += sel_vec[selected_elev][j];
                }
                elevator[i] = inter_floor_traveling_time * N +
                              disembarking_time * prev_disembark + disembarking_time +
                              (opening_time + closing_time) * prev_door_open_close +
                              opening_time;

                //13
                delivery[i] += elevator[i];

                //14
                DELTIME += delivery[i];

                //15
                if (delivery[i] > MAXDEL)
                {
                    MAXDEL = delivery[i];
                }

                //16
                if (elevator[i] > MAXELEV)
                {
                    MAXELEV = elevator[i];
                }

                //17
                sel_sum = 0;
                for (int j = 1; j <= number_of_floors; j++)
                {
                    sel_sum += sel_vec[selected_elev][j];
                    if (sel_vec[selected_elev][j])
                    {
                        Max = j;
                    }
                }
                stop[selected_elev] += sel_sum;

                flr_sum = 0;
                for (int j = 1; j <= number_of_floors; j++)
                {
                    flr_sum += flr_vec[selected_elev][j];
                }

                eldel[selected_elev] = (inter_floor_traveling_time * 2) * (Max - 1) +
                                       disembarking_time * flr_sum + (opening_time + closing_time) * sel_sum;

                ret[selected_elev] = TIME + eldel[selected_elev];
                operate[selected_elev] += eldel[selected_elev];
            }

            goto step_31;
        }

    //31
    step_31:
        que = remain;
        quecust = R + 1;
        startque = arrive[R + 1];

        //32
        if (TIME <= simulation_termination)
            goto step_20;

        // }
    }

    //33
    N = cur_customers - que;
    DELTIME = DELTIME / N;

    //34
    for (int i = 1; i <= limit; i++)
    {
        ELEVTIME += elevator[i] / limit;
    }

    //35
    QUETIME /= quetotal;

    //36

    out<<fixed<<setprecision(0);

    out<<run_no<<",";

    out<<N<<","<<DELTIME<<","<<MAXDEL<<","<<ELEVTIME<<","<<MAXELEV<<","<<QUELEN<<","<<QUETIME<<","<<MAXQUE<<",";
    for(int i=1;i<=ELEV_CNT;i++)
    {
        out<<stop[i]<<",";
    }
    for(int i=1;i<=ELEV_CNT;i++)
    {
        out<<operate[i]/simulation_termination<<",";
    }

    out<<endl;
}


void calc_avg(){
    ifstream in;
    in.open("stat.csv");

    double stat[20];
    memset(stat,0,sizeof(stat));

    double x;
    char c;

    for(int i=0;i<10;i++)
    {
        for(int j=0;j<17;j++)
        {
            in>>x>>c;
            stat[j] += x;
        }
    }

    in.close();

    ofstream out;
    out.open("stat.csv", ios_base::app);

    for(int i=0;i<17;i++)
    {
        if(i)
            out<<(int)(stat[i]/10)<<",";
        else
            out<<"-"<<",";
    }
 
    out.close();

}

int main()
{
    read_input();

    ofstream out;
    out.open("stat.csv");
   
    for(int i=0;i<10;i++){
        init();
        fill_between_and_dest_flr();
        simulate(i+1, out);
    }

    out.close();

    calc_avg();
   

    return 0;
}