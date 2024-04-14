// Copyright Denis Faraci, Inc. All Rights Reserved.


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
	return CurrentSaveManager->GetSaveGameInstance();
}

void USSUtility::SaveAsManualSave()
{
	CurrentSaveManager->SaveAsManualSave();
}

void USSUtility::SaveOnSelectedSlot()
{
	CurrentSaveManager->SaveOnSelectedSlot();
}

void USSUtility::LoadFromSelectedSlot()
{
	CurrentSaveManager->LoadFromSelectedSlot();
}
