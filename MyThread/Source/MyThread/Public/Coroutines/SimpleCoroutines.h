#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleThreadType.h"

//协程请求结构体
struct MYTHREAD_API FCoroutinesRequest
{
	FCoroutinesRequest(float _InIntervalTime);

	//是否已完成请求
	bool bCompleteRequest;
	//执行任务的时间间隔
	float IntervalTime;
};

//协程对象接口
class MYTHREAD_API ICoroutinesObject : public TSharedFromThis<ICoroutinesObject>
{
	friend class ICoroutinesContainer;

public:
	ICoroutinesObject();
	virtual ~ICoroutinesObject(){}

	bool operator==(const ICoroutinesObject& _ICoroutinesObject)
	{
		return this->Handle == _ICoroutinesObject.Handle;
	}

	FORCEINLINE void Awaken() { bAwaken = true; }

protected:
	
	virtual void Update(FCoroutinesRequest& CoroutinesRequest) = 0;
	
	//用于协程绑定后的静态注册
	static TArray<TSharedPtr<ICoroutinesObject>> Array;
	uint8 bAwaken : 1; //唤醒当前的协程
	FSimpleThreadHandle Handle;
};

typedef TWeakPtr<ICoroutinesObject> FCoroutinesHandle;

//协程对象
class MYTHREAD_API FCoroutinesObject : public ICoroutinesObject
{
public:
	FCoroutinesObject(const FSimpleDelegate &InSimpleDelegate);
	FCoroutinesObject(float InTotalTime,const FSimpleDelegate& InSimpleDelegate);
protected:
	virtual void Update(FCoroutinesRequest& CoroutinesRequest) override final; //final代表当前函数不能被下一级子类重写

private:
	FSimpleDelegate SimpleDelegate;
	const float TotalTime;
	float RunningTime;
};