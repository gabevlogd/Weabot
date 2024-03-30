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

USaveGame* USVUtility::GetSaveGameData()
{
	return CurrentSaveManager->GetSaveGame();
}

void USVUtility::SaveGame()
{
	CurrentSaveManager->Save();
}

void USVUtility::LoadGame()
{
	CurrentSaveManager->Load();
}
