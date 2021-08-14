#ifndef HIRZEL_UTIL_SYS_H
#define HIRZEL_UTIL_SYS_H

#include <iostream>
#include <chrono>
#include <thread>

#define HIRZEL_WINDOWS	1
#define HIRZEL_LINUX	2
#define HIRZEL_APPLE	3
#define HIRZEL_FREEBSD	4
#define HIRZEL_ANDROID	5

#if defined(_WIN32) || defined(_WIN64)
	#define HIRZEL_OS HIRZEL_WINDOWS
	#include <libloaderapi.h>
#else
	#include <unistd.h>

	#if defined(__linux__)
		#define HIRZEL_OS HIRZEL_LINUX
	#elif defined(__APPLE__)
		#define HIRZEL_OS HIRZEL_APPLE
	#elif defined(__FreeBSD__)
		#define HIRZEL_OS HIRZEL_FREEBSD
	#elif defined(__ANDROID__)
		#define HIRZEL_OS HIRZEL_ANDROID
	#else
		#error Unsupported OS being used
	#endif

#endif

namespace hirzel
{
	namespace sys
	{
		/**
		 * @return	milliseconds since unix epoch
		 */
		inline long long epoch_millis() noexcept
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		inline constexpr bool os_is_unixlike()
		{
		#if HIRZEL_OS == HIRZEL_WINDOWS
			return false;
		#else 
			return true;
		#endif
		}

		inline constexpr const char *current_os()
		{
		#if HIRZEL_OS == HIRZEL_WINDOWS
			return "Windows";
		#elif HIRZEL_OS == HIRZEL_LINUX
			return "Linux";
		#elif HIRZEL_OS == HIRZEL_APPLE
			return "macOS";
		#elif HIRZEL_OS == HIRZEL_FREEBSD
			return "Free BSD";
		#elif HIRZEL_OS == HIRZEL_ANDROID
			return "Android";
		#endif
		}

		/**
		 * @return	seconds since unix epoch
		 */
		inline long long epoch_seconds() noexcept
		{
			return std::chrono::duration_cast<std::chrono::seconds>
				(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		inline std::string executable_path()
		{
		#if HIRZEL_OS == HIRZEL_WINDOWS

			char buf[MAX_PATH + 1];
			GetModuleFileNameA(NULL, buf, MAX_PATH);
			return std::string(buf);

		#elif HIRZEL_OS == HIRZEL_LINUX

			char buf[512];
			readlink("/proc/self/exe", buf, sizeof(buf) / sizeof(char) - 1);
			return std::string(buf);
			
		#else

			throw std::runtime_error("hirzel::sys::executable_path() is not implemented for "
				+ std::string(current_os()));

		#endif
		}

		inline std::string executable_dir()
		{
		#if HIRZEL_OS == HIRZEL_WINDOWS

			char path_buffer[MAX_PATH + 1];
			GetModuleFileNameA(NULL, path_buffer, MAX_PATH);

			char *end_of_str = path_buffer;

			while (*end_of_str)
				end_of_str += 1;

			while (*end_of_str != '\\')
				end_of_str -= 1;

		#elif HIRZEL_OS == HIRZEL_LINUX

			char path_buffer[512];
			readlink("/proc/self/exe", path_buffer, sizeof(path_buffer) / sizeof(char) - 1);
			char *end_of_str = path_buffer;

			while (*end_of_str)
				end_of_str += 1;

			while (*end_of_str != '/')
				end_of_str -= 1;
			
		#else

			throw std::runtime_error("hirzel::sys::executable_path() is not implemented for "
				+ std::string(current_os()));

		#endif

			*end_of_str = 0;
			return std::string(path_buffer);
		}

		/**
		 * Pauses thread until input is received from stdin
		 */
		inline void wait_for_key() noexcept
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
