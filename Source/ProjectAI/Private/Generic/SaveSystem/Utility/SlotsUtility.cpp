// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Utility/SlotsUtility.h"

#include "Generic/SaveSystem/Constants/SaveConstants.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager* USlotsUtility::CurrentSaveManager = nullptr;

void USlotsUtility::Init(USaveManager* SaveManager)
{
	if (!SaveManager) return;

	CurrentSaveManager = SaveManager;
}

bool USlotsUtility::DoesSlotFileExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SAVES_DIRECTORY + SlotName, 0);
}

bool USlotsUtility::DoesAnySlotFileExist()
{
	return GetTotalSlots() > 0;
}

bool USlotsUtility::TryGetSlotFileNames(TArray<FString>& OutSaveFiles, const bool bWithExtension)
{
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return false;

	FileManager.FindFiles(OutSaveFiles, *SaveDirectory, SAVE_EXTENSION);

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
	if (!TryGetSlotFileNames(SaveFiles)) return 0;
	return SaveFiles.Num();
}

int32 USlotsUtility::GetTotalAutoSaveSlots()
{
	TArray<FString> SaveFiles;
	int32 AutoSaveSlotsNumber = 0;
	if (!TryGetSlotFileNames(SaveFiles)) return 0;

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

bool USlotsUtility::TryGetSlotInfosOfType(TArray<FSlotInfoData>& OutSlotInfos, ESaveTypeFilter Type)
{
    OutSlotInfos = CurrentSaveManager->GetSaveInfos();

    OutSlotInfos.RemoveAll([&](const FSlotInfoData& SlotInfo)
    {
        return (Type == ESaveTypeFilter::Manual && SlotInfo.SlotInfoName.Contains(AUTO_SAVE_SLOT_NAME)) ||
               (Type == ESaveTypeFilter::Auto && SlotInfo.SlotInfoName.Contains(SAVE_SLOT_NAME));
    });

    return OutSlotInfos.Num() > 0;
}

bool USlotsUtility::TryGetMostRecentSlotInfoData(FSlotInfoData& OutSlotData, const ESaveTypeFilter Type)
{
	TArray<FSlotInfoData> SaveInfos;

	if (!TryGetSlotInfosOfType(SaveInfos, Type)) return false;

	OutSlotData = SaveInfos[0];
	return true;
}

bool USlotsUtility::TryGetMostAncientSlotInfoData(FSlotInfoData& OutSlotData, const ESaveTypeFilter Type)
{
	TArray<FSlotInfoData> SaveInfos;

	if (!TryGetSlotInfosOfType(SaveInfos, Type)) return false;

	OutSlotData = SaveInfos[SaveInfos.Num() - 1];
	return true;
}

float USlotsUtility::GetMostRecentSlotInfoPlayedTime()
{
	FSlotInfoData SlotInfoData;
	if (!TryGetMostRecentSlotInfoData(SlotInfoData)) return 0.0f;
	return SlotInfoData.TimePlayed;
}

bool USlotsUtility::TryGetTimeSinceCreation(float& OutTimeSinceCreation)
{
	if (!GEngine) return false;
	if (!GEngine->GameViewport) return false;
	const UWorld* World = GEngine->GameViewport->GetWorld();
	if (!World) return false;
	if (UGameplayStatics::IsGamePaused(World)) return false; // If the game is paused, the time since creation is not updated
	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return false;

	OutTimeSinceCreation = PlayerController->GetGameTimeSinceCreation();
	return true;
}

bool USlotsUtility::IsSlotNameValid(const FString& SlotName)
{
	return
		!SlotName.IsEmpty();
}
