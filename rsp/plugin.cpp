///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "plugin.h"
#include <stdexcept>

using namespace std;
using namespace MyEngine;

//@param  sFilename  Filename of the plugin to load
Plugin::Plugin(const std::string &sFilename) :m_hDLL(0),m_pfnGetEngineVersion(0),m_pfnRegisterPlugin(0),m_pDLLRefCount(0) {
	// Try to load the plugin as a dynamic library
	m_hDLL = ::LoadLibrary(sFilename.c_str());
	if(!m_hDLL)
		throw runtime_error(string("无法加载 '") + sFilename + "'");
	// Locate the plugin's exported functions  
	try 
	{
		pName = reinterpret_cast<name_t*>(::GetProcAddress(m_hDLL, "getEngineName"));//插件的名字
		m_pfnGetEngineVersion = reinterpret_cast<fnGetEngineVersion *>(::GetProcAddress(m_hDLL, "getEngineVersion"));//插件的版本
		m_pfnRegisterPlugin = reinterpret_cast<fnRegisterPlugin *>(::GetProcAddress(m_hDLL, "registerPlugin"));//插件主程序入口
		// If the functions aren't found, we're going to assume this is a plain simple DLL and not one of our plugins
		if(!m_pfnGetEngineVersion || ! m_pfnRegisterPlugin)
			throw runtime_error(string("'") + sFilename + "' 不是一个有效的插件");
		// Initialize a new DLL reference counter
		m_pDLLRefCount = new size_t(1);
	}
	catch(...) 
	{
		::FreeLibrary(m_hDLL);
		throw;
	}
} 



//@param  Other  Other plugin instance to copy
Plugin::Plugin(const Plugin &Other) :m_hDLL(Other.m_hDLL),pName(Other.pName),m_pfnGetEngineVersion(Other.m_pfnGetEngineVersion),m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin),m_pDLLRefCount(Other.m_pDLLRefCount) 
{
	/// Increase DLL reference counter
	++*m_pDLLRefCount;
}

/** Destroys the plugin, unloading its library when no more references to it exist.*/
Plugin::~Plugin() 
{
	// Only unload the DLL if there are no more references to it
	if(!--*m_pDLLRefCount)
	{
		delete m_pDLLRefCount;
		::FreeLibrary(m_hDLL);
	}
}
