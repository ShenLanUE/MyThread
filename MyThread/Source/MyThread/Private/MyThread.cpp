// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyThread.h"

#include "ThreadManage.h"

#define LOCTEXT_NAMESPACE "FMyThreadModule"

void FMyThreadModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMyThreadModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	GThread::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyThreadModule, MyThread)