#include "openscale/openscale.hpp"
#include <cassert>
#include <iostream>
int main(){std::vector<float>a(4,2),b(4,3);assert(openscale::math::dot_f32(a,b)==24);assert(openscale::detect_hardware().threads>0);std::cout<<"OpenScale tests passed\n";}
