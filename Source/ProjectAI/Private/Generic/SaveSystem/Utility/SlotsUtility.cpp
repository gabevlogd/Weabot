// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Utility/SlotsUtility.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager* USlotsUtility::CurrentSaveManager = nullptr;

void USlotsUtility::Init(USaveManager* SaveManager)
{
	if (!SaveManager) return;

	CurrentSaveManager = SaveManager;

	// if (!DoesAnySlotFileExist())
	// {
	// 	CreateIndexedSaveGameFile();
	// 	return;
	// }
}

bool USlotsUtility::DoesSlotFileExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SAVES_DIRECTORY + SlotName, 0);
}

bool USlotsUtility::DoesAnySlotFileExist()
{
	return GetTotalSlots() > 0;
}

bool USlotsUtility::GetSlotFilesNames(TArray<FString>& OutSaveFiles, const bool bWithExtension)
{
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return false;

	FileManager.FindFiles(OutSaveFiles, *SaveDirectory, TEXT(SAVE_EXTENSION));

	if (!bWithExtension)
	{
		for (FString& SlotName : OutSaveFiles)
			SlotName.RemoveFromEnd(SAVE_EXTENSION);
	}

	return OutSaveFiles.Num() > 0;
}

int32 USlotsUtility::GetTotalSlots()
{
	TArray<FString> SaveFiles;
	if (!GetSlotFilesNames(SaveFiles)) return 0;
	return SaveFiles.Num();
}

int32 USlotsUtility::GetTotalAutoSaveSlots()
{
	TArray<FString> SaveFiles;
	int32 AutoSaveSlotsNumber = 0;
	if (!GetSlotFilesNames(SaveFiles)) return 0;

	for (const FString& SlotName : SaveFiles)
	{
		if (SlotName.Contains(AUTO_SAVE_SLOT_NAME))
			AutoSaveSlotsNumber++;
	}

	return AutoSaveSlotsNumber;
}

int32 USlotsUtility::GetTotalManualSaveSlots()
{
	const int32 TotalSaves = GetTotalSlots();
	const int32 AutoSaveSlots = GetTotalAutoSaveSlots();
	return TotalSaves - AutoSaveSlots;
}

bool USlotsUtility::GetMostRecentSlotInfoData(FSlotInfoData& OutSlotData, const bool bIgnoreAutoSaves)
{
	TArray<UDefaultSaveGame*> SaveGames = CurrentSaveManager->GetAllSaveGames();

	if (bIgnoreAutoSaves)
	{
		SaveGames.RemoveAll([](const UDefaultSaveGame* SaveGame)
		{
			return SaveGame->SlotInfoData.SlotName.Contains(AUTO_SAVE_SLOT_NAME);
		});
	}
	
	if (SaveGames.Num() <= 0) return false; // Do the check after, so we don't have to do it twice
	
	OutSlotData = SaveGames[0]->SlotInfoData;
	return true;
}

bool USlotsUtility::GetMostAncientSlotInfoData(FSlotInfoData& OutSlotData, const bool bIgnoreAutoSaves)
{
	TArray<UDefaultSaveGame*> SaveGames = CurrentSaveManager->GetAllSaveGames();
	
	if (bIgnoreAutoSaves)
	{
		SaveGames.RemoveAll([](const UDefaultSaveGame* SaveGame)
		{
			return SaveGame->SlotInfoData.SlotName.Contains(AUTO_SAVE_SLOT_NAME);
		});
	}
	
	if (SaveGames.Num() <= 0) return false;

	OutSlotData = SaveGames[SaveGames.Num() - 1]->SlotInfoData;
	return true;
}

bool USlotsUtility::IsSlotNameValid(const FString& SlotName)
{
	return
		!SlotName.IsEmpty();
}
