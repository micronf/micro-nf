#ifndef _CYCLCOUNTER_HH
#define _CYCLCOUNTER_HH
#include <string>
#include <vector>
//#include <linux/hardirq.h>
//#include <linux/preempt.h>
#define _BUFFER_SIZE_ 1000000 

using namespace std;

class cyclcounter { public:
    cyclcounter();
    ~cyclcounter();

    void update();  // update local cycle
    void addone();  // push back cycle
    void addmany(int );

	void accum_result();


    string get_stdev();
    string get_mean();

	void setfname(string str);

    __inline__ uint64_t start_rdtsc() {
         unsigned int lo,hi;

         //preempt_disable();
         //raw_local_irq_save(_flags);

         __asm__ __volatile__ ("CPUID\n\t"
                               "RDTSC\n\t"
                               "mov %%edx, %0\n\t"
                               "mov %%eax, %1\n\t": "=r" (hi), "=r" (lo):: "%rax", "%rbx", "%rcx", "%rdx");
         return ((uint64_t)hi << 32) | lo;
    }    

    __inline__ uint64_t end_rdtsc() {
        unsigned int lo, hi;
        
        __asm__ __volatile__ ("RDTSCP\n\t"
                              "mov %%edx, %0\n\t"
                              "mov %%eax, %1\n\t"
                              "CPUID\n\t": "=r" (hi), "=r" (lo):: "%rax", "%rbx", "%rcx", "%rdx");
        //raw_local_irq_save(_flags);
        //preempt_enable();
        return ((uint64_t)hi << 32) | lo;
    }

    private:
    uint32_t _idx;
    uint32_t _round;
    uint64_t _cycle;
    double _stdev, _mean;
    bool _stop;
    uint64_t _cycles[_BUFFER_SIZE_];
    std::vector<string> _sumary;
    unsigned long _flags;
	std::string _fname;

};

#endif
