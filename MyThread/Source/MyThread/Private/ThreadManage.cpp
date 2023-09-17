#include "ThreadManage.h"


TSharedPtr<FThreadManagement> FThreadManagement::ThreadManagement = nullptr;
/*
FThreadProxyManage FThreadManagement::ThreadProxyManage;
FThreadTaskManagement FThreadManagement::ThreadTaskManagement;
FThreadAbandonableManage FThreadManagement::ThreadAbandonableManage;
FCoroutinesManage FThreadManagement::CoroutinesManage;
FThreadGraphManage FThreadManagement::ThreadGraphManage;
FResourceLoadingManage FThreadManagement::ResourceLoadingManage;*/

TSharedRef<FThreadManagement> FThreadManagement::Get()
{
	if (!ThreadManagement.IsValid())
	{
		ThreadManagement = MakeShareable(new FThreadManagement);
	}
	return ThreadManagement.ToSharedRef();
}

void FThreadManagement::Destroy()
{
	if (ThreadManagement.IsValid())
	{
		ThreadManagement = nullptr;
	}
}

void FThreadManagement::Tick(float DeltaTime)
{
	//线程任务队列
	ThreadTaskManagement.Tick(DeltaTime);
	//协程管理
	CoroutinesManage.Tick(DeltaTime);
}

TStatId FThreadManagement::GetStatId() const
{
	return TStatId();
}


