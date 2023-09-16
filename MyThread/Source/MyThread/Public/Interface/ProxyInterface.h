// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleThreadType.h"

//TSharedFromThis<IThreadProxy> 可以将继承的类的实例对象转换为共享指针管理的对象
class MYTHREAD_API IThreadProxy : public TSharedFromThis<IThreadProxy>
{
public:
	IThreadProxy();
	virtual ~IThreadProxy(){}

	//安全创建线程
	virtual void CreateSafeThread() = 0;
	//唤醒线程
	virtual void WakeupThread() = 0;
	//线程是否挂起
	virtual bool IsSuspend() = 0;
	//等待其他线程任务完成，用于清除线程
	virtual void WaitAndCompleted() = 0;
	//阻塞主线程，用于同步操作
	virtual void BlockingAndCompletion() = 0;
	
	FORCEINLINE FSimpleDelegate& GetThreadDelegate(){return ThreadDelegate;}
	FORCEINLINE FThreadHandle GetThreadHandle(){return SimpleThreadHandle;}
	
protected:
	//单播代理
	FSimpleDelegate ThreadDelegate;

private:
	TSharedPtr<FSimpleThreadHandle> SimpleThreadHandle;
}; 