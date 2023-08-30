#include <hirzel/time.hpp>
#include <chrono>
#include <thread>

namespace hirzel::time
{
	using namespace hirzel::primitives;

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
		std::thread::this_thread::sleep_for(std::chrono::seconds(seconds));
	}

	void sleepForMilliseconds(u64 milliseconds)
	{
		std::thread::this_thread::sleep_for(std::chrono::milliseconds(seconds));
	}

	void sleepForMicroseconds(u64 microseconds)
	{
		std::thread::this_thread::sleep_for(std::chrono::microseconds(seconds));
	}
}
