#include "Core/Manage/CoroutinesManage.h"

FCoroutinesManage::FCoroutinesManage()
	: FThreadTemplateBase<ICoroutinesContainer,FCoroutinesHandle>()
{
	
}

void FCoroutinesManage::Tick(float DeltaTime)
{
	*this <<= DeltaTime;
}