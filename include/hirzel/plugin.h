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
#include <vector>
#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64)
#define OS_IS_WINDOWS true
#include <libloaderapi.h>
#else
#define OS_IS_WINDOWS false
#include <dlfcn.h>
#endif

namespace hirzel
{
	class Plugin
	{
	public:

		typedef void(*Function)();
		typedef void *Variable;

		struct Symbol
		{
			bool is_func = false;
			union
			{
				Variable var = nullptr;
				Function func;
			};

			Symbol() = default;

			Symbol(Function function) :
			is_func(true),
			func(function)
			{}

			Symbol(Variable var) :
			is_func(false),
			var(var)
			{}
		};

	private:

		void* _handle = nullptr;
		std::string _filepath;
		std::unordered_map<std::string, Symbol> _symbols;

	private: // private functions

		inline void bind_handle(const std::string& filepath)
		{
			if (filepath.empty())
				throw std::invalid_argument("failed to bind to plugin: filepath was empty");

			#if OS_IS_WINDOWS
			_handle = (void*)LoadLibrary(filepath.c_str());
			#else
			_handle = dlopen(filepath.c_str(), RTLD_NOW);
			#endif

			if(!_handle)
				throw std::invalid_argument("failed to bind to plugin '"
					+ filepath
					+ "': plugin could not be found");

			_filepath = filepath;
		}

		inline void bind_function(const std::string& label)
		{
			if (_symbols.find(label) != _symbols.end())
				throw std::invalid_argument("failed to bind variable '"
					+ label
					+ "': symbol is already bound");

			#if OS_IS_WINDOWS
			Function ptr = (Function)GetProcAddress((HMODULE)_handle, label.c_str());
			// assuring symbol bound
			if (!ptr)
				throw std::invalid_argument("failed to bind function '"
					+ label
					+ "': symbol could not be found");
			#else
			Function ptr = (Function)dlsym(_handle, label.c_str());
			if (!ptr)
				throw std::invalid_argument("failed to bind function '"
					+ label
					+ "': "
					+ dlerror());
			#endif

			_symbols[label] = Symbol(ptr);
		}

		inline void bind_variable(const std::string& label)
		{
			if (_symbols.find(label) != _symbols.end())
				throw std::invalid_argument("failed to bind variable '"
					+ label
					+ "': symbol is already bound");

			#if OS_IS_WINDOWS
			Variable ptr = (Variable)GetProcAddress((HMODULE)_handle, label.c_str());
			if (!ptr)
				throw std::invalid_argument("failed to bind variable '"
					+ label
					+ "': symbol could not be found");
			#else
			Variable ptr = (Variable)dlsym(_handle, label.c_str());
			if (!ptr)
				throw std::invalid_argument("failed to bind variable '"
					+ label
					+ "': "
					+ dlerror());
			#endif

			_symbols[label] = Symbol(ptr);
		}

		inline void free()
		{
			if (_handle)
			{
				#if OS_IS_WINDOWS
				FreeLibrary((HINSTANCE)_handle);
				#else
				dlclose(_handle);
				#endif
			}
		}

	public:

		Plugin() = default;

		Plugin(const std::string& filepath, const std::vector<std::string>& functions = {},
			const std::vector<std::string>& variables = {})
		{
			bind_handle(filepath);

			try
			{
				for (const auto& label : functions)
					bind_function(label);

				for (const auto& label : variables)
					bind_variable(label);
			}
			catch (const std::exception&)
			{
				free();
				throw;
			}
		}

		Plugin(Plugin&& other)
		{
			*this = std::move(other);
		}

		Plugin(const Plugin&) = delete;

		~Plugin()
		{
			free();
		}

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

		inline Function get_function_ptr(const std::string& label) const
		{
			auto iter = _symbols.find(label);

			if (iter == _symbols.end())
				throw std::invalid_argument("failed to get function '"
					+ label
					+ "': symbol is not bound");

			if (!iter->second.is_func)
				throw std::invalid_argument("symbol '"
					+ label
					+ "' does not reference a function");

			return iter->second.func;
		}

		inline Variable get_variable_ptr(const std::string& label) const
		{
			auto iter = _symbols.find(label);

			if (iter == _symbols.end())
				throw std::invalid_argument("failed to get variable pointer '"
					+ label
					+ "': symbol is not bound");

			if (iter->second.is_func)
				throw std::invalid_argument("failed to get variable pointer '"
					+ label
					+ "': symbol does not reference a variable");

			return iter->second.var;
		}

		template <typename T>
		inline T get_variable(const std::string& label) const 
		{
			return *(T*)get_variable_ptr(label);
		}

		template <typename T, typename... Args>
		inline T call_function(const std::string& label, Args... args) const 
		{
			T(*func)(Args...) = (decltype(func))get_function_ptr(label);
			return func(args...);
		}		

		inline Plugin& operator=(const Plugin&) = delete;

		inline Plugin& operator=(Plugin&& other)
		{
			_handle = other._handle;
			_filepath = std::move(other._filepath);
			_symbols = std::move(other._symbols);

			other._handle = nullptr;

			return *this;
		}

		inline size_t count() const
		{
			return _symbols.size();
		}

		inline const std::string& filepath() const
		{
			return _filepath;
		}
	};
}

#endif // HIRZEL_PLUGIN_H
