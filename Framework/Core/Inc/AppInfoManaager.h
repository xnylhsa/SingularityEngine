
#ifndef INCLUDED_APPLICATION_INFORMATION_MANAGER_H
#define INCLUDED_APPLICATION_INFORMATION_MANAGER_H
#include "Common.h"
#include "StringCasting.h"

namespace SingularityEngine {

	namespace Core {

		class AppInfoManager
		{
		public:
			static void StaticInitialize(std::wstring applicationRoot);
			static void StaticTerminate();
			static AppInfoManager* Get();
			const std::wstring GetAppRootFolder(bool useForwardSlash = true) const 
			{
				std::wstring rootPath = appRootDirectory;
				if (!useForwardSlash)
				{
					for (wchar_t& character : rootPath)
					{
						if (character == '\\')
						{
							character = '/';
						}
					}
					//rootPath.replace(rootPath.begin(), rootPath.end(),"\\", L"/");
				}
				return rootPath;
			}
			const std::string GetAppRootFolderStr(bool useForwardSlash = true) const
			{
				std::wstring rootPath = appRootDirectory;

				if (!useForwardSlash)
				{
					for (wchar_t& character : rootPath)
					{
						if (character == '\\')
						{
							character = '/';
						}
					}
					//rootPath.replace(rootPath.begin(), rootPath.end(),"\\", L"/");
				}
				std::string appRootString = string_cast<std::string, std::wstring>(rootPath);
				return appRootString;
			}

		private:
			AppInfoManager() {}
			~AppInfoManager() {}
			void Initialize(std::wstring applicationRoot) { appRootDirectory = applicationRoot; }
			void Terminate() {}
			std::wstring appRootDirectory;
		};


	}
}
#endif