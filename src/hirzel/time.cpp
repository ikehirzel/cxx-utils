#include <hirzel/time.hpp>
#include <chrono>
#include <thread>

namespace hirzel::time
{
	u64 secondsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	u64 millisecondsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	u64 microsecondsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	void sleepForSeconds(u64 seconds)
	{
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
	}

	void sleepForMilliseconds(u64 milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void sleepForMicroseconds(u64 microseconds)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
	}
}
