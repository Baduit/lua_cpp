//
// UseLua.hpp for lua in /home/bertho_i/lua
// 
// Made by Lena BERTHO
// Login   <bertho_i@epitech.net>
// 
// Started on  Thu May 26 14:50:33 2016 Lena BERTHO
// Last update Thu Dec  8 12:48:29 2016 Lena BERTHO
//

#ifndef USE_LUA_HPP
#define USE_LUA_HPP

#include <string>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "liblua/lua.hpp"

class Lua
{
public:
  Lua(const std::string &file)
  {
    state = luaL_newstate();
    luaL_openlibs(state);
    if ((luaL_dofile(state, file.c_str())) != 0)
      throw std::runtime_error("Can't open the Lua script: " + file + ".");
  };

  virtual ~Lua(){};

  double	getNumber(const std::string &varName)
  {
    double	dest;

    lua_settop(state, 0);
    lua_getglobal(state, varName.c_str());
    if (lua_isnumber(state, 1))
      {
	dest = lua_tonumber(state, 1);
	lua_pop(state, 1);
      }
    else
      {
	lua_pop(state, 1);
	throw std::runtime_error(varName + " is not a number.");
      }
    return (dest);
  };

  std::string	getString(const std::string &varName)
  {
    std::string	dest;

    lua_settop(state,0);
    lua_getglobal(state, varName.c_str());
    if (lua_isstring(state, 1))
      {
	dest = lua_tostring(state, 1);
	lua_pop(state, 1);
      }
    else
      {
	lua_pop(state, 1);
	throw std::runtime_error(varName + " is not a string.");
      }
    return (dest);
  };

  typedef int	(*luaFct)(lua_State *);

  void	registerFunction(const std::string &fctName, luaFct fctPtr)
  {
    lua_register(state, fctName.c_str(), fctPtr);
  }

  // bug part about calling a function
  template <typename T, typename ... Args>
  T	callFunction(const std::string &fctName,	\
		     const Args& ... a)
   {
    T	res;

    lua_getglobal(state, fctName.c_str());
    if (lua_isfunction(state, -1))
      {
	pushArgs(0, 0, sizeof ... (Args), a ...);
	lua_call(state, sizeof ... (Args), 1);
	res = lua_tonumber(state, -1);
	return (res);
      }
    else
      {
	lua_pop(state, 1);
	throw std::runtime_error(fctName + " is not a function.");
      }
    return (res);
  };

  template <typename ... Args>
  int	callFunction(const std::string &fctName, int &res,	\
		     const Args& ... a)
  {
    lua_getglobal(state, fctName.c_str());
    if (lua_isfunction(state, -1))
      {
	pushArgs(0, 0, sizeof ... (Args), a ...);
	lua_call(state, sizeof ... (Args), 1);
	res = lua_tonumber(state, -1);
	return (res);
      }
    else
      {
	lua_pop(state, 1);
	throw std::runtime_error(fctName + " is not a function.");
      }
    return (res);
  };

  template <typename ... Args>
  std::string	callFunction(const std::string &fctName,		\
			     std::string &res, const Args& ... a)
  {
    lua_getglobal(state, fctName.c_str());
    if (lua_isfunction(state, -1))
      {
	pushArgs(0, 0, sizeof ... (Args) , a ...);
	lua_call(state, sizeof ... (Args), 1);
	const char *s = lua_tostring(state, -1);
	res = (s) ? s: "";
	return (res);
      }
    else
      {
	lua_pop(state, 1);
	throw std::runtime_error(fctName + " is not a function.");
      }
    return (res);
  };

  template <typename ... Args>
  void	callFunction(const std::string &fctName, const Args& ... a)
  {
    lua_getglobal(state, fctName.c_str());
    if (lua_isfunction(state, -1))
      {
	pushArgs(0, 0, sizeof ... (Args), a ...);
	lua_call(state, sizeof ... (Args), 0);
      }
    else
      {
	lua_pop(state, 1);
	throw std::runtime_error(fctName + " is not a function.");
      }
  };

private:
  // for int arguments
  void	pushArgs(int begin, int i, int n,	\
		 int value)
  {
    if (i >= n + begin)
      return ;
    if (i >= begin)
      lua_pushnumber(state, value);
  }

  template <typename ... Args>
  void	pushArgs(int begin, int i, int n,		\
		 int value, const Args& ... args)
  {
    if (i >= n + begin)
      return ;
    if (i >= begin)
      lua_pushnumber(state, value);
    pushArgs(begin, i + 1, n, args ...);
  }

  // for float/double arguments
  void	pushArgs(int begin, int i, int n,	\
		 double value)
  {
    if (i >= n + begin)
      return ;
    if (i >= begin)
      lua_pushnumber(state, value);
  }

  template <typename ... Args>
  void	pushArgs(int begin, int i, int n,		\
		 double value, const Args& ... args)
  {
    if (i >= n + begin)
      return ;
    if (i >= begin)
      lua_pushnumber(state, value);
    pushArgs(begin, i + 1, n, args ...);
  }

  // for string arguments
  void	pushArgs(int begin, int i, int n,	\
		 const std::string &value)
  {
    if (i >= n + begin)
      return ;
    if (i >= begin)
      lua_pushstring(state, value.c_str());
  }

  template <typename ... Args>
  void	pushArgs(int begin, int i, int n,				\
		 const std::string &value, const Args& ... args)
  {
    if (i >= n + begin)
      return ;
    if (i >= begin)
      lua_pushstring(state, value.c_str());
    pushArgs(begin, i + 1, n, args ...);
  }

  // without arguments
  void	pushArgs(__attribute__((unused)) int begin,
		 __attribute__((unused)) int i,
		 __attribute__((unused)) int n)
  {};

private:
  lua_State	*state;
};

#endif
