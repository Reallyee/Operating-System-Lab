#include <stdio.h>
#include <vector>
#include<string.h>
#include<string>
#include<iostream>
using namespace std;
struct User{
    string user_name;//user name
    string password;//user password
}; 
struct File{
    int if_is_directory;// flag to identify is file is a directory(not the same definition in class)
    int flag;//if the file is open
    string file_name;//file name
    int file_protect[3];// file execute right
    string path;//file path of the direcitory
    int pointer;//file content pointer
    string content;//file content
    vector<struct File> file_list;//if file is a directory, it has file list
    string file_creater;
    string file_opener;
    string file_mode;//create mode
};
struct User current_user;
struct File* current_directory;
struct File root_directory;// file /
struct arg{//struct to define argument
    string first_arg;
    string second_arg;
    string third_arg;
    string fouth_arg;
    string fifth_arg;
};
vector<User> user_list;// all the users
vector<File*> open_file_list;// all the opened files
struct arg argument;//the arguments of every command
vector<int> protect(3,0);//read write execute
void man();
void init();
void login(struct arg argument);
void reguser(struct arg argument);
void delusr(struct arg argument);
void disusr(struct arg argument);
void ls();
void pwd();
void lsp(struct arg argument);
void mkdir(struct arg argument);
void mkdirp(struct arg argument);
struct File* find_path(string path);
int if_file_exists(struct File file, string name);
int if_directory_exists(struct File file, string name);
void cd(struct arg argument);
void cdp(struct arg argument);
void dedir(struct File* file_path, string name);
void create(struct File*, string,string);
void read_argument(int number);
void search(string);
void open_file(struct File*, string);
void lsfile();
void write(struct File*,string,string,string);
void reposition(struct File*, string,string);
void cat(struct File*,string);
void truncate(struct File*, string, string);
void chmod(struct File*, string ,string);
void close(struct File*,string);
void delete_file(struct File*,string);
int main(void)
{
    init();//inital the root
    while(1)
    {
        cout<<current_user.user_name;
        printf("$");
        cout<<(*current_directory).path;
        printf("%%:");
        string arg1;
        cin>>arg1;
        argument.first_arg=arg1;
        if(argument.first_arg=="exit")
            return 0;
        if(argument.first_arg=="man")
            man();
        if(argument.first_arg=="login")
        {
            read_argument(2);
            login(argument);
        }
            
        if(argument.first_arg=="regusr")
        {
            read_argument(3);
            reguser(argument);
        }
            
        if(argument.first_arg== "delusr")
        {
            read_argument(2);
            delusr(argument);
        }
        if(argument.first_arg== "disusr")
        {
            disusr(argument);
        }

        if(argument.first_arg=="pwd")
        {
            pwd();
        }
        if(argument.first_arg=="ls")
        {
            ls();
        }
        if(argument.first_arg=="lsp")
        {
            read_argument(2);
            lsp(argument);
        }
        if(argument.first_arg=="mkdir")
        {
           read_argument(2);
            mkdir(argument);
        }
         if(argument.first_arg=="mkdirp")
        {
           read_argument(3);
            mkdirp(argument);
        }  
        if(argument.first_arg=="cd")
        {
            read_argument(2);
            cd(argument);
        }
        if(argument.first_arg=="cdp")
        {
            read_argument(2);
            cdp(argument);
        }
        if(argument.first_arg=="dedir")
        {
            read_argument(2);
            dedir(current_directory, argument.second_arg);
        }
        if(argument.first_arg=="dedirp")
        {
            read_argument(3);
            struct File*file_path=find_path(argument.second_arg);
            if(file_path)
            dedir(file_path, argument.third_arg);
        }
        if(argument.first_arg=="create")
        {
            read_argument(3);
            create(current_directory,argument.second_arg,argument.third_arg);
        }
        if(argument.first_arg=="createp")
        {
            read_argument(4);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
            create(file_path, argument.third_arg, argument.fouth_arg);
        }
        if(argument.first_arg=="search")
        {
            read_argument(2);
            search(argument.second_arg);
        }
        if(argument.first_arg=="open")
        {
            read_argument(2);
            open_file(current_directory, argument.second_arg);
        }
        if(argument.first_arg=="openp")
        {
            read_argument(3);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
            open_file(file_path, argument.third_arg);
        }
        if(argument.first_arg=="lsfile")
        {
            lsfile();
        }
        if(argument.first_arg=="write")
        {
            read_argument(4);
            write(current_directory,argument.second_arg,argument.third_arg,argument.fouth_arg);
        }
        if(argument.first_arg=="writep")
        {
            read_argument(5);
            struct File* file_path=find_path(argument.second_arg);
            if(find_path)
            write(file_path, argument.third_arg,argument.fouth_arg,argument.fifth_arg);
        }
        if(argument.first_arg=="reposition")
        {
            read_argument(3);
            reposition(current_directory,argument.second_arg,argument.third_arg);
        }
        if(argument.first_arg=="repositionp")
        {
            read_argument(4);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
            reposition(file_path,argument.third_arg,argument.fouth_arg);
        }
        if(argument.first_arg=="cat")
        {
            read_argument(2);
            cat(current_directory,argument.second_arg);
        }
        if(argument.first_arg=="catp")
        {
            read_argument(3);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
            cat(file_path,argument.third_arg);
        }
        if(argument.first_arg=="truncate")
        {
            read_argument(3);
            truncate(current_directory,argument.second_arg,argument.third_arg);
        }
        if(argument.first_arg=="truncatep")
        {
            read_argument(4);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
                truncate(file_path,argument.third_arg,argument.fouth_arg);
        }
        if(argument.first_arg=="chmod")
        {
            read_argument(3);
            chmod(current_directory,argument.second_arg,argument.third_arg);
        }
        if(argument.first_arg=="chmodp")
        {
            read_argument(4);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
                chmod(file_path,argument.third_arg,argument.fouth_arg);
        }
        if(argument.first_arg=="close")
        {
            read_argument(2);
            close(current_directory,argument.second_arg);
        }
        if(argument.first_arg=="closep")
        {
            read_argument(3);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
                close(file_path, argument.third_arg);
        }
        if(argument.first_arg=="delete")
        {
            read_argument(2);
            delete_file(current_directory,argument.second_arg);
        }
        if(argument.first_arg=="deletep")
        {
            read_argument(3);
            struct File* file_path=find_path(argument.second_arg);
            if(file_path)
                delete_file(file_path,argument.third_arg);
        }
        
    }      
}
void read_argument(int number)//read arguments
{
    string arg1;
    if(number==2)
    {
        cin>>arg1;
        argument.second_arg=arg1;
        return;
    }
    if (number==3)
    {
        cin>>arg1;
        argument.second_arg=arg1;
        cin>>arg1;
        argument.third_arg=arg1;
        return;
    }
    if(number==4)
    {
        cin>>arg1;
        argument.second_arg=arg1;
        cin>>arg1;
        argument.third_arg=arg1;
        cin>>arg1;
        argument.fouth_arg=arg1;
    }
    if(number==5)
    {
        cin>>arg1;
        argument.second_arg=arg1;
        cin>>arg1;
        argument.third_arg=arg1;
        cin>>arg1;
        argument.fouth_arg=arg1;
        cin>>arg1;
        argument.fifth_arg=arg1;
    }
}
void init()
{
    struct User root;
    root.user_name="root";
    root.password="111";
    root_directory.if_is_directory=1;
    root_directory.file_name="/";
    root_directory.path="/";
    current_directory=&root_directory;
    current_user=root;
    user_list.push_back(root);
}

