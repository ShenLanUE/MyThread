#include "Core/Manage/ResourceLoadingManage.h"

FResourceLoadingManage::FResourceLoadingManage()
	:Super()
	,StreamableManager(new FStreamableManager())
{
	
}

FResourceLoadingManage::~FResourceLoadingManage()
{
	delete this;
}

void FResourceLoadingManage::SetObjectPath(const TArray<FSoftObjectPath>& InObjectPathArray)
{
	SoftObjectPathArray = InObjectPathArray;
}

TArray<FSoftObjectPath>& FResourceLoadingManage::GetObjectPath()
{
	return SoftObjectPathArray;
}

FStreamableManager* FResourceLoadingManage::GetStreamableManager()
{
	return StreamableManager;
}
