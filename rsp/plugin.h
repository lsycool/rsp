///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_PLUGIN_H
#define MYENGINE_PLUGIN_H

#ifndef MYENGINE_MYENGINE_H
#define MYENGINE_MYENGINE_H

#ifdef MYENGINE_EXPORTS
#define MYENGINE_API __declspec(dllexport)
#else
#define MYENGINE_API __declspec(dllimport)
#endif
#endif // MYENGINE_MYENGINE_H

#include "rsp.h"
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace MyEngine 
{
	/// Representation of a plugin
	class Plugin {
	public:
		/// Initialize and load plugin
		Plugin(const std::string &sFilename);
		/// Copy existing plugin instance
		Plugin(const Plugin &Other);
		/// Unload a plugin
		~Plugin();

	public:
		/// Query the plugin for its expected engine version
		int getEngineVersion() const {
			return m_pfnGetEngineVersion();
		}

		/// Register the plugin to a kernel
		void registerPlugin(CString ImgName) {
			m_pfnRegisterPlugin(ImgName);
		}

		CString getEngineName() {
			return pName();
		}

	private:
		/// Too lazy to this now...
		Plugin &operator =(const Plugin &Other);

		//插件名称
		typedef CString  name_t();
		/// Signature for the version query function
		typedef int  fnGetEngineVersion();
		/// Signature for the plugin's registration function
		typedef void fnRegisterPlugin(CString );//定义函数类型

		HMODULE             m_hDLL;                ///< Win32 DLL handle
		size_t             *m_pDLLRefCount;        ///< 插件数量
		name_t             *pName;
		fnGetEngineVersion *m_pfnGetEngineVersion; ///< 版本查询
		fnRegisterPlugin   *m_pfnRegisterPlugin;   ///< 插件注册
	};
} // namespace MyEngine
#endif // MYENGINE_PLUGIN_H