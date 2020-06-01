#ifndef __ROBOTINFO_DEF_H__
#define __ROBOTINFO_DEF_H__

#include <boost/thread/mutex.hpp>
#include <thread>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unistd.h>
#include <vector>
#include <math.h>
#include <string>
#include "lm_sensorlist.h"

class RobotInfo {

public:
	static RobotInfo* GetInstance();

protected:
	RobotInfo();
	virtual ~RobotInfo();

private:
	static RobotInfo* m_pRi;
	static boost::mutex single_mutex;
	bool shutdown;
	std::thread scan_thread;
	lm_sensors::LMSensorList sensors;
};


#endif
