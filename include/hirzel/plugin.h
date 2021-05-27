/**
 * @file	plugin.h
 * @brief	Data structure that makes binding to dynamic objects easier
 * @author	Ike Hirzel
 * 
 * Copyright 2020 Ike Hirzel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef HIRZEL_PLUGIN_H
#define HIRZEL_PLUGIN_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace hirzel
{
	/**
	 * @brief	Typedef for basic void function pointer
	 */


	/**
	 * @brief	A Wrapper for binding to shared object plugins
	 * @details Binds to a shared object using OS's dylib and stores a map of functions
	 */
	class Plugin
	{
	public:
		typedef void* Symbol;

	private:
		// stores handle of library
		void* _handle = nullptr;
		// filepath of the shared object
		std::string _filepath;
		// Stores pointers to the functions
		std::unordered_map<std::string, Symbol> _symbols;



	public:
		/**
		 * @brief	Default constructor
		 */
		Plugin() = default;

		/**
		 * @brief	Frees all bound symbols
		 */
		~Plugin();

		/**
		 * @brief	Binds dynamic library
		 * @param	filepath	path to dynamic library
		 */
		const char *bind(const std::string& filepath);
		const char *bind_symbol(const std::string& symbol_name);
		
		/**
		 * @brief	Binds functions and adds pointer to it into map
		 * @param	funcname	symbolic name of function
		 * @return	pointer to function
		 */

		/**
		 * @param	funcname	symbolic name of function
		 * @return	true if symbol is bound, false if it is not
		 */
		inline bool contains(const std::string& symbol_name) const
		{
			return _symbols.find(symbol_name) != _symbols.end();
		}

		/**
		 * @return true if library and all symbols bound correctly, false if not
		 */
		inline bool is_bound() const { return _handle != nullptr; }

		/**
		 * @param	funcname	symbolic name of function
		 * @return	typeless pointer to function or nullptr if not bound
		 */
		inline Symbol get_symbol(const std::string& symbol_name) const
		{
			auto iter = _symbols.find(symbol_name);
			return (iter != _symbols.end()) ? iter->second : nullptr;
		}

		template <typename T>
		inline T get(const std::string& symbol_name) const 
		{
			T t = {0};
			Symbol s = get_symbol(symbol_name);
			return (s) ? *(T*)s: t;
		}

		template <typename T, typename ...Args>
		inline T execute(const std::string& symbol_name, Args... args) const 
		{
			T t = {0};
			T(*func)(Args...) = (T(*)(Args...))get_symbol(symbol_name);
			return (func) ? func(args...) : t;
		}


		/**
		 * @return	filepath of plugin
		 */
		inline const std::string& filepath() const { return _filepath; }
	};
}

#endif // HIRZEL_PLUGIN_H

#ifdef HIRZEL_IMPLEMENT
#undef HIRZEL_IMPLEMENT

#if defined(_WIN32) || defined(_WIN64)

#define OS_IS_WINDOWS true
#include <windows.h>

#elif defined(__unix__) || defined(linux)

#define OS_IS_WINDOWS false
#include <dlfcn.h>

#endif

namespace hirzel
{
	Plugin::~Plugin()
	{
		if(_handle)
		{
			#if OS_IS_WINDOWS
			FreeLibrary((HINSTANCE)_handle);
			#else
			dlclose(_handle);
			#endif
		}
	}

	const char *Plugin::bind(const std::string& filepath)
	{
		if(_handle) return "handle is already bound: overwriting is not allowed.";

		_filepath = filepath;

		if (_filepath.empty()) return "handle cannot be bound: filepath was empty";

		#if OS_IS_WINDOWS
		_handle = (void*)LoadLibrary(_filepath.c_str());
		#else
		_handle = dlopen(_filepath.c_str(), RTLD_NOW);
		#endif

		if(!_handle)
		{
			_filepath.clear();
			#if OS_IS_WINDOWS
			return GetLastError();
			#else
			return dlerror();
			#endif
		}

		return nullptr;
	}

	const char *Plugin::bind_symbol(const std::string& symbol_name)
	{
		//guard against unloaded library
		if(!_handle) return "functions cannot be bound before a handle is bound";

		//loading function from library
		#if OS_IS_WINDOWS
		Symbol symbol = (void*)GetProcAddress((HINSTANCE)_handle, funcname.c_str());
		#else
		Symbol symbol = (void*)dlsym(_handle, symbol_name.c_str());
		#endif

		// guard against unbound function
		if(!symbol)
		{
			#if OS_IS_WINDOWS
			return GetLastError();
			#else
			return dlerror();
			#endif
		}
		_symbols[symbol_name] = symbol;
		return nullptr;
	}
} // namespace hirzel

#endif // HIRZEL_IMPLEMENT
