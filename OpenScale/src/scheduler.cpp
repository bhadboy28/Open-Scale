#include "openscale/scheduler.hpp"
#include <thread>
#include <algorithm>
namespace openscale {
Scheduler::Scheduler(size_t w):workers_(w?w:std::max(1u,std::thread::hardware_concurrency())){}
size_t Scheduler::workers()const{return workers_;}
void Scheduler::set_workers(size_t w){workers_=std::max<size_t>(1,w);}
size_t Scheduler::choose_workers(double cpu,double ram)const{ size_t w=workers_; if(cpu>0.9||ram>0.9) w=std::max<size_t>(1,w/2); else if(cpu<0.5&&ram<0.8) w=std::min(workers_,w+1); return w; }
}
