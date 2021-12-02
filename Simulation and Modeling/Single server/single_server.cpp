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
    double e = -(mean*log(u));
    return e;
}

void schedule_next_arrival()
{
    cur_customer++;
    double inter_arrival = get_expo(mean_interval_time);
    arrival_time[cur_customer] = clk + inter_arrival;
    event_list.push(Event(cur_customer, 0, arrival_time[cur_customer]));
}

void schedule_departure(int customer)
{
    double service_time = get_expo(mean_service_time);
    event_list.push(Event(customer, 1, clk + service_time));
}


void init()
{
    server_status = queue_size = number_of_delay_completed = cur_customer = 0;
    clk = time_of_last_event = total_delay_in_queue = total_size_of_queue = server_utilization = 0.0;
   
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

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);

    

    cin>>mean_interval_time>>mean_service_time>>number_of_customer;

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

    cout<<total_delay_in_queue/number_of_customer<<"\n"<<total_size_of_queue/clk<<"\n"<<server_utilization/clk<<"\n"<<clk<<endl;
    

    return 0;
}