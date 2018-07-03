#include<bits/stdc++.h>
#include<unistd.h>
#include<map>
 #include <climits>
using namespace std;

const double eps = 1e-8;
int dcmp(const double &x){ if (fabs(x) < eps) return 0; return x < 0 ? -1 : 1; }

#define DEFAULT_CACHE_SIZE 1024
#define DEFAULT_ALGORITHM 0

typedef pair<int, string> _algorithm;
typedef pair<int, int> _clock_elem;
int cache_size, working_algorithm, hit, miss;
vector<_algorithm> algorithms;
vector<int> pages;

void init();
void print_menu();
void set_cache_size();
void print_algorithms();
void set_algorithm();
void add_an_algorithm();
void generate_input();
void generate_randomly(int n);
void generate_by_hand(int n);
void print_pages();
void FIFO_algorithm();
void run_algorithm();
void read_pages();
void LRU_algorithm();
void MIN_algorithm();
void CLOCK_algorithm();
void SECOND_algorithm();

int main(){
    //freopen("1.in", "r", stdin);
    // init();
    //generate_input();
    //return 0;
    // int op;
    // while (){
    //     print_menu();
    //     cin>>op;
    //     switch (op){
    //         case 1: set_cache_size(); break;
    //         case 2: set_algorithm(); break;
    //         case 3: add_an_algorithm(); break;
    //         case 4: run_algorithm(); break;
    //         case 5: read_pages(); break;
    //         case 233: cout << "Bye..." << endl; sleep(1); return 0;
    //         default: break;
    //     }
    //     sleep(1);
    // }
    
    set_cache_size();
    set_algorithm();
    read_pages();
    
    
    run_algorithm();
    

}

void print_pages(){
    cout << "pages number is: ";
    cout << pages.size() << endl;
    cout << "pages are: ";
    for (auto &x : pages){
        cout << x << " ";
    }
    cout << endl;
}

void generate_randomly(int n){
    cout << n << endl;
    for (int i = 0; i < n; i++){
        cout << rand() * 233 % cache_size << " ";
    }
    cout << endl;
}

void generate_by_hand(int n){
    cout << n << endl;
    for (int i = 0; i < n; i++){
        int x; cin >> x; cout << x << " ";
    }
    cout << endl;
}

void generate_input(){
    pages.clear();
    cout << "Please input the number of pages: ";
    int n, op;
    cin >> n;
    cout << "1) Generate randomly" << endl;
    cout << "2) Generate by hand" << endl;
    cin >> op;
    freopen("1.in", "w", stdout);
    switch (op){
        case 1: generate_randomly(n); break;
        case 2: generate_by_hand(n); break;
        default: break;
    }
}


void read_pages(){
    pages.clear();
    int n; 
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x; 
        cin >> x; 
        pages.push_back(x);
    }
}

void print_menu(){
    cout << "**********MENU**********" << endl;
    cout << "1) set cache size" << endl;
    cout << "2) set algorithm" << endl;
    cout << "3) add an algorithm" << endl;
    cout << "4) run algorithm" << endl;
    cout << "5) read pages" << endl;
    cout << "233) quit" << endl;
}

void set_cache_size(){
    int x;
    cin >> x;
    cache_size = x;
} 

void print_algorithms(){
    cout << "All algorithms are: " << endl;
    for (auto &x : algorithms){
        cout << x.first << " : " << x.second << endl;
    }
}

void set_algorithm(){
    unsigned int id;
    cin >> id;
    working_algorithm = id;
}

void add_an_algorithm(){
    cout << "Please input the name of new algorithm: ";
    string algo;
    cin >> algo;
    algorithms.push_back(make_pair(algorithms.size(), algo));
    cout << "Algorithm added!" << endl;
    print_algorithms();
}

void FIFO_algorithm()
{
    hit = miss = 0;
    list<int> cache_list;
    cache_list.clear();
    for(auto &x : pages)
    {
        bool flag = false;
        for(auto &y: cache_list)
            if(y == x)
            {
                flag = true;
                break;
            }
        if(flag)
        {
            ++ hit;
            continue;
        }
        ++ miss;
        if(cache_list.size() == cache_size)
            cache_list.pop_back();
        cache_list.push_front(x);
    }
}


void LRU_algorithm()
{
    hit = miss = 0;
    list<int> cache_list;
    cache_list.clear();
    for(auto &x : pages)
    {
        bool flag = false;
        for(auto &y: cache_list)
            if(y == x)
            {
                flag = true;
                cache_list.remove(y);
                cache_list.push_front(x);
                break;
            }
        if(flag)
        {
            ++ hit;
            continue;
        }
        ++ miss;
        if(cache_list.size() == cache_size)
            cache_list.pop_back();
        cache_list.push_front(x);
    }
}




