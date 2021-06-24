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

		class BindException : public std::exception
		{
		private:
			std::string _msg;
			BindException(const std::string& msg) : _msg(msg) {}
		public:
			static BindException variable(const std::string& symbol, const std::string& error = "")
			{
				std::string msg = "failed to bind to variable: " + symbol;
				if (!error.empty()) msg += " : " + error;
				return BindException(msg);
			}

			static BindException function(const std::string& symbol, const std::string& error = "")
			{
				std::string msg = "failed to bind to function: " + symbol;
				if (!error.empty()) msg += " : " + error;
				return BindException(msg);
			}

			static BindException plugin(const std::string& filepath, const std::string& error = "")
			{
				std::string msg = "failed to bind to plugin: " + filepath;
				if (!error.empty()) msg += " : " + error;	
				return BindException(msg);
			}

			const char *what() const noexcept override { return _msg.c_str(); }
		};


		class DneException : public std::exception
		{
		private:
			std::string _msg;
			DneException(const std::string& msg) : _msg(msg) {}

		public:
			static DneException variable(const std::string& symbol)
			{
				return DneException("the requested variable does not exist: " + symbol);
			}

			static DneException function(const std::string& function)
			{
				return DneException("the requested function does not exist: " + function);
			}

			const char *what() const noexcept override { return _msg.c_str(); }
		};


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

	public:
		/**
		 * @brief	Default constructor
		 */
		Plugin() = default;
		inline Plugin(const std::string& filepath)
		{
			try
			{
				bind(filepath);
			}
			catch (const BindException& e)
			{
				throw;
			}
		}

		/**
		 * @brief	Frees all bound symbols
		 */
		~Plugin();

		/**
		 * @brief	Binds dynamic library
		 * @param	filepath	path to dynamic library
		 */
		void bind(const std::string& filepath);
		

		void bind_function(const std::string& label);
		void bind_functions(const std::initializer_list<std::string>& labels);

		void bind_variable(const std::string& label);
		void bind_variables(const std::initializer_list<std::string>& labels);
		

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
			T* ptr = (T*)get_variable_ptr(label);
			if (!ptr) throw DneException::variable(label);
			return *ptr;
		}


		template <typename T, typename ...Args>
		inline T execute(const std::string& label, Args... args) const 
		{
			T(*func)(Args...) = (T(*)(Args...))get_function(label);
			if (!func) throw DneException::function(label);
			return func(args...);
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
		inline const std::string& filepath() const
		{
			return _filepath;
		}
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


	void Plugin::bind(const std::string& filepath)
	{
		if(_handle) throw BindException::plugin(filepath, "plugin is already bound");
		if (filepath.empty()) throw BindException::plugin(filepath, "filepath was empty");

		#if OS_IS_WINDOWS
		_handle = (void*)LoadLibrary(filepath.c_str());
		#else
		_handle = dlopen(filepath.c_str(), RTLD_NOW);
		#endif

		if(!_handle) throw BindException::plugin(filepath, "plugin could not be found");

		_filepath = filepath;
	}

	void Plugin::bind_function(const std::string& label)
	{
		//guard against unloaded library
		if(!_handle) throw BindException::function(label, "plugin must be bound first");

		if (_symbols.find(label) != _symbols.end())
			throw BindException::function(label, "symbol is already bound");

		Symbol s;
		s.is_func = true;
		#if OS_IS_WINDOWS
		s.func = (Function)GetProcAddress((HMODULE)_handle, label.c_str());
		#else
		s.func = (Function)dlsym(_handle, label.c_str());
		#endif

		// assuring symbol bound
		if (!s.func) throw BindException::function("symbol could not be found");

		// storing symbol in table
		_symbols[label] = s;
	}

	void Plugin::bind_functions(const std::initializer_list<std::string>& labels)
	{
		for (const std::string& label : labels)
		{
			try
			{
				bind_function(label);
			}
			catch (const BindException& e)
			{
				throw;
			}
		}
	}


	void Plugin::bind_variable(const std::string& label)
	{
		//guard against unloaded library
		if(!_handle) throw BindException::variable(label, "plugin is not bound");

		if (_symbols.find(label) != _symbols.end())
			throw BindException::variable(label, "symbol is already bound");

		Symbol s;
		s.is_func = false;
		#if OS_IS_WINDOWS
		s.var = (Variable)GetProcAddress((HMODULE)_handle, label.c_str());
		#else
		s.var = (Variable)dlsym(_handle, label.c_str());
		#endif

		if (!s.var) throw BindException::variable(label, "symbol could not be found");

		_symbols[label] = s;
	}


	void Plugin::bind_variables(const std::initializer_list<std::string>& labels)
	{
		for (const std::string& label : labels)
		{
			try
			{
				bind_variable(label);
			}
			catch (const BindException& e)
			{
				throw;
			}
		}
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
