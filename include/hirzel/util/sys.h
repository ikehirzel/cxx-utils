#ifndef HIRZEL_UTIL_SYS_H
#define HIRZEL_UTIL_SYS_H

#include <iostream>
#include <chrono>
#include <thread>

namespace hirzel
{
	namespace sys
	{
		/**
		 * @return	milliseconds since unix epoch
		 */
		inline long long epoch_millis()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::system_clock::now().time_since_epoch()).count();
		}


		/**
		 * @return	seconds since unix epoch
		 */
		inline long long epoch_seconds()
		{
			return std::chrono::duration_cast<std::chrono::seconds>
				(std::chrono::system_clock::now().time_since_epoch()).count();
		}


		/**
		 * Pauses thread until input is received from stdin
		 */
		inline void wait_for_key()
		{
			std::cout << "Press enter to continue... ";
			std::cin.get();
		}


		/**
		 * Pauses thread for specified time
		 * @param	millis	milliseconds to sleep for
		 */
		inline void sleep_millis(unsigned millis)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(millis));
		}


		/**
		 * Pauses thread for specified time
		 * @param	micros	microseconds to sleep for
		 */
		inline void sleep_micros(unsigned micros)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(micros));
		}
	}
}

#endif // HIRZEL_UTIL_SYS_H
