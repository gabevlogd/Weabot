// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Utility/SVUtility.h"


USaveManager* USVUtility::CurrentSaveManager = nullptr;

void USVUtility::Init(USaveManager* SaveManager)
{
	CurrentSaveManager = SaveManager;
}

USaveManager* USVUtility::GetSaveManager()
{
	return CurrentSaveManager;
}

UDefaultSaveGame* USVUtility::GetSaveGame()
{
	return CurrentSaveManager->GetSaveGame();
}

void USVUtility::SaveAsNewSlot()
{
	CurrentSaveManager->SaveAsNewSlot();
}

void USVUtility::SaveOnSelectedSlot()
{
	CurrentSaveManager->SaveOnSelectedSlot();
}

void USVUtility::LoadFromSelectedSlot()
{
	CurrentSaveManager->LoadFromSelectedSlot();
}
