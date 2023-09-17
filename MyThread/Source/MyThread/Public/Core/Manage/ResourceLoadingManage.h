#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "Core/Manage/ManageBase/ThreadManageBase.h"

//通过模板基类的Create 可以绑定异步结束后通知的事件
struct MYTHREAD_API FResourceLoadingManage :
	public FThreadTemplateBase<IStreamableContainer,TSharedPtr<struct FStreamableHandle>>
{
	typedef FThreadTemplateBase<IStreamableContainer,TSharedPtr<struct FStreamableHandle>> Super;

	FResourceLoadingManage();
	virtual ~FResourceLoadingManage() override;
	
protected:
	virtual void SetObjectPath(const TArray<FSoftObjectPath>& InObjectPathArray) override final;
	virtual TArray<FSoftObjectPath>& GetObjectPath() override final;
	virtual FStreamableManager* GetStreamableManager() override final;

private:
	TArray<FSoftObjectPath> SoftObjectPathArray;
	FStreamableManager* StreamableManager;
};
