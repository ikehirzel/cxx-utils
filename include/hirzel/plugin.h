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

// standard library
#include <string>
#include <unordered_map>


#if defined(_WIN32) || defined(_WIN64)
#define OS_IS_WINDOWS true
#else
#define OS_IS_WINDOWS false
#endif

namespace hirzel
{
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
		std::string _error;

		// Stores pointers to the functions
		std::unordered_map<std::string, Symbol> _symbols;

		void get_system_error();

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
		bool bind(const std::string& filepath);
		

		bool bind_function(const std::string& label);
		bool bind_functions(const std::initializer_list<std::string>& labels);

		bool bind_variable(const std::string& label);
		bool bind_variables(const std::initializer_list<std::string>& labels);
		

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


		inline size_t count() const { return _symbols.size(); }
		size_t function_count() const;
		size_t variable_count() const;

		inline void clear_symbols()
		{
			_symbols.clear();
		}


		/**
		 * @return	filepath of plugin
		 */
		inline const std::string& filepath() const { return _filepath; }
		inline const std::string& error() const { return _error; }
	};
}

#endif // HIRZEL_PLUGIN_H

#ifdef HIRZEL_IMPLEMENT
#undef HIRZEL_IMPLEMENT

#if OS_IS_WINDOWS
#include <windows.h>
#else
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


	void Plugin::get_system_error()
	{
		#if OS_IS_WINDOWS
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer, 0, NULL);
    
		//Copy the error message into a std::string.
		_error = std::string(messageBuffer, size);
		//Free the Win32's string's buffer.
		LocalFree(messageBuffer);
		#else
		_error = dlerror();
		#endif
	}

	bool Plugin::bind(const std::string& filepath)
	{
		if(_handle)
		{
			_error = "handle is already bound: overwriting is not allowed.";
			return false;
		}

		_filepath = filepath;

		if (_filepath.empty())
		{
			_error = "handle cannot be bound: filepath was empty";
			return false;
		}

		#if OS_IS_WINDOWS
		std::wstring tmp(filepath.begin(), filepath.end());
		_handle = (void*)LoadLibrary(_filepath.c_str());
		#else
		_handle = dlopen(_filepath.c_str(), RTLD_NOW);
		#endif

		if(!_handle)
		{
			_filepath.clear();
			get_system_error();
			return false;
		}

		return true;
	}

	bool Plugin::bind_function(const std::string& label)
	{
		//guard against unloaded library
		if(!_handle)
		{
			_error = "functions cannot be bound before a handle is bound";
			return false;
		}

		if (_symbols.find(label) != _symbols.end())
		{
			_error = "a symbol is already bound with the same name";
			return false;
		}

		Symbol s;
		s.is_func = true;
		#if OS_IS_WINDOWS
		s.func = (Function)GetProcAddress((HMODULE)_handle, label.c_str());
		#else
		s.func = (Function)dlsym(_handle, label.c_str());
		#endif

		if (!s.func)
		{
			get_system_error();
			return false;
		}

		_symbols[label] = s;

		return true;
	}

	bool Plugin::bind_functions(const std::initializer_list<std::string>& labels)
	{
		for (const std::string& label : labels)
		{
			if (!bind_function(label)) return false;
		}
		return true;
	}


	bool Plugin::bind_variable(const std::string& label)
	{
		//guard against unloaded library
		if(!_handle)
		{
			_error = "variables cannot be bound before a handle is bound";
			return false;
		}

		if (_symbols.find(label) != _symbols.end())
		{
			_error = "a symbol is already bound with the same name";
			return false;
		}

		Symbol s;
		s.is_func = false;
		#if OS_IS_WINDOWS
		s.var = (Variable)GetProcAddress((HMODULE)_handle, label.c_str());
		#else
		s.var = (Variable)dlsym(_handle, label.c_str());
		#endif

		if (!s.var)
		{
			get_system_error();
			return false;
		}

		_symbols[label] = s;

		return true;
	}


	bool Plugin::bind_variables(const std::initializer_list<std::string>& labels)
	{
		for (const std::string& label : labels)
		{
			if (!bind_variable(label)) return false;
		}
		return true;
	}


	size_t Plugin::function_count() const
	{
		size_t count = 0;
		for (auto p : _symbols)
		{
			if (p.second.is_func) count += 1;
		}
		return count;
	}


	size_t Plugin::variable_count() const
	{
		size_t count = 0;
		for (auto p : _symbols)
		{
			if (!p.second.is_func) count += 1;
		}
		return count;
	}

} // namespace hirzel

#endif // HIRZEL_IMPLEMENT
