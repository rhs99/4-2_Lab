#include<bits/stdc++.h>
using namespace std;
#define MX 1000100
#define pii pair<int,int>


random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> urd(0.0, 1.0);


double mean_interval_time, mean_service_time;
int number_of_customer;

int cur_customer;

int server_status, queue_size, number_of_delay_completed;
double clk, time_of_last_event, total_delay_in_queue, total_size_of_queue, server_utilization;
double delay[MX];
double arrival_time[MX];
queue<int>q;

bool is_task_1 = true;
vector<double>uniform, interval_expo, service_expo;

struct Event{
    int customer;
    int type;
    double tm;
    Event(){

    }
    Event(int _customer,int _type, double _tm)
    {
        customer = _customer;
        type = _type;
        tm = _tm;
    }
    void print()
    {
        cout<<customer<<" -- "<<type<<"  --  "<<tm<<endl<<endl;
    }
};

struct CompareEvent {
    bool operator()(Event const& e1, Event const& e2)
    {
        return e1.tm > e2.tm;
    }
};

priority_queue<Event,vector<Event>,CompareEvent>event_list;//{time,type}...type:0->Arrival,type:1->departure

double get_expo(double mean)
{
    double u = urd(gen);

    if(is_task_1){
        uniform.push_back(u);
    }

    double e = -(mean*log(u));
    return e;
}

void schedule_next_arrival()
{
    cur_customer++;
    double inter_arrival = get_expo(mean_interval_time);

    if(is_task_1)
    {
        interval_expo.push_back(inter_arrival);
    }

    arrival_time[cur_customer] = clk + inter_arrival;
    event_list.push(Event(cur_customer, 0, arrival_time[cur_customer]));
}

void schedule_departure(int customer)
{
    double service_time = get_expo(mean_service_time);

    if(is_task_1)
    {
        service_expo.push_back(service_time);
    }

    event_list.push(Event(customer, 1, clk + service_time));
}


void init()
{
    server_status = queue_size = number_of_delay_completed = cur_customer = 0;
    clk = time_of_last_event = total_delay_in_queue = total_size_of_queue = server_utilization = 0.0;

    while(!q.empty())
    {
        q.pop();
    }

    while(!event_list.empty())
    {
        event_list.pop();
    }
   
    schedule_next_arrival();


}

void handle_arrival(Event e)
{
    schedule_next_arrival();

    if(server_status == 0)
    {
        delay[e.customer] = 0.0;
        total_delay_in_queue += 0.0;
        number_of_delay_completed++;
        server_status = 1;
        schedule_departure(e.customer);
    }
    else
    {
        queue_size++;
        q.push(e.customer);
    }
}

void handle_departure(Event e)
{
    if(queue_size == 0)
    {
        server_status = 0;
    }
    else
    {
        queue_size--;
        
        int nxt = q.front();
        q.pop();

        delay[nxt] = clk - arrival_time[nxt];
        total_delay_in_queue += delay[nxt];

        number_of_delay_completed++;

        schedule_departure(nxt);
    }
}

void calculate_stat()
{
    double time_gap = clk - time_of_last_event;
    time_of_last_event = clk;

    total_size_of_queue += (time_gap * queue_size);
    server_utilization += (time_gap * server_status);

}


void task_1()
{
    ifstream in;
    in.open("input.txt");

    in>>mean_interval_time>>mean_service_time>>number_of_customer;

    init();

    while(number_of_delay_completed < number_of_customer)
    {
        Event cur_event = event_list.top();
        event_list.pop();

        clk = cur_event.tm;

        calculate_stat();

        if(cur_event.type == 0)
        {
            handle_arrival(cur_event);
        }
        else
        {
            handle_departure(cur_event);
        }
    }

    ofstream out;
    out.open("task_1.txt");

    out<<total_delay_in_queue/number_of_customer<<"\n"<<total_size_of_queue/clk<<"\n"<<server_utilization/clk<<"\n"<<clk<<endl; 

    in.close();
    out.close();
}


void task_2()
{
    is_task_1 = 0;
    
    ofstream out;
    out.open("task_2.csv");

    out<<"k,average_delay_in_queue,average_number_in_queue,server_utilization,time_the_simulation_ended\n"; 

    double k_vals[] = {0.5, 0.6, 0.7, 0.8, 0.9};

    
    for(int k=0;k<5;k++)
    {
        mean_service_time = mean_interval_time * k_vals[k];

        init();

        while(number_of_delay_completed < number_of_customer)
        {
            Event cur_event = event_list.top();
            event_list.pop();

            clk = cur_event.tm;

            calculate_stat();

            if(cur_event.type == 0)
            {
                handle_arrival(cur_event);
            }
            else
            {
                handle_departure(cur_event);
            }
        }

        out<< k_vals[k]<<","<<total_delay_in_queue/number_of_customer<<","<<total_size_of_queue/clk<<","<<server_utilization/clk<<","<<clk<<endl; 

    }

    out.close();

}


void task_3()
{
    ofstream out;
    out.open("task_3.txt");

    for(auto x : uniform)
    {
        out<<x<<" ";
    }
    out<<"\n";

    for(auto x : interval_expo)
    {
        out<<x<<" ";
    }
    out<<"\n";

    for(auto x : service_expo)
    {
        out<<x<<" ";
    }
    out<<"\n";

    out.close();

}


int main()
{

    task_1();

    task_2();

    task_3();
     
    return 0;
}