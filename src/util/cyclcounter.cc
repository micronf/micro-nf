#include "cyclcounter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>

cyclcounter::cyclcounter() : _cycle(0), _stdev(0.0), _mean(0.0)
{
}

cyclcounter::~cyclcounter()
{
}

void
cyclcounter::update() 
{
    _cycle = cyclcounter::start_rdtsc();
}

void
cyclcounter::addone()
{
    _cycles.push_back(cyclcounter::end_rdtsc() - _cycle);
}

void
cyclcounter::addmany(int num) {
	uint64_t ttl = cyclcounter::end_rdtsc() - _cycle;
	uint64_t each = ttl / num;
	for (int i = 0; i < num; i++)
		_cycles.push_back(each);
}

void
cyclcounter::writresult(string str)
{
    std::ofstream file;
    std::vector<uint64_t>::iterator it;

    file.open(str.c_str());
    if ( !file.fail() ) {
        for (it=_cycles.begin(); it!=_cycles.end(); it++) {
            file << *it;
            file << "\n";
        }
        file.close();
    }
}

string
cyclcounter::get_stdev()
{
    double dev = 0.0;
    std::vector<uint64_t>::iterator it;
    //get_mean();
    
    for (it=_cycles.begin(); it!=_cycles.end(); it++) {
        dev += std::pow(*it - _mean, 2);
    }
    _stdev = std::sqrt(dev/_cycles.size());
    return "stdev: " + to_string(_stdev);
}

string
cyclcounter::get_mean()
{
    uint64_t sum = 0;
    std::vector<uint64_t>::iterator it;
    for (it=_cycles.begin(); it!=_cycles.end(); it++) {
        sum += *it;
    }
    _mean = sum / _cycles.size();
    return "    mean: " + to_string(_mean);
}
