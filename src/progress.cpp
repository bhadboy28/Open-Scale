#include "openscale/progress.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
namespace openscale {
Progress::Progress(uint64_t total): total_(total), start_(std::chrono::steady_clock::now()) {}
void Progress::stage(const std::string& s){ stage_=s; update(done_); }
void Progress::update(uint64_t d){
    done_=d;
    double pct=total_?100.0*double(done_)/double(total_):0.0;
    auto sec=std::chrono::duration<double>(std::chrono::steady_clock::now()-start_).count();
    double rate=sec>0?double(done_)/sec:0;
    double eta=(rate>0&&total_>=done_)?double(total_-done_)/rate:0;
    int width=30, filled=int(pct*width/100.0); if(filled>width)filled=width;
    std::ostringstream bar;
    bar << '[' << std::string(filled,'#') << std::string(width-filled,'.') << ']';
    std::cout << '\r' << bar.str() << ' ' << std::fixed << std::setprecision(1)
              << pct << "% | " << stage_
              << " | " << std::setprecision(1) << rate/1e6 << " MB/s"
              << " | elapsed " << int(sec)/60 << ':' << std::setw(2) << std::setfill('0') << int(sec)%60
              << " | ETA ";
    if(eta>86400) std::cout << ">1d";
    else std::cout << int(eta)/60 << ':' << std::setw(2) << int(eta)%60;
    std::cout << std::setfill(' ') << std::flush;
}
void Progress::finish(){ update(total_); std::cout << "\n"; }
}
