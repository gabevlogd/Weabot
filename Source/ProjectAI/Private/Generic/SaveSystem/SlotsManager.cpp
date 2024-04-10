// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SlotsManager.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Data/Saves/SlotsInfoSave.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


FString USlotsManager::SelectedSaveSlotName = "";
USaveManager* USlotsManager::CurrentSaveManager = nullptr;

void USlotsManager::Init(USaveManager* SaveManager)
{
	if (!SaveManager) return;
	
	CurrentSaveManager = SaveManager;
	
	if (!DoesAnySaveFileExist())
	{
		CreateSlotFile();
		return;
	}
	
	LoadSlotsInfo();
}

bool USlotsManager::SelectSlot(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
	SelectedSaveSlotName = SlotName;
	SaveSlotsInfo();
	return true;
}

FString USlotsManager::GetSelectedSlotName()
{
	return SelectedSaveSlotName;
}

bool USlotsManager::CreateSlotFileOfName(const FString& SlotName, const bool bSelectCreatedSlot)
{
	if (!IsSaveSlotNameValid(SlotName)) return false;

	if (CurrentSaveManager->CreateFile(SlotName))
	{
		if (bSelectCreatedSlot)
			SelectSlot(SlotName);
		
		UDefaultSaveGame* SaveGameData = CurrentSaveManager->GetSaveGame();
		FSaveSlotData& SlotData = SaveGameData->SaveSlotData;
		SlotData = FSaveSlotData();
		SlotData.SlotName = SlotName;
		return true;
	}
	
	return false;
}

bool USlotsManager::CreateSlotFile()
{
	const FString NextSlotName = SAVE_SLOT_NAME + FString::FromInt(GetNumberOfSaveSlots());
	return CreateSlotFileOfName(NextSlotName);
}

bool USlotsManager::DeleteSlotFile(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USlotsManager::DeleteAllSlotFiles()
{
	const FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/" + SAVES_DIRECTORY);
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*SaveDirectory, false, true);
}

bool USlotsManager::DoesSlotFileExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SAVES_DIRECTORY + SlotName, 0);
}

bool USlotsManager::DoesAnySaveFileExist()
{
	return GetNumberOfSaveSlots() > 0;
}

bool USlotsManager::IsSelectedSlotValid()
{
	return !SelectedSaveSlotName.IsEmpty() && DoesSlotFileExist(SelectedSaveSlotName);
}

bool USlotsManager::IsSaveSlotNameValid(const FString& SlotName)
{
	return
	!SlotName.IsEmpty()
	&& !DoesSlotFileExist(SlotName);
}

int32 USlotsManager::GetNumberOfSaveSlots()
{
	const FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/" + SAVES_DIRECTORY);
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return 0;

	TArray<FString> SaveFiles;
	FileManager.FindFiles(SaveFiles, *SaveDirectory, TEXT("sav"));
	return SaveFiles.Num();
}

bool USlotsManager::CreateAutoSaveSlotFile(FString& SlotName)
{
	SlotName = AUTO_SAVE_SLOT_NAME + FString::FromInt(GetNumberOfSaveSlots());
	return CreateSlotFileOfName(SlotName, false);
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

void USlotsManager::SaveSlotsInfo()
{
	USlotsInfoSave* SaveGameInstance = Cast<USlotsInfoSave>(UGameplayStatics::CreateSaveGameObject(USlotsInfoSave::StaticClass()));
	SaveGameInstance->LastSelectedSaveSlotName = SelectedSaveSlotName;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SLOTS_INFO_SLOT_NAME, 0);
}

void USlotsManager::LoadSlotsInfo()
{
	const USlotsInfoSave* SaveGameInstance = Cast<USlotsInfoSave>(UGameplayStatics::LoadGameFromSlot(SLOTS_INFO_SLOT_NAME, 0));
	if (!SaveGameInstance) return;
	
	SelectedSaveSlotName = SaveGameInstance->LastSelectedSaveSlotName;
	SelectSlot(SelectedSaveSlotName);
}
