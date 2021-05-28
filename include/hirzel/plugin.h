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
		typedef void(*Function)();
		typedef void *Variable;
	private:
		struct Symbol
		{
			bool is_func = false;
			union
			{
				Variable var = nullptr;
				Function func;
			};
		};

		// stores handle of library
		void* _handle = nullptr;
		// filepath of the shared object
		std::string _filepath;
		// Stores pointers to the functions
		std::unordered_map<std::string, Symbol> _symbols;

		const char *bind_symbol(const std::string& label, bool function);

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
		
		inline const char *bind_function(const std::string& label)
		{
			return bind_symbol(label, true);
		}

		inline const char *bind_variable(const std::string& label)
		{
			return bind_symbol(label, false);
		}

		/**
		 * @brief	Binds functions and adds pointer to it into map
		 * @param	funcname	symbolic name of function
		 * @return	pointer to function
		 */

		/**
		 * @param	funcname	symbolic name of function
		 * @return	true if symbol is bound, false if it is not
		 */
		inline bool contains(const std::string& label) const
		{
			return _symbols.find(label) != _symbols.end();
		}

		inline bool contains_function(const std::string& label) const
		{
			auto iter = _symbols.find(label);
			return iter != _symbols.end() && iter->second.is_func;
		}

		inline bool contains_variable(const std::string& label) const
		{
			auto iter = _symbols.find(label);
			return iter != _symbols.end() && !iter->second.is_func;
		}

		/**
		 * @return true if library and all symbols bound correctly, false if not
		 */
		inline bool is_bound() const { return _handle != nullptr; }


		inline Function get_function(const std::string& label) const
		{
			auto iter = _symbols.find(label);
			return (iter != _symbols.end() && iter->second.is_func) ? iter->second.func : nullptr;
		}


		inline Variable get_variable_ptr(const std::string& label) const
		{
			auto iter = _symbols.find(label);
			return (iter != _symbols.end() && !iter->second.is_func) ? iter->second.var : nullptr;
		}


		template <typename T>
		inline T get_variable_val(const std::string& label) const 
		{
			auto iter = _symbols.find(label);
			return (iter != _symbols.end() && !iter->second.is_func) ?
				*(T*)iter->second.var : T();
		}


		template <typename T, typename ...Args>
		inline T execute(const std::string& label, Args... args) const 
		{
			T(*func)(Args...) = (T(*)(Args...))get_function(label);
			return (func) ? func(args...) : T();
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

	const char *Plugin::bind_symbol(const std::string& label, bool function)
	{
		//guard against unloaded library
		if(!_handle) return "functions cannot be bound before a handle is bound";

		Symbol s;
		//loading function from library
		if (function)
		{
			#if OS_IS_WINDOWS
			s.func = (Function)GetProcAddress((HINSTANCE)_handle, funcname.c_str());
			if (!s.func) return GetLastError();
			#else
			s.func = (Function)dlsym(_handle, label.c_str());
			if (!s.func) return dlerror();
			#endif
			s.is_func = true;
		}
		else
		{
			#if OS_IS_WINDOWS
			s.var = (Variable)GetProcAddress((HINSTANCE)_handle, funcname.c_str());
			if (!s.var) return GetLastError();
			#else
			s.var = (Variable)dlsym(_handle, label.c_str());
			if (!s.var) return dlerror();
			#endif
			s.is_func = false;
		}

		_symbols[label] = s;
		return nullptr;
	}
} // namespace hirzel

#endif // HIRZEL_IMPLEMENT
