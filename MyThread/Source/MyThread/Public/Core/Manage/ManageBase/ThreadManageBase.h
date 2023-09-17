#pragma once

#include "CoreMinimal.h"

//模板匹配，统一接口
template<class FTask,typename ReturnType = void>
class FThreadTemplateBase : public FTask
{
public:
	FThreadTemplateBase() : FTask(){}
#pragma region Create

	template<class UserClass,typename... VarTypes>
	ReturnType CreateRaw(UserClass* TargetClass,typename TMemFunPtrType<false,UserClass,void(VarTypes...)>::Type InMethod,VarTypes... Vars)
	{
		return *this >> FSimpleDelegate::CreateRaw(TargetClass,InMethod,Vars...);
	}

	template<class UserClass,typename... VarTypes>
	ReturnType CreateUObject(UserClass* TargetClass,typename TMemFunPtrType<false,UserClass,void(VarTypes...)>::Type InMethod,VarTypes... Vars)
	{
		return *this >> FSimpleDelegate::CreateUObject(TargetClass,InMethod,Vars...);
	}

	template<class UserClass,typename... VarTypes>
	ReturnType CreateSP(const TSharedRef<UserClass>& TargetClass,typename TMemFunPtrType<false,UserClass,void(VarTypes...)>::Type InMethod,VarTypes... Vars)
	{
		return *this >> FSimpleDelegate::CreateSP(TargetClass,InMethod,Vars...);
	}

	template<typename FunctorType,typename... VarTypes>
	ReturnType CreateLambda(FunctorType&& InMethod,VarTypes... Vars)
	{
		return *this >> FSimpleDelegate::CreateLambda(InMethod,Vars...);
	}

	template<class UserClass,typename... VarTypes>
	ReturnType CreateUFunction(UserClass* TargetClass,const FName& InMethod,VarTypes... Vars)
	{
		return *this >> FSimpleDelegate::CreateUFunction(TargetClass,InMethod,Vars...);
	}
 
#pragma endregion
	
};


template<class FTask,typename ReturnType = void>
class FThreadTemplate : public FThreadTemplateBase<FTask,ReturnType>
{
public:
	
	FThreadTemplate() : FThreadTemplateBase<FTask,ReturnType>(){}
	
#pragma region Bind
	
	template<class UserClass,typename... VarTypes>
	ReturnType BindRaw(UserClass* TargetClass,typename TMemFunPtrType<false,UserClass,void(VarTypes...)>::Type InMethod,VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateRaw(TargetClass,InMethod,Vars...);
	}

	template<class UserClass,typename... VarTypes>
	ReturnType BindUObject(UserClass* TargetClass,typename TMemFunPtrType<false,UserClass,void(VarTypes...)>::Type InMethod,VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateUObject(TargetClass,InMethod,Vars...);
	}

	template<class UserClass,typename... VarTypes>
	ReturnType BindSP(const TSharedRef<UserClass>& TargetClass,typename TMemFunPtrType<false,UserClass,void(VarTypes...)>::Type InMethod,VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateSP(TargetClass,InMethod,Vars...);
	}

	template<typename FunctorType,typename... VarTypes>
	ReturnType BindLambda(FunctorType&& InMethod,VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateLambda(InMethod,Vars...);
	}

	template<class UserClass,typename... VarTypes>
	ReturnType BindUFunction(UserClass* TargetClass,const FName& InMethod,VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateUFunction(TargetClass,InMethod,Vars...);
	}

#pragma endregion
	
};