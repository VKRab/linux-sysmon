#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>
#include <iomanip>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <sys/statvfs.h>

//uptime in sec
long gutime(){
    std::ifstream f("/proc/uptime");
    if(!f.is_open())return -1;
    double s;
    f>>s;
    return(long)s;
}

//format uptime
std::string fmtup(long s){
    long d=s/86400;
    s%=86400;
    long h=s/3600;
    s%=3600;
    long m=s/60;
    std::ostringstream o;
    if(d>0)o<<d<<"d ";
    o<<std::setw(2)<<std::setfill('0')<<h<<"h "
     <<std::setw(2)<<std::setfill('0')<<m<<"m";
    return o.str();
}

//cpu load
double gcusage(){
    static unsigned long long pidle=0,ptotal=0;
    unsigned long long u,n,sy,i,io,ir,sir,st;
    std::ifstream f("/proc/stat");
    if(!f.is_open())return -1.0;
    std::string l;
    std::getline(f,l);
    std::istringstream ss(l);
    ss.ignore(5);
    ss>>u>>n>>sy>>i>>io>>ir>>sir>>st;
    unsigned long long idle=i+io;
    unsigned long long total=u+n+sy+idle+ir+sir+st;
    unsigned long long didle=idle-pidle;
    unsigned long long dtotal=total-ptotal;
    pidle=idle;
    ptotal=total;
    if(dtotal==0)return 0.0;
    return 100.0*(dtotal-didle)/dtotal;
}

//ram
void gmem(unsigned long long& t,unsigned long long& u){
    struct sysinfo i;
    sysinfo(&i);
    t=i.totalram*i.mem_unit/1024;
    u=(i.totalram-i.freeram)*i.mem_unit/1024;
}

//disk
void gdisk(const std::string& p,unsigned long long& t,unsigned long long& u){
    struct statvfs s;
    if(statvfs(p.c_str(),&s)!=0){
        t=u=0;
        return;
    }
    t=(s.f_frsize*s.f_blocks)/1024;
    u=(s.f_frsize*(s.f_blocks-s.f_bfree))/1024;
}

//process list
std::vector<std::tuple<int,std::string,char>> gprocs(int max=10){
    std::vector<std::tuple<int,std::string,char>> v;
    DIR* d=opendir("/proc");
    if(!d)return v;
    dirent* e;
    while((e=readdir(d))!=nullptr){
        if(e->d_type!=DT_DIR)continue;
        int pid=std::atoi(e->d_name);
        if(pid<=0)continue;
        std::string cpath="/proc/"+std::string(e->d_name)+"/comm";
        std::ifstream c(cpath);
        if(!c.is_open())continue;
        std::string name;
        std::getline(c,name);
        if(name.empty())continue;
        std::string spath="/proc/"+std::string(e->d_name)+"/stat";
        std::ifstream s(spath);
        if(!s.is_open())continue;
        std::string l,tok;
        std::getline(s,l);
        std::istringstream ss(l);
        int i=0;
        char st='?';
        while(ss>>tok){
            if(i==2){
                st=tok[0];
                break;
            }
            i++;
        }
        v.emplace_back(pid,name,st);
        if((int)v.size()>=max)break;
    }
    closedir(d);
    return v;
}

//draw screen
void draw(){
    system("clear");
    std::cout<<"Linux System Monitor (CLI)\n\n";
    double cpu=gcusage();
    std::cout<<"CPU Load: "<<std::fixed<<std::setprecision(1)<<cpu<<"%\n";
    unsigned long long rt,ru;
    gmem(rt,ru);
    double rp=rt?100.0*ru/rt:0.0;
    std::cout<<"RAM: "<<ru/1024<<" MB / "<<rt/1024<<" MB ("
             <<std::fixed<<std::setprecision(1)<<rp<<"%)\n";
    unsigned long long dt,du;
    gdisk("/",dt,du);
    double dp=dt?100.0*du/dt:0.0;
    std::cout<<"Disk (/): "<<du/1024<<" MB / "<<dt/1024<<" MB ("
             <<std::fixed<<std::setprecision(1)<<dp<<"%)\n";
    long up=gutime();
    std::cout<<"Uptime: "<<fmtup(up)<<"\n\n";
    std::cout<<"Top Active Processes (PID | Name | State):\n";
    auto p=gprocs(10);
    for(auto&[pid,name,st]:p){
        std::cout<<std::setw(6)<<pid<<"|"
                 <<std::setw(20)<<std::left<<name.substr(0,20)
                 <<" | "<<st<<"\n";
    }
    std::cout<<"\nUpdates every 2 seconds. Press Ctrl+C to exit\n";
}

int main(){
    std::cout<<"Starting the system monitor...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    while(true){
        draw();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
