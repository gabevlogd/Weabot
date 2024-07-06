// Copyright The Prototypers, Inc. All Rights Reserved.

#include "UGameFramework/GameInstanceBase.h"
#include "Framework/Application/NavigationConfig.h"

void UGameInstanceBase::Init()
{
	Super::Init();
	SetSlateNavigationConfigs();


}

void UGameInstanceBase::SetSaveMasterID(const FName NewSaveMasterID)
{
	SaveMasterID = NewSaveMasterID;
}

void UGameInstanceBase::SetSlateNavigationConfigs() const
{
	FNavigationConfig& NavigationConfig = *FSlateApplication::Get().GetNavigationConfig();
	NavigationConfig.bTabNavigation = bTabNavigation;
	NavigationConfig.bAnalogNavigation = bAnalogNavigation;
	NavigationConfig.bKeyNavigation = bKeyNavigation;
}
