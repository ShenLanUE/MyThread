#pragma once

#include "CoreTypes.h"

#if PLATFORM_WINDOWS
struct MYTHREAD_API FWindowsPlatformThread
{
	//显示Windows线程
	static void Show();

	//隐藏Windows线程
	static void Hide();

	//判断Windows线程是否在运行
	static bool IsShow();

	static FSimpleDelegate CompletedDelegate;

	static FSimpleDelegate RunDelegate;
};
#endif