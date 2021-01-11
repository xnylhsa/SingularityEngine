#include "Precompiled.h"
#include "AppInfoManaager.h"

using namespace SingularityEngine;
using namespace Core;
namespace
{
	AppInfoManager* sAppInfoManager = nullptr;
}

void AppInfoManager::StaticInitialize(std::wstring applicationRoot)
{

	sAppInfoManager = new AppInfoManager();
	sAppInfoManager->Initialize(applicationRoot);

}
AppInfoManager* AppInfoManager::Get()
{
	return sAppInfoManager;
}
void AppInfoManager::StaticTerminate()
{
	if (sAppInfoManager != nullptr)
	{
		sAppInfoManager->Terminate();
		delete sAppInfoManager;
		sAppInfoManager = nullptr;
	}
}