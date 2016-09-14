///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////

#ifndef MYENGINE_KERNEL_H
#define MYENGINE_KERNEL_H

#include "plugin.h"
#include <string>
#include <map>

namespace MyEngine {

	/// The engine's system core
	class Kernel {
	public:
		/// Loads a plugin
		void loadPlugin(const std::string &sFilename) 
		{
			if(m_LoadedPlugins.find(sFilename) == m_LoadedPlugins.end())//�жϵ�ǰ����Ƿ��Ѿ����أ����δ��������뵽��ǰ���������
				m_LoadedPlugins.insert(PluginMap::value_type(sFilename,Plugin(sFilename)));/*.first->second.registerPlugin(NULL);*/
		}
		//CString getName()
		//{
		//	for (std::map<std::string, Plugin>::iterator it=m_LoadedPlugins.begin() ; it != m_LoadedPlugins.end(); it++ )
		//		return (*it).second.getEngineName();

		//}
		//int getVersion()
		//{
		//	for (std::map<std::string, Plugin>::iterator it=m_LoadedPlugins.begin() ; it != m_LoadedPlugins.end(); it++ )
		//		return (*it).second.getEngineVersion();
		//}
	public:
		typedef std::map<std::string, Plugin> PluginMap;//������ļ����������ӳ��   
		PluginMap      m_LoadedPlugins;  ///< All plugins currently loaded����ǰ�Ѽ��ص����в��
	};
} // namespace MyEngine

#endif // MYENGINE_KERNEL_H