#pragma once
#include "CoreMinimal.h"
#include "Core/Manage/ThreadAbandonableManage.h"
#include "Core/Manage/ThreadProxyManage.h"
#include "Core/Manage/ThreadTaskManage.h"
#include "Core/Manage/CoroutinesManage.h"
#include "Core/Manage/ThreadGraphManage.h"
#include "Core/Manage/ResourceLoadingManage.h"
#include "Tickable.h"


/*管理线程*/
namespace NS_ThreadManage
{
	//为线程安全类，不容易出现死锁
	class MYTHREAD_API FThreadManagement : public TSharedFromThis<FThreadManagement>,public FTickableGameObject
	{
	public:
		static TSharedRef<FThreadManagement> Get();
		static void Destroy();
		
	private:
		virtual void Tick(float DeltaTime) override;
		virtual TStatId GetStatId() const override;
		
	public:
		static FThreadProxyManage& GetThreadProxyManage(){ return Get()->ThreadProxyManage;}
		static FThreadTaskManagement& GetThreadTaskManagement(){ return Get()->ThreadTaskManagement;}
		static FCoroutinesManage& GetCoroutinesManage(){ return Get()->CoroutinesManage;}
		static FThreadAbandonableManage& GetThreadAbandonableManage(){ return Get()->ThreadAbandonableManage; }
		static FThreadGraphManage& GetThreadGraphManage(){ return Get()->ThreadGraphManage; }
		static FResourceLoadingManage& GetResourceLoadingManage(){ return Get()->ResourceLoadingManage; }

	protected:
		//自定义线程创建,可以简单直接的创建线程
		FThreadProxyManage ThreadProxyManage;
		//自定义的线程池,可以往线程池内添加任务代理，令其执行
		FThreadTaskManagement ThreadTaskManagement;
		//可以设置前置任务的UE5线程池
		FThreadGraphManage ThreadGraphManage;
		
		//协程管理
		FCoroutinesManage CoroutinesManage;

		//从UE4线程池内直接取线程执行
		FThreadAbandonableManage ThreadAbandonableManage;

		//资源读取
		FResourceLoadingManage ResourceLoadingManage;
	
	private:
		static TSharedPtr<FThreadManagement> ThreadManagement;
		FThreadManagement(){}
		void operator=(const FThreadManagement& _ThreadManagement) const {}
	
	};
	
}
using namespace NS_ThreadManage;

typedef NS_ThreadManage::FThreadManagement GThread;

#if 0
void Example()
{
	TArray<FSoftObjectPath> ObjectPathArray;
	TSharedPtr<FStreamableHandle> Handle;
	auto La = [&Handle]()
	{
		TArray<UObject*> ExampleObjects;
		Handle->GetLoadedAssets(ExampleObjects);
	};
	//异步使用方法
	Handle = GThread::GetResourceLoadingManage() >> ObjectPathArray >> FSimpleDelegate::CreateLambda(La);

	//同步使用方法
	Handle = GThread::GetResourceLoadingManage() << ObjectPathArray;
	La();
	
}
#endif