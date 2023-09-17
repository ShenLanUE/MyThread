#include "Core/Manage/ThreadProxyManage.h"

FThreadProxyManage::~FThreadProxyManage()
{
	for (const auto& Tmp : *this)
	{
		Tmp->WaitAndCompleted();
	}
	
	//this->Empty();
}

bool FThreadProxyManage::Join(FThreadHandle Handle)
{
	const TSharedPtr<IThreadProxy> IThreadProxy = *this >> Handle;

	if (IThreadProxy.IsValid())
	{
		IThreadProxy->BlockingAndCompletion();
		return true;
	}
	
	return false;
}

bool FThreadProxyManage::Detach(FThreadHandle Handle)
{
	const TSharedPtr<IThreadProxy> IThreadProxy = *this >> Handle;

	if (IThreadProxy.IsValid())
	{
		IThreadProxy->WakeupThread();
		return true;
	}
	
	return false;
}

EThreadState FThreadProxyManage::Joinable(FThreadHandle Handle)
{
	const TSharedPtr<IThreadProxy> IThreadProxy = *this >> Handle;
	
	if (IThreadProxy.IsValid())
	{
		if (IThreadProxy->IsSuspend())
		{
			return EThreadState::LEISURELY;
		}
		else
		{
			return  EThreadState::WORKING;
		}
	}
	return EThreadState::ERROR;
}
