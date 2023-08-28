// /**
//  * @file	plugin.hpp
//  * @brief	Value structure that makes binding to dynamic objects easier
//  * @author	Isaac Hirzel
//  * 
//  * Copyright 2020 Isaac Hirzel
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy of
//  * this software and associated documentation files (the "Software"), to deal in the
//  * Software without restriction, including without limitation the rights to use, copy,
//  * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  * and to permit persons to whom the Software is furnished to do so, subject to the
//  * following conditions:
//  * 
//  * The above copyright notice and this permission notice shall be included in all copies or
//  * substantial portions of the Software.
//  * 
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//  * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//  */

// #ifndef HIRZEL_PLUGIN_HPP
// #define HIRZEL_PLUGIN_HPP

// // standard library
// #include <string>
// #include <unordered_map>
// #include <vector>
// #include <stdexcept>
// #include <memory>

// #if defined(_WIN32) || defined(_WIN64)
// #define OS_IS_WINDOWS true
// #include <libloaderapi.hpp>
// #else
// #define OS_IS_WINDOWS false
// #include <dlfcn.h>
// #endif

// namespace hirzel
// {
// 	class Plugin
// 	{
// 	public: // sub-definitions

// 		typedef void(*Function)();
// 		typedef void *Variable;

// 	private: // sub-definitions

// 		struct Symbol
// 		{
// 			bool is_func = false;
// 			union
// 			{
// 				Variable var = nullptr;
// 				Function func;
// 			};

// 			Symbol() = default;

// 			Symbol(Function function) :
// 			is_func(true),
// 			func(function)
// 			{}

// 			Symbol(Variable var) :
// 			is_func(false),
// 			var(var)
// 			{}
// 		};

// 		struct Value
// 		{
// 			void *handle = nullptr;
// 			std::string filepath;
// 			std::unordered_map<std::string, Symbol> symbols;

// 			Value(void *handle, std::string filepath) :
// 			handle(handle),
// 			filepath(filepath)
// 			{}

// 			~Value()
// 			{
// 				if (handle)
// 				{
// 					#if OS_IS_WINDOWS
// 					FreeLibrary((HINSTANCE)_handle);
// 					#else
// 					dlclose(handle);
// 					#endif
// 				}
// 			}
// 		};

// 	private: // members

// 		static std::unordered_map<std::string, std::shared_ptr<Value>> _cache;

// 		std::shared_ptr<Value> _value;

// 	private: // methods

// 		std::shared_ptr<Value> get_Value(const std::string& filepath);	

// 	public: // methods

// 		Plugin(const std::string& filepath, const std::vector<std::string>& functions = {},
// 			const std::vector<std::string>& variables = {}) :
// 		_value(get_Value(filepath))
// 		{
// 			for (const auto& label : functions)
// 				bind_function(label);

// 			for (const auto& label : variables)
// 				bind_variable(label);
// 		}

// 		Plugin(Plugin&& other) :
// 		_value(std::move(other._value))
// 		{}

// 		Plugin(const Plugin& other) :
// 		_value(_cache[other.filepath()])
// 		{}

// 		void bind_function(const std::string& label);
// 		void bind_variable(const std::string& label);
		
// 		bool contains(const std::string& label) const
// 		{
// 			return _value->symbols.find(label) != _value->symbols.end();
// 		}

// 		bool contains_function(const std::string& label) const
// 		{
// 			auto iter = _value->symbols.find(label);

// 			return iter != _value->symbols.end() && iter->second.is_func;
// 		}

// 		bool contains_variable(const std::string& label) const
// 		{
// 			auto iter = _value->symbols.find(label);

// 			return iter != _value->symbols.end() && !iter->second.is_func;
// 		}

// 		Function get_function_ptr(const std::string& label) const;
// 		Variable get_variable_ptr(const std::string& label) const;	

// 		template <typename T>
// 		T get_variable(const std::string& label) const 
// 		{
// 			return *(T*)get_variable_ptr(label);
// 		}

// 		template <typename T, typename... Args>
// 		T call_function(const std::string& label, Args... args) const 
// 		{
// 			T(*func)(Args...) = (decltype(func))get_function_ptr(label);
// 			return func(args...);
// 		}		

// 		Plugin& operator=(const Plugin& other)
// 		{
// 			_value = _cache[other.filepath()];

// 			return *this;
// 		}

// 		Plugin& operator=(Plugin&& other)
// 		{
// 			_value = std::move(other._value);

// 			return *this;
// 		}

// 		std::shared_ptr<Plugin::Value> Value() const
// 		{
// 			return _value;
// 		}

