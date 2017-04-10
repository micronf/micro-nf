#ifndef _MSSTATS_H_
#define _MSSTATS_H_

#define MAX_NUM_MS 100
#define MAX_NUM_PORT 50

#define MZ_STAT "MZ_STAT"

class MSStats {
	public:
		int num_nf;
		unsigned int packet_drop[MAX_NUM_MS][MAX_NUM_PORT];

};

#endif // _MSSTATS_H_
