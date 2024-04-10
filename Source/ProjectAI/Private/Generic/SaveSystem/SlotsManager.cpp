// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SlotsManager.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Data/Saves/LastSelectedSlotSave.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


FString USlotsManager::SelectedSaveSlotName = DEFAULT_SAVE_GAME_NAME;

void USlotsManager::Init()
{
	if (!DoesAnySaveFileExist())
	{
		CreateSlotFile(DEFAULT_SAVE_GAME_NAME); // If there are no save files, create a default one
		return;
	}
	
	LoadLastSelecetedSaveSlot();
}

bool USlotsManager::SelectSlot(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
	SelectedSaveSlotName = SlotName;
	return true;
}

FString USlotsManager::GetSelectedSlotName()
{
	return SelectedSaveSlotName;
}

bool USlotsManager::CreateSlotFile(const FString& SlotName)
{
	if (DoesSlotFileExist(SlotName)) return false;

	if(USVUtility::GetSaveManager()->CreateFile(SlotName))
	{
		SelectSlot(SlotName);
		UDefaultSaveGame* SaveGameData = USVUtility::GetSaveGameData();
		FSaveSlotData& SlotData = SaveGameData->SaveSlotData;
		SlotData = FSaveSlotData();
		SlotData.SlotName = SlotName;
		return true;
	}
	
	return false;
}

bool USlotsManager::DeleteSlotFile(const FString& SlotName)
{
	if (DoesSlotFileExist(SlotName)) return false;

	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USlotsManager::DeleteAllSlotFiles()
{
	const FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/");
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*SaveDirectory, false, true);
}

bool USlotsManager::DoesSlotFileExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool USlotsManager::DoesAnySaveFileExist()
{
	const FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/");
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return false;

	TArray<FString> SaveFiles;
	FileManager.FindFiles(SaveFiles, *SaveDirectory, true, false);
	return SaveFiles.Num() > 0;
}

void USlotsManager::SaveSlotData(FSaveSlotData& SlotData)
{
	SlotData.LastSaveDate = FDateTime::Now();
	if (GEngine && GEngine->GameViewport)
	{
		if (const UWorld* World = GEngine->GameViewport->GetWorld())
		{
			if (const APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				const float TimeSinceCreation = PlayerController->GetGameTimeSinceCreation();
				SlotData.TimePlayed += TimeSinceCreation;
			}
		}
	}
}

void USlotsManager::SaveLastSelectedSaveSlot()
{
	ULastSelectedSlotSave* SaveGameInstance = Cast<ULastSelectedSlotSave>(UGameplayStatics::CreateSaveGameObject(ULastSelectedSlotSave::StaticClass()));
	SaveGameInstance->LastSelectedSaveSlotName = SelectedSaveSlotName;
}

void USlotsManager::LoadLastSelecetedSaveSlot()
{
	const ULastSelectedSlotSave* SaveGameInstance = Cast<ULastSelectedSlotSave>(UGameplayStatics::CreateSaveGameObject(ULastSelectedSlotSave::StaticClass()));
	if (!SaveGameInstance) return;
	
	SelectedSaveSlotName = SaveGameInstance->LastSelectedSaveSlotName;
	SelectSlot(SelectedSaveSlotName);
}
