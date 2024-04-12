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
	
	if (!DoesAnySlotFileExist())
	{
		CreateSlotFile();
		return;
	}
	
	LoadSlotsInfo();
}

bool USlotsManager::SelectSaveSlot(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
	SelectedSaveSlotName = SlotName;
	SaveSlotsInfo();
	return true;
}

FString USlotsManager::GetSelectedSaveSlotName()
{
	return SelectedSaveSlotName;
}

bool USlotsManager::CreateSlotFileOfName(const FString& SlotName, const bool bSelectCreatedSlot)
{
	if (!IsSlotNameValid(SlotName)) return false;

	if (CurrentSaveManager->CreateFile(SlotName))
	{
		if (bSelectCreatedSlot)
			SelectSaveSlot(SlotName);
		
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
	const FString NextSlotName = SAVE_SLOT_NAME + FString::FromInt(GetNumberOfSlots());
	return CreateSlotFileOfName(NextSlotName);
}

bool USlotsManager::DeleteSlotFile(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName)) return false;
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
	return GetNumberOfSlots() > 0;
}

bool USlotsManager::IsSelectedSlotValid()
{
	return !SelectedSaveSlotName.IsEmpty() && DoesSlotFileExist(SelectedSaveSlotName);
}

bool USlotsManager::IsSlotNameValid(const FString& SlotName)
{
	return
	!SlotName.IsEmpty()
	&& !DoesSlotFileExist(SlotName);
}

TArray<FSaveSlotData> USlotsManager::GetAllSlots()
{
	TArray<FSaveSlotData> Slots;
	TArray<FString> SlotNames;
	USlotsManager::GetSlotFileNames(SlotNames);

	for (const FString& SaveGameName : SlotNames)
	{
		if (const UDefaultSaveGame* DefaultSaveGame = Cast<UDefaultSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0)))
			Slots.Add(DefaultSaveGame->SaveSlotData);
	}
	
	return Slots;
}

bool USlotsManager::GetSlotFileNames(TArray<FString>& OutSaveFiles)
{
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return false;
	
	FileManager.FindFiles(OutSaveFiles, *SaveDirectory, TEXT(SAVE_EXTENSION));
	return OutSaveFiles.Num() > 0;
}

int32 USlotsManager::GetNumberOfSlots()
{
	TArray<FString> SaveFiles;
	if (GetSlotFileNames(SaveFiles)) return 0;
	return SaveFiles.Num();
}

bool USlotsManager::GetMostRecentSlotData(FSaveSlotData& OutSlotData)
{
	FSaveSlotData Result = FSaveSlotData();
	TArray<FString> SaveFiles;
	if (!GetSlotFileNames(SaveFiles)) return false;

	FDateTime MostRecentDate = FDateTime::MinValue();
	for(const FString& SlotName : SaveFiles)
	{
		const UDefaultSaveGame* SaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		if (SaveGameInstance->SaveSlotData.LastSaveDate > MostRecentDate)
		{
			MostRecentDate = SaveGameInstance->SaveSlotData.LastSaveDate;
			Result = SaveGameInstance->SaveSlotData;
		}
	}

	return true;
}

bool USlotsManager::CreateAutoSaveSlotFile(FString& SlotName)
{
	SlotName = AUTO_SAVE_SLOT_NAME + FString::FromInt(GetNumberOfSlots());
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
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SLOTS_INFO_SAVE_NAME, 0);
}

void USlotsManager::LoadSlotsInfo()
{
	const USlotsInfoSave* SaveGameInstance = Cast<USlotsInfoSave>(UGameplayStatics::LoadGameFromSlot(SLOTS_INFO_SAVE_NAME, 0));
	if (!SaveGameInstance) return;
	
	SelectedSaveSlotName = SaveGameInstance->LastSelectedSaveSlotName;
	SelectSaveSlot(SelectedSaveSlotName);
}
