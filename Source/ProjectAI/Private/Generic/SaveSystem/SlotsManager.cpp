// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SlotsManager.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


FString USlotsManager::SelectedSaveSlotName = "";
USaveManager* USlotsManager::CurrentSaveManager = nullptr;

void USlotsManager::Init(USaveManager* SaveManager)
{
	if (!SaveManager) return;

	CurrentSaveManager = SaveManager;

	// if (!DoesAnySlotFileExist())
	// {
	// 	CreateSlotFile();
	// 	return;
	// }
}

bool USlotsManager::SelectSaveSlot(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
	SelectedSaveSlotName = SlotName;
	return true;
}

FString USlotsManager::GetSelectedSaveSlotName()
{
	return SelectedSaveSlotName;
}

bool USlotsManager::DeleteSlotFile(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
	SelectMostRecentSaveSlot(); // Select the most recent slot after deleting the current one to avoid invalid selection
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USlotsManager::DeleteAllSlotFiles()
{
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*SaveDirectory, false, true);
}

bool USlotsManager::DoesSlotFileExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SAVES_DIRECTORY + SlotName, 0);
}

bool USlotsManager::DoesAnySlotFileExist()
{
	int32 AutoSaveSlots;
	int32 SaveSlots;
	return GetNumberOfSlots(AutoSaveSlots, SaveSlots) > 0;
}

TArray<FSaveSlotData> USlotsManager::GetAllSlotsData()
{
	TArray<FSaveSlotData> Slots;
	TArray<FString> SlotNames;
	USlotsManager::GetSlotFilesNames(SlotNames);

	for (const FString& SaveGameName : SlotNames)
	{
		if (const UDefaultSaveGame* DefaultSaveGame = Cast<UDefaultSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveGameName, 0)))
			Slots.Add(DefaultSaveGame->SaveSlotData);
	}

	return Slots;
}

bool USlotsManager::GetSlotFilesNames(TArray<FString>& OutSaveFiles, bool bHasExtension)
{
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return false;

	FileManager.FindFiles(OutSaveFiles, *SaveDirectory, TEXT(SAVE_EXTENSION));

	if (!bHasExtension)
	{
		for (FString& SlotName : OutSaveFiles)
			SlotName.RemoveFromEnd(SAVE_EXTENSION);
	}

	return OutSaveFiles.Num() > 0;
}

int32 USlotsManager::GetNumberOfSlots(int32& OutAutoSavesSlotsNumber, int32& OutManualSaveSlotsNumber)
{
	TArray<FString> SaveFiles;
	OutManualSaveSlotsNumber = 0;
	OutAutoSavesSlotsNumber = 0;
	if (!GetSlotFilesNames(SaveFiles)) return 0;

	for (const FString& SlotName : SaveFiles)
	{
		if (SlotName.Contains(AUTO_SAVE_SLOT_NAME))
			OutAutoSavesSlotsNumber++;
	}

	OutManualSaveSlotsNumber = SaveFiles.Num() - OutAutoSavesSlotsNumber;
	return SaveFiles.Num();
}

void USlotsManager::SelectMostRecentSaveSlot()
{
	FSaveSlotData SlotData;
	GetMostRecentSlotData(SlotData);
	SelectSaveSlot(SlotData.SlotName);
}

void USlotsManager::SelectMostAncientSaveSlot()
{
	FSaveSlotData SlotData;
	GetMostAncientSlotData(SlotData);
	SelectSaveSlot(SlotData.SlotName);
}

bool USlotsManager::GetMostRecentSlotData(FSaveSlotData& OutSlotData)
{
	const TArray<UDefaultSaveGame*> SaveGames = CurrentSaveManager->GetAllSaveGames();
	if (SaveGames.Num() <= 0) return false;

	OutSlotData = SaveGames[0]->SaveSlotData;
	return true;
}

bool USlotsManager::GetMostAncientSlotData(FSaveSlotData& OutSlotData, bool bIsAutoSave)
{
	TArray<UDefaultSaveGame*> SaveGames = CurrentSaveManager->GetAllSaveGames();

	if (bIsAutoSave)
	{
		SaveGames.RemoveAll([](const UDefaultSaveGame* SaveGame)
		{
			return !SaveGame->SaveSlotData.SlotName.Contains(AUTO_SAVE_SLOT_NAME);
		});
	}
	
	if (SaveGames.Num() <= 0) return false;

	OutSlotData = SaveGames[SaveGames.Num() - 1]->SaveSlotData;
	return true;
}

bool USlotsManager::CreateSlotFileOfName(const FString& SlotName, const bool bSelectCreatedSlot)
{
	if (!IsSlotNameValid(SlotName)) return false;

	if (CurrentSaveManager->CreateFile(SlotName))
	{
		if (bSelectCreatedSlot)
			SelectSaveSlot(SlotName);

		UDefaultSaveGame* SaveGameData = CurrentSaveManager->GetSaveGame();
		SaveGameData->CreateSlotData(SlotName);
		SaveSlotData(SaveGameData);
		return true;
	}

	return false;
}

bool USlotsManager::CreateSlotFile()
{
	int32 AutoSaveSlots;
	int32 SaveSlots;
	GetNumberOfSlots(AutoSaveSlots, SaveSlots);
	const FString NextSlotName = SAVE_SLOT_NAME + FString::FromInt(SaveSlots);
	return CreateSlotFileOfName(NextSlotName);
}

bool USlotsManager::IsSelectedSlotValid()
{
	return !SelectedSaveSlotName.IsEmpty() && DoesSlotFileExist(SelectedSaveSlotName);
}

bool USlotsManager::IsSlotNameValid(const FString& SlotName)
{
	return
		!SlotName.IsEmpty();
}

bool USlotsManager::CreateAutoSaveSlotFile(FString& SlotName)
{
	int32 AutoSaveSlots;
	int32 SaveSlots;
	GetNumberOfSlots(AutoSaveSlots, SaveSlots);
	SlotName = AUTO_SAVE_SLOT_NAME + FString::FromInt(AutoSaveSlots);

	if (AutoSaveSlots > CurrentSaveManager->MaxAutoSaves)
	{
		FSaveSlotData SlotData;
		GetMostAncientSlotData(SlotData, true);
		SlotName = SlotData.SlotName;
	}
	
	return CreateSlotFileOfName(SlotName, false);
}

void USlotsManager::SaveSlotData(UDefaultSaveGame* SaveGame)
{
	SaveGame->SaveSlotData.LastSaveDate = FDateTime::Now();
	
	if (GEngine && GEngine->GameViewport)
	{
		if (const UWorld* World = GEngine->GameViewport->GetWorld())
		{
			if (!UGameplayStatics::IsGamePaused(World))
			{
				if (const APlayerController* PlayerController = World->GetFirstPlayerController())
				{
					const float TimeSinceCreation = PlayerController->GetGameTimeSinceCreation();
					const FTimespan TimeToAdd = FTimespan::FromSeconds(TimeSinceCreation);
					SaveGame->SaveSlotData.TimePlayed += TimeToAdd;
				}
			}
		}
	}
}
