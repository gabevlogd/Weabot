// Copyright The Prototypers, Inc. All Rights Reserved.

#include "UGameFramework/GameInstanceBase.h"
#include "Framework/Application/NavigationConfig.h"

void UGameInstanceBase::Init()
{
	Super::Init();

	FNavigationConfig& NavigationConfig = *FSlateApplication::Get().GetNavigationConfig();
	NavigationConfig.bTabNavigation = false; // Disable tab navigation in widgets
}
