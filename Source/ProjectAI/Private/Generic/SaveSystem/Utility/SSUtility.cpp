// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Generic/SaveSystem/Utility/SSUtility.h"

USaveManager* USSUtility::CurrentSaveManager = nullptr;

void USSUtility::Init(USaveManager* SaveManager)
{
	CurrentSaveManager = SaveManager;
}

USaveManager* USSUtility::GetSaveManager()
{
	return CurrentSaveManager;
}

UDefaultSaveGame* USSUtility::GetSaveGame()
{
	if (!Check()) return nullptr;
	return CurrentSaveManager->GetSaveGameInstance();
}

void USSUtility::StartNewSaveGame()
{
	if (!Check()) return;
	CurrentSaveManager->StartNewSaveGame();
}

void USSUtility::ManualSave()
{
	if (!CurrentSaveManager) return;
	CurrentSaveManager->ManualSave();
}

bool USSUtility::Check()
{
	return CurrentSaveManager != nullptr;
}
