#include "cyclcounter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>

cyclcounter::cyclcounter() : _cycle(0), _stdev(0.0), _mean(0.0), _idx(0), _round(0), _stop(false)
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
	if(_idx < _BUFFER_SIZE_)
		_cycles[_idx++] = cyclcounter::end_rdtsc() - _cycle;
	else
		accum_result();
}

void
cyclcounter::addmany(int num) {
	uint64_t ttl = cyclcounter::end_rdtsc() - _cycle;
	uint64_t each = ttl / num;
	int i = 0;
	if(_stop) return;
	for (; i < num && _idx < _BUFFER_SIZE_; i++) {
		_cycles[_idx++] = each;
	}
	if(i < num) {
		accum_result();
		for(; i < num; i++)
			_cycles[_idx++] = each;
	}
}

void
cyclcounter::setfname(std::string str) {
	_fname = str;
}

void
cyclcounter::accum_result() {
    std::ofstream file(_fname, std::ofstream::out | std::ofstream::app);
    std::ofstream sfile(_fname+"summary", std::ofstream::out | std::ofstream::app);

	if(_round > 20) {
		_idx = 0;
		_stop = true;
		return;
	}
	_round++;
	// write all results to file
//	file.open(_fname, std::ofstream::out | std::ofstream::app);
	for(int i = 0; i < _BUFFER_SIZE_; i++)
		file << _cycles[i] << "\n";
	file.close();
	// put result in result vector
//	sfile.open(_fname+"summary", std::ofstream::out | std::ofstream::app);
	get_stdev();
	sfile << _mean << ", " << _stdev << "\n";
	sfile.close();

	// reset index
	_idx = 0;
}

string
cyclcounter::get_stdev()
{
    double dev = 0.0;
    get_mean();
    
    for (int i = 0; i < _BUFFER_SIZE_; i++) {
        dev += std::pow(_cycles[i] - _mean, 2);
    }
    _stdev = std::sqrt(dev/_BUFFER_SIZE_);
    return "stdev: " + to_string(_stdev);
}

string
cyclcounter::get_mean()
{
    uint64_t sum = 0;
    for(int i = 0; i < _BUFFER_SIZE_; i++)
        sum += _cycles[i];
    
    _mean = sum / _BUFFER_SIZE_;
    return "mean: " + to_string(_mean);
}
