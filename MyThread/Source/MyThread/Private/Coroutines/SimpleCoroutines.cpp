#include "Coroutines/SimpleCoroutines.h"


TArray<TSharedPtr<ICoroutinesObject>> ICoroutinesObject::Array;

FCoroutinesRequest::FCoroutinesRequest(float _InIntervalTime)
	:bCompleteRequest(false)
	,IntervalTime(_InIntervalTime)
{
	
}

ICoroutinesObject::ICoroutinesObject()
	:bAwaken(false)
{
	
}

FCoroutinesObject::FCoroutinesObject(const FSimpleDelegate& InSimpleDelegate)
	:ICoroutinesObject()
	,SimpleDelegate(InSimpleDelegate)
	,TotalTime(INDEX_NONE)
	,RunningTime(0.f)
{
	
}

FCoroutinesObject::FCoroutinesObject(float InTotalTime, const FSimpleDelegate& InSimpleDelegate)
	:ICoroutinesObject()
	,SimpleDelegate(InSimpleDelegate)
	,TotalTime(InTotalTime)
	,RunningTime(0.f)
{
	
}

void FCoroutinesObject::Update(FCoroutinesRequest& CoroutinesRequest)
{
	if (TotalTime != INDEX_NONE)
	{
		RunningTime += CoroutinesRequest.IntervalTime;
		if (RunningTime >= TotalTime)
		{
			if (SimpleDelegate.IsBound())
			{
				SimpleDelegate.Execute();
				CoroutinesRequest.bCompleteRequest = true;
			}
		}
	}
	else
	{
		if (bAwaken)
		{
			if (SimpleDelegate.IsBound())
			{
				SimpleDelegate.Execute();
				CoroutinesRequest.bCompleteRequest = true;
			}
		}
	}
}