void run_algorithm(){
    switch (working_algorithm){
        case 0: FIFO_algorithm(); break;
        case 1: LRU_algorithm(); break;
        case 2: MIN_algorithm();break;
        case 3: CLOCK_algorithm();break;
        case 4: SECOND_algorithm();break;
        default: break;
    }
    printf("Hit ratio = %.2f%%", (double) hit/(hit+miss)*10000/100);
}
void MIN_algorithm()
{
    hit = miss = 0;
    map<int, vector<int>> page_map;
    int index = 0;
    for(int index = 0; index<pages.size();index++)
    {
        if(page_map.find(pages[index]) != page_map.end())
        {
            page_map.find(pages[index])->second.push_back(index);
        }
        else
        {
            vector<int> index_list;
            index_list.push_back(index);
            page_map.insert(make_pair(pages[index], index_list));
        }
    }
    vector<int> cache_list;
    for(int i = 0;i < pages.size();i++)
    {
        page_map.find(pages[i])->second.erase(page_map.find(pages[i])->second.begin());
        if(cache_list.empty())
        {
            ++ miss;
            cache_list.push_back(pages[i]);
            continue;
        }
        bool flag = false;
        for(int j = 0; j < cache_list.size(); j++)
            if(cache_list[j] == pages[i])
                {
                    
                    flag = true;
                    break;
                }
        if (flag)
        {
            ++ hit;
            continue;
        }
        ++ miss;
        if(cache_list.size() == cache_size)
        {
           int min = INT_MIN;
           vector<int>::iterator subs;
           bool has_erase = false;
           for(vector<int>::iterator iter=cache_list.begin();iter!=cache_list.end();++iter)
           {
               
               if(page_map.find(*iter)->second.size() == 0)
                {
                    cache_list.erase(iter);
                    has_erase = true;
                    break;
                }
                else if(page_map.find(*iter)->second.size()>=1)
                {
                    
                    int first = page_map.find(*iter)->second[0];
                    if(first>min)
                    {
                        min = first;
                        subs = iter;
                    }
                }
            
           }
           if (has_erase == false)
                cache_list.erase(subs);
           
        }
        
            cache_list.push_back(pages[i]);
            
    }
}
void CLOCK_algorithm()
{
    hit = miss = 0;
    _clock_elem cache_array[cache_size];
    int hand=0;
    int index=0;
    for(int i=0;i<pages.size();i++)
    {
        bool flag=false;
        for(int j=0;j<=index;j++)
        {
            if(cache_array[j].first==pages[i])
            {
                flag=true;
                hit++;
                cache_array[j].second=1;
                break;
            }
        }
        if(!flag)
        {
            ++miss;
            if(index<cache_size)
            {
                cache_array[index]=make_pair(pages[i],1);
                hand = (hand+1)%cache_size;
                ++index;
                
            }
            else
            {
                 while(cache_array[hand].second !=0)
                {
                    cache_array[hand].second=0;
                    hand = (hand+1)%cache_size;
                } 
                cache_array[hand]=make_pair(pages[i],1);
                hand = (hand+1)%cache_size;

            }
           
        }
    }
   
}

void SECOND_algorithm()
{
    hit = miss = 0;
    list<int> cache_fifo;
    list<int>cache_lru;
    int cache_fifo_size=cache_size/2;
    int cache_lru_size=cache_size-cache_size/2;
    for( auto &x : pages)
    {
        bool flag=false;
        for(auto &y : cache_fifo)
        {
            if(y == x)
            {
                ++hit;
                flag = true;
                break;
            }
        }
        if(flag == false)
            for(auto &y : cache_lru)
            {
                if(y == x)
                {
                    ++hit;
                    flag=true;
                    cache_fifo.push_front(y);
                    cache_lru.remove(y);
                    if(cache_fifo.size()>cache_fifo_size)
                    {
                        cache_lru.push_front(cache_fifo.back());
                        cache_fifo.pop_back();
                    }
                    break;
                }
            }
        if(flag == false)
        {
            ++miss;
            if(cache_fifo.size()<cache_fifo_size)
            {
                cache_fifo.push_front(x);
            }
            else
            {
                cache_fifo.push_front(x);
                cache_lru.push_front(cache_fifo.back());
                cache_fifo.pop_back();
                if(cache_lru.size()>cache_lru_size)
                    cache_lru.pop_back();
            }
            
        }
    }
}
