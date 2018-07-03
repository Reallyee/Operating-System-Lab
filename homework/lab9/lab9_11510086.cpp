#include<bits/stdc++.h>
#include<unistd.h>
#include<map>
#include<climits>
#include<math.h>
using namespace std;
void read_data();
void FCFS_algorithm();
void SSTF_algorithm();
void SCAN_algorithm();
void C_SCAN_algorithm();
void LOOK_algorithm();
void C_LOOK_algorithm();
int start_sector;
int max_track;
int sector_number;
vector<int> sector;
int main()
{
  read_data();
  FCFS_algorithm();    
  SSTF_algorithm();
  SCAN_algorithm();
  C_SCAN_algorithm();
  LOOK_algorithm();
  C_LOOK_algorithm();
}

void read_data()
{
    cin>>start_sector>>max_track>>sector_number;
    int input_sector;
    for(int i=0;i<sector_number;i++)
    {
        cin>>input_sector;
        sector.push_back(input_sector);
    }
}

void FCFS_algorithm()
{
    cout<<"FCFS\n";
    int sum = 0;
    sum += abs(sector[0]-start_sector);
    cout<<start_sector<<"->";
    if(sector.size()>1)
        for (int i=1;i<sector.size();i++)
        {
            sum += abs(sector[i]-sector[i-1]);
            cout<<sector[i-1]<<"->";
        }
        cout<<sector[sector.size()-1]<<"\n"<<sum<<"\n";
            
}