void man()
{
    puts("Manual:");
    puts("0) pwd (current work directory)");
    puts("1) mkdir(p) (path) directory_name");
    puts("2) dedir(p) (path) directory_name");
    puts("3) search file_name");
    puts("4) create(p) (path) file_name omode");
    puts("5) cat(p) (path) file_name");
    puts("6) delete(p) (path) file_name");
    puts("7) write(p) (path) file_name buff wmode");
    puts("8) reposition(p) (path) file_name pos");
    puts("9) truncate(p) (path) file_name cnt");
    puts("10) ls(p) (path)");
    puts("11) open(p) (path) file_name");
    puts("12) close(p) (path) file_name");
    puts("13) reguser user_name password");
    puts("14) delusr user_name [root only]");
    puts("15) disusr");
    puts("16) login user_name");
    puts("17) lsfile");
    puts("18) chmod(p) (path) file_name mode");
    puts("19) cd(p) (path) dir_name");
    puts("20) exit");
}

void login(struct arg argument)
{
    string name=argument.second_arg;
    printf("%s","passwrod:");
    string password;
    cin>>password;
    int find=0;
    struct User login_user;
    for(auto &iter:user_list)
    {
        if(iter.user_name== name)
        {    
            login_user.user_name=iter.user_name;
            login_user.password=iter.password;
            find=1;
            break;    
        }
    }
    if(find)
    {
        if(login_user.password==password)
        {
            puts("login ok");
            current_user=login_user;
        }
        else{
            puts("password wrong");
        }
       
    }
    else
    {
        puts("user does not exist");
    }
}
void reguser(struct arg argument)
{
    string name=argument.second_arg;
    string password=argument.third_arg;
    int find=0;
    for(auto &iter:user_list)
    {
        if(iter.user_name== name)
        {
           find=1;
           break;
        }
    }
    if(find)
    {
        puts("user already exist");
    }
    else
    {
        struct User user;
        user.user_name=name;
        user.password=password;
        user_list.push_back(user);
       puts("register ok");
    }
}
void delusr(struct arg argument)
{
    int find=0;
    string name=argument.second_arg;
    int index=0;
    if(name=="root")
    {
        cout<<"permission denied"<<"\n";
        return;
    }
    if(current_user.user_name == (*user_list.begin()).user_name)
    {
        for(auto &iter:user_list)
        {

            if(iter.user_name== name)
            { 
                find=1;
                break;
            }  
            index++; 
        } 
        user_list.erase(user_list.begin()+index);
    }
    else
    {
        puts("permission denied");
    }
    if(find==0)
        puts("user does not exist");
}
void disusr(struct arg argument)
{
    if(current_user.user_name!="root")
    {
        cout<<"permission denied"<<"\n";
        return;
    }
    for(auto &iter:user_list)
    {
        cout<<iter.user_name<<"\n";     
    }
}
void ls()
{
    if(!((*current_directory).file_list.empty()))
    {
        for (auto &file:(*current_directory).file_list)
            if(file.if_is_directory==1)
                cout<<file.file_name<<"\n";
        for (auto &file:(*current_directory).file_list)
            if(file.if_is_directory==0)
                cout<<file.file_name<<"\n";
    }
}
void pwd()
{
    cout<<(*current_directory).path<<"\n";
}
struct File* find_path(string path)
{
    if(path=="/")
    {
        return &root_directory;
    }
    else
    {
        string a=path;
        struct File* ls_file=&root_directory;
        int start=1;
        int end=0;
        int find=0;
        while(end != (a.length()-1))
        {
            end++;
            if(a[end]=='/')
            {
                string str=a.substr(start, end-start);
                start=end+1;
                if(!(*ls_file).file_list.empty())
                for(auto &directory:(*ls_file).file_list)
                {
                   if(directory.file_name==str&&directory.if_is_directory==1)
                    {
                        ls_file=&directory;
                        find=1;
                        break;
                    }
                }
            }
        }
        if(find)
            return ls_file;
        else
        {
            cout<<"directory does not exist"<<"\n";
            return 0;
        }
    }     
}
void lsp(struct arg argument)
{
    struct File* ls_file=find_path(argument.second_arg);
        if(ls_file)
        if(!((*ls_file).file_list.empty()))
        {
            for (auto &file:(*ls_file).file_list)
                if(file.if_is_directory==1)
                    cout<<file.file_name<<"\n";
            for (auto &file:(*ls_file).file_list)
                if(file.if_is_directory==0)
                    cout<<file.file_name<<"\n";
        }
    
}
int if_directory_exist(struct File directory, string name)
{
    for (auto &file:directory.file_list)
        if(file.file_name==name&&file.if_is_directory==1)
            {
                cout<<"directory already exists"<<"\n";
                return 1;
            }
    return 0;
}
void mkdir(struct arg argument)
{
    if(if_directory_exist((*current_directory),argument.second_arg))
        return;
    struct File file;
    file.file_name=argument.second_arg;
    file.if_is_directory=1;
    file.flag=0;
    file.path=(*current_directory).path+argument.second_arg+"/";
    (*current_directory).file_list.push_back(file);
}
void mkdirp(struct arg argument)
{
    struct File* mkdirp_file=find_path(argument.second_arg);
    if(mkdirp_file)
    if(if_directory_exist((*mkdirp_file), argument.third_arg))
        return;
    struct File file;
    file.file_name=argument.third_arg;
    file.if_is_directory=1;
    file.flag=0;
    file.path=(*mkdirp_file).path+(*mkdirp_file).file_name+"/";
    (*mkdirp_file).file_list.push_back(file);
}
void cd(struct arg argument)
{
    if(!(*current_directory).file_list.empty())
    {
        int find=0;
        for(auto &file:(*current_directory).file_list)
            if (file.file_name==argument.second_arg&&file.if_is_directory==1)
            {
                current_directory=&file;
                find=1;
                break;
            }
        if(!find)
        {
            cout<<"directory does not exist"<<"\n";
        }
    }
    else
    {
        cout<<"directory does not exist"<<"\n";
    }
}
void cdp(struct arg argument)
{
    struct File* cdp_file=find_path(argument.second_arg);
    if(cdp_file)
        current_directory=cdp_file;
}
void dedir(struct File* file_path, string name)
{
    if(!(*file_path).file_list.empty())
    {
        int find=0;
        int index=0;
        for(auto &file:(*file_path).file_list)
        {
            if(file.file_name==name&&file.if_is_directory==1)
            {
                find=1;
                break;
            }
            index++;
        }
        (*file_path).file_list.erase((*file_path).file_list.begin()+index);
        if(!find)
            cout<<"file does not exist\n";
    }
}
int if_file_exist(struct File directory,string name)
{
    for(auto &file:directory.file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            cout<<"file already exist"<<"\n";
            return 1;
        }
    }
    return 0;
}
void priority(int mode)
{
    for(int i=0;i<3;i++)
        protect[i]=0;
    for(int i=0;i<3;i++)
    {
        if(mode==1 || mode==5 || mode==7 || mode==3)
        {
            protect[2]=1;
        }
        if(mode==2 || mode==6 || mode==7 || mode==3)
        {
            protect[1]=1;
        }
        if(mode==4 || mode==6 || mode==5 || mode==7 )
        {
            protect[0]=1;
        }
    }
}
void create(struct File* file_create, string name, string mode)
{
   if(!if_file_exist((*file_create), name))
   {
        struct File file;
        file.file_name=name;
        file.if_is_directory=0;
        file.pointer=0;
        priority(stoi(mode));
        file.file_protect[0]=protect[0];
        file.file_protect[1]=protect[1];
        file.file_protect[2]=protect[2];
        file.flag=0;
        file.file_mode=mode;
        file.file_creater=current_user.user_name;
        file.path=(*file_create).path;
        (*file_create).file_list.push_back(file);
   }    
}
void search(string name)
{
    int find=0;
    vector<File> search_list;
    search_list.push_back(root_directory);
    struct File directory;
    while(!search_list.empty())
    {
        directory=*search_list.begin();
        for(auto &file:directory.file_list)
        {
            if(file.if_is_directory==1)
            {
                search_list.push_back(file);
            }
            else
            {
                if(file.file_name==name)
                {
                    cout<<file.path<<file.file_name<<"\n";
                    find=1;
                }
                
            }
        }
        search_list.erase(search_list.begin());
    }
    if(!find)
        cout<<"file does not found\n";
}
void open_file(struct File* file_path, string name)
{
    int find=0;
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            if(file.flag==0)
            {
            file.flag=1;
            file.file_opener=current_user.user_name;
            open_file_list.push_back(&file);
            return;
            }
            else
            {
                cout<<"file already open"<<"\n";
                return;
            }
        }
    }
        cout<<"file does not exist"<<"\n";
}
void lsfile()
{
    if(!open_file_list.empty())
    for(auto &file:open_file_list)
        cout<<(*file).path<<" "<<(*file).file_name<<" "<<(*file).file_creater<<" "<<(*file).file_opener<<" "<<(*file).file_mode<<"\n";
}
void write(struct File* file_path, string name, string content, string mode)
{
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            if(file.flag==0)
            {
                cout<<"file does not open"<<"\n";
                return;
            }
            else
            {
                if(file.file_protect[1]==1)
                {
                    int file_mode=stoi(mode);
                    if(file_mode==0)
                    file.content=file.content+content;
                    if(file_mode==1)
                    {
                        if(file.pointer+content.length()<=file.content.length()-1)
                            file.content.erase(file.pointer,file.pointer+content.length());
                        else
                            file.content.erase(file.pointer, file.content.length()-1);
                        file.content.insert(file.pointer,content);
                    }
                    if(file_mode==2)
                        file.content.insert(file.pointer,content);
                    cout<<"write ok\n";
                    return;
                }
                else
                {
                    cout<<"permission denied"<<"\n";
                    return;
                }
            }
            
            
        }
    }
    cout<<"file does not exist"<<"\n";
}
void reposition(struct File* file_path,string name, string pos)
{
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            if(stoi(pos)<(file.content.length()-1))
            {
                file.pointer=stoi(pos);
                cout<<"reposition ok"<<"\n";
                return;
            }
            else
            {
                cout<<"reposition failed"<<"\n";
                return;
            }
        }
    }
    cout<<"file does not exist"<<"\n";
}
void cat(struct File* file_path, string name)
{
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            if(file.flag==0)
            {
               cout<<"file does not open"<<"\n";
               return;

           }
           else
           {
               if(file.file_protect[0]==1)
                {
                   cout<<file.content<<"\n";
                    return;
                }
                else
                {
                    cout<<"permission denied"<<"\n";
                    return;
                }
           }
        }
    }
    cout<<"file does not exist"<<"\n";
}
void truncate(struct File* file_path, string name, string cnt)
{
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            if(stoi(cnt)<file.content.length())
                file.content.erase(stoi(cnt));
        }
    }
}
void chmod(struct File* file_path, string name, string mode)
{
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            file.file_mode=mode;
            priority(stoi(mode));
            file.file_protect[0]=protect[0];
            file.file_protect[1]=protect[1];
            file.file_protect[2]=protect[2];
            return;
        }
    }
    cout<<"file does not exist\n";
}
void close(struct File* file_path, string name)
{
    for(auto &file:(*file_path).file_list)
    {
        if(file.file_name==name && file.if_is_directory==0)
        {
            if(file.flag==1)
            {
                file.flag=0;
                int index=0;
                for(auto &open_file:open_file_list)
                {

                    if((*open_file).file_name==name&& ((*open_file).path==file.path))
                        break;
                    index++;
                }
                open_file_list.erase(open_file_list.begin()+index);
                return;
            }
            else
            {
                cout<<"file dose not open\n";
                return;
            }
            
        }
    }
    cout<<"file dose not exist\n";
}
void delete_file(struct File* file_path,string name)
{
    if(!(*file_path).file_list.empty())
    {
        int find=0;
        int index=0;
        for(auto &file:(*file_path).file_list)
        {

            if(file.file_name==name && file.if_is_directory==0)
            {
                if(file.flag==1)
                    close(file_path, name);
                find=1;
                break;
            }
            index++;
        }
        (*file_path).file_list.erase((*file_path).file_list.begin()+index);
        if(!find)
            cout<<"file does not exist\n";
    }
}