// 		size_t count() const
// 		{
// 			return _value->symbols.size();
// 		}

// 		const std::string& filepath() const
// 		{
// 			return _value->filepath;
// 		}
// 	};
// }

// #endif // HIRZEL_PLUGIN_HPP

// #if !defined(HIRZEL_PLUGIN_I) && defined(HIRZEL_IMPLEMENT)
// #define HIRZEL_PLUGIN_I

// namespace hirzel
// {
// 	std::unordered_map<std::string, std::shared_ptr<Plugin::Value>> Plugin::_cache;

// 	std::shared_ptr<Plugin::Value> Plugin::get_Value(const std::string& filepath)
// 	{
// 		if (filepath.empty())
// 			throw std::invalid_argument("failed to bind to plugin: filepath was empty");

// 		auto iter = _cache.find(filepath);

// 		if (iter != _cache.end())
// 			return iter->second;

// 		#if OS_IS_WINDOWS
// 		auto handle = (void*)LoadLibrary(filepath.c_str());

// 		if (!handle)
// 			throw std::invalid_argument("failed to bind to plugin '"
// 				+ filepath
// 				+ "': ERROR "
// 				+ std::to_string(GetLastError()));
// 		#else
// 		auto handle = dlopen(filepath.c_str(), RTLD_NOW);

// 		if(!handle)
// 			throw std::invalid_argument("failed to bind to plugin '"
// 				+ filepath
// 				+ "': "
// 				+ std::string(dlerror()));
// 		#endif

// 		auto Value = std::make_shared<Value>(handle, filepath);

// 		_cache[filepath] = Value;

// 		return Value;
// 	}

// 	void Plugin::bind_function(const std::string& label)
// 	{
// 		if (_value->symbols.find(label) != _value->symbols.end())
// 			return;

// 		#if OS_IS_WINDOWS
// 		Function ptr = (Function)GetProcAddress((HMODULE)_handle, label.c_str());
// 		if (!ptr)
// 			throw std::invalid_argument("failed to bind variable '"
// 				+ label
// 				+ "': ERROR "
// 				+ std::to_string(GetLastError()));
// 		#else
// 		Function ptr = (Function)dlsym(_value->handle, label.c_str());
// 		if (!ptr)
// 			throw std::invalid_argument("failed to bind function '"
// 				+ label
// 				+ "': "
// 				+ std::string(dlerror()));
// 		#endif

// 		if (!ptr)
// 			throw std::invalid_argument("failed to bind function '"
// 				+ label
// 				+ "': symbol could not be found");

// 		_value->symbols[label] = Symbol(ptr);
// 	}

// 	void Plugin::bind_variable(const std::string& label)
// 	{
// 		if (_value->symbols.find(label) != _value->symbols.end())
// 			return;

// 		#if OS_IS_WINDOWS
// 		Variable ptr = (Variable)GetProcAddress((HMODULE)_handle, label.c_str());
// 		if (!ptr)
// 			throw std::invalid_argument("failed to bind variable '"
// 				+ label
// 				+ "': ERROR "
// 				+ std::to_string(GetLastError()));
// 		#else
// 		Variable ptr = (Variable)dlsym(_value->handle, label.c_str());
// 		if (!ptr)
// 			throw std::invalid_argument("failed to bind variable '"
// 				+ label
// 				+ "': "
// 				+ std::string(dlerror()));
// 		#endif


// 		_value->symbols[label] = Symbol(ptr);
// 	}

// 	Plugin::Variable Plugin::get_variable_ptr(const std::string& label) const
// 	{
// 		auto iter = _value->symbols.find(label);

// 		if (iter == _value->symbols.end())
// 			throw std::invalid_argument("failed to get variable pointer '"
// 				+ label
// 				+ "': symbol is not bound");

// 		if (iter->second.is_func)
// 			throw std::invalid_argument("failed to get variable pointer '"
// 				+ label
// 				+ "': symbol does not reference a variable");

// 		return iter->second.var;
// 	}

// 	Plugin::Function Plugin::get_function_ptr(const std::string& label) const
// 	{
// 		auto iter = _value->symbols.find(label);

// 		if (iter == _value->symbols.end())
// 			throw std::invalid_argument("failed to get function '"
// 				+ label
// 				+ "': symbol is not bound");

// 		if (!iter->second.is_func)
// 			throw std::invalid_argument("symbol '"
// 				+ label
// 				+ "' does not reference a function");

// 		return iter->second.func;
// 	}
// }

// #endif