void SSTF_algorithm()
{
    cout<<"SSTF\n";
    int sum=0;
    int pointer_sector=start_sector;
    cout<<start_sector<<"->";
    int is_check[sector.size()];
    for (int i=0;i<sector.size();i++)
        is_check[i] = 0;
    int index;
    int min;
    int number = 0;
    for(int i=0;i<sector.size();i++)
    {
        min = __INT32_MAX__;
        for(int j=0;j<sector.size();j++)
        {
            if(abs(sector[j]-pointer_sector)<min &&  &pointer_sector!= &sector[j] && is_check[j]==0)
                {
                    index = j;
                    min = abs(sector[j]-pointer_sector);
                }
           
        }
         sum += min;
            pointer_sector = sector[index];
            is_check[index]=1;
            number++;
            if(number<sector.size())
                cout<<pointer_sector<<"->";
            else
                cout<<pointer_sector<<"\n"<<sum<<"\n";
    }
}
void SCAN_algorithm()
{
    int sum = 0;
    list<int> sort_sector;
    for(int i=0;i<sector.size();i++)
        sort_sector.push_back(sector[i]);
    sort_sector.sort();
    int sort_sector_array[sort_sector.size()];
    int i = 0;
    for(auto &x : sort_sector)
    {
        sort_sector_array[i]=x;
        i++;
    }
    for(i=0;i<sort_sector.size();i++)
    {
        if(sort_sector_array[i]>start_sector)
        {
            break;
        }   
    }
    
    int index_left = i-1;
    int index_right = i;
    cout<<"SCAN\n";
    cout<<start_sector<<"->";
    sum += abs(start_sector-sort_sector_array[index_left]);
    while(index_left> 0)
    {
         sum += abs(sort_sector_array[index_left]-sort_sector_array[index_left-1]);
         cout<<sort_sector_array[index_left]<<"->";
         index_left =index_left-1;
     }
     cout<<sort_sector_array[index_left]<<"->";
     sum += sort_sector.front()+ sort_sector_array[index_right];
     cout<<"0->";
     while(index_right<sort_sector.size()-1)
     {
         sum += sort_sector_array[index_right+1]-sort_sector_array[index_right];
         cout<<sort_sector_array[index_right]<<"->";
         index_right=index_right+1;
     }
    cout<<sort_sector_array[index_right]<<"\n"<<sum<<"\n";
}
void C_SCAN_algorithm()
{
    int sum = 0;
    list<int> sort_sector;
    for(int i=0;i<sector.size();i++)
        sort_sector.push_back(sector[i]);
    sort_sector.sort();
    int sort_sector_array[sort_sector.size()];
    int i = 0;
    for(auto &x : sort_sector)
    {
        sort_sector_array[i]=x;
        i++;
    }
    for(i=0;i<sort_sector.size();i++)
    {
        if(sort_sector_array[i]>start_sector)
        {
            break;
        }   
    }
    
    int index_left = i-1;
    int index_right = i;
    cout<<"C_SCAN\n";
    cout<<start_sector<<"->";
    sum += abs(start_sector-sort_sector_array[index_left]);
    while(index_left> 0)
    {
         sum += abs(sort_sector_array[index_left]-sort_sector_array[index_left-1]);
         cout<<sort_sector_array[index_left]<<"->";
         index_left =index_left-1;
     }
     cout<<sort_sector_array[index_left]<<"->";
     sum += sort_sector.front()+ max_track-1-sort_sector_array[sector.size()-1];
     cout<<"0->"<<(max_track-1)<<"->";
     index_left=sector.size()-1;
     while(index_right<index_left)
     {
         sum += sort_sector_array[index_left]-sort_sector_array[index_left-1];
         cout<<sort_sector_array[index_left]<<"->";
         index_left=index_left-1;
     }
    cout<<sort_sector_array[index_left]<<"\n"<<sum<<"\n";
}
void LOOK_algorithm()
{
    int sum = 0;
    list<int> sort_sector;
    for(int i=0;i<sector.size();i++)
        sort_sector.push_back(sector[i]);
    sort_sector.sort();
    int sort_sector_array[sort_sector.size()];
    int i = 0;
    for(auto &x : sort_sector)
    {
        sort_sector_array[i]=x;
        i++;
    }
    for(i=0;i<sort_sector.size();i++)
    {
        if(sort_sector_array[i]>start_sector)
        {
            break;
        }   
    }
    
    int index_left = i-1;
    int index_right = i;
    cout<<"LOOK\n";
    cout<<start_sector<<"->";
    sum += abs(start_sector-sort_sector_array[index_left]);
    while(index_left> 0)
    {
         sum += abs(sort_sector_array[index_left]-sort_sector_array[index_left-1]);
         cout<<sort_sector_array[index_left]<<"->";
         index_left =index_left-1;
     }
     sum+=sort_sector_array[index_right]-sort_sector_array[index_left];
     cout<<sort_sector_array[index_left]<<"->";
     while(index_right<sort_sector.size()-1)
     {
         sum += sort_sector_array[index_right+1]-sort_sector_array[index_right];
         cout<<sort_sector_array[index_right]<<"->";
         index_right=index_right+1;
     }
    cout<<sort_sector_array[index_right]<<"\n"<<sum<<"\n";
}
void C_LOOK_algorithm()
{
    int sum = 0;
    list<int> sort_sector;
    for(int i=0;i<sector.size();i++)
        sort_sector.push_back(sector[i]);
    sort_sector.sort();
    int sort_sector_array[sort_sector.size()];
    int i = 0;
    for(auto &x : sort_sector)
    {
        sort_sector_array[i]=x;
        i++;
    }
    for(i=0;i<sort_sector.size();i++)
    {
        if(sort_sector_array[i]>start_sector)
        {
            break;
        }   
    }
    
    int index_left = i-1;
    int index_right = i;
    cout<<"C_LOOK\n";
    cout<<start_sector<<"->";
    sum += abs(start_sector-sort_sector_array[index_left]);
    while(index_left> 0)
    {
         sum += abs(sort_sector_array[index_left]-sort_sector_array[index_left-1]);
         cout<<sort_sector_array[index_left]<<"->";
         index_left =index_left-1;
     }
     cout<<sort_sector_array[index_left]<<"->";
     index_left=sector.size()-1;
     while(index_right<index_left)
     {
         sum += sort_sector_array[index_left]-sort_sector_array[index_left-1];
         cout<<sort_sector_array[index_left]<<"->";
         index_left=index_left-1;
     }
    cout<<sort_sector_array[index_left]<<"\n"<<sum<<"\n";
}