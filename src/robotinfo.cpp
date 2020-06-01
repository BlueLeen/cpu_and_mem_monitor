#include "robotinfo.h"
#include <chrono>
#include <vector>
#include <boost/format.hpp>
#include "cpu.h"
#include "memory.h"

#define 	NODENAME			"Monitor(Robot Info)"

RobotInfo* RobotInfo::m_pRi = NULL;
boost::mutex RobotInfo::single_mutex;

RobotInfo* RobotInfo::GetInstance()
{
	{
		boost::mutex::scoped_lock lock(single_mutex);
		if(!m_pRi)
		{
			m_pRi = new RobotInfo;
		}
	}
	return m_pRi;
}

RobotInfo::RobotInfo():shutdown(false)
{
	cpuStats.Init();
	scan_thread = std::thread([&]{
		int64_t ticks_last = 0;
		while(!shutdown)
		{
			sensors.update();
			cpuStats.UpdateCPUData();
			cpuStats.UpdateCoreMhz();
			cpuStats.UpdateCpuTemp();
			cpuStats.GetCpuFile();
			update_meminfo();
			const std::vector<CPUData>& cpu_vec = cpuStats.GetCPUData();
			const CPUData& cpu_inf = cpuStats.GetCPUDataTotal();
			bool immediate_pub = false;
			//if(cpu_inf.temp > 75 || cpu_inf.percent > 80 || memused > 20.0)
			{
				for(auto it=sensors.begin(); it != sensors.end(); ++it)
					std::cout << (*it)->label() << " temperature-" << (*it)->to_string() << std::endl;
				for(decltype(cpu_vec.size()) i = 0; i < cpu_vec.size(); ++i)
				{
					std::cout<< "cpu thread[" << (i+1) << "] "<< "percent- " << cpu_vec[i].percent << std::endl;
				}
				std::cout<< "cpu[total] temperature- " << cpu_inf.temp << "°C,"
						<< "percent- " << cpu_inf.percent << std::endl;
				std::cout<< "memory max- " << memmax << "G"
						<< ",memory used- " << memused << "G" << std::endl;
				immediate_pub = true;
			}
	    	std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
	    	int64_t ticks =  std::chrono::duration_cast<std::chrono::seconds>(p.time_since_epoch()).count();
	    	if((ticks-ticks_last >= 20) || immediate_pub)
	    	{
				boost::format fmt = boost::format("cpu temperature:%d°C percent:%f, memory used:%fG")
									% cpu_inf.temp % cpu_inf.percent % memused;
				printf("%s\n", fmt.str().c_str());
				ticks_last = ticks;
				
	    	}
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
	});
	scan_thread.join();
}

RobotInfo::~RobotInfo()
{
	shutdown = true;
}
