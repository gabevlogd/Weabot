// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/SlotsManager.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): SaveGameData(nullptr)
{
}

void USaveManager::Init(const TSubclassOf<USaveGame> SaveGameClass, const bool bUseAutoSave, const float AutoSaveIntervalSeconds, const int MaxAutoSavesNumber)
{
	if (!SaveGameClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Save Game Class is not valid."));
		return;
	}

	CurrentSaveGameClass = SaveGameClass;
	bAutoSave = bUseAutoSave;
	AutoSaveInterval = AutoSaveIntervalSeconds;
	MaxAutoSaves = MaxAutoSavesNumber;
	
	USVUtility::Init(this);
	USlotsManager::Init(this);
	
	if (bAutoSave)
	{
		AutoSaveTimerHandle = FTimerHandle();
		GetWorldTimerManager().SetTimer(AutoSaveTimerHandle, this, &USaveManager::SaveAsAutoSave, AutoSaveInterval, true);
		UnPauseAutoSave();
	}
}

UDefaultSaveGame* USaveManager::GetSaveGame() const
{
	return SaveGameData;
}

TArray<UDefaultSaveGame*> USaveManager::GetAllSaveGames()
{
	TArray<UDefaultSaveGame*> SaveGames;
	TArray<FString> SlotNames;
	USlotsManager::GetSlotFilesNames(SlotNames, false);
	
	for (const FString& SaveGameName : SlotNames)
	{
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SAVES_DIRECTORY + SaveGameName, 0);
		if (UDefaultSaveGame* DefaultSaveGame = Cast<UDefaultSaveGame>(SaveGame))
		{
			SaveGames.Add(DefaultSaveGame);
		}
	}

	SaveGames.Sort();
	Algo::Reverse(SaveGames); // Sort in descending order, so the most recent save is the first one
	return SaveGames;
}

void USaveManager::SaveAsNewSlot()
{
	USlotsManager::CreateSlotFile();
	SaveOnSelectedSlot();
}

void USaveManager::SaveOnSlot(const FString& SlotName)
{
	if (!USlotsManager::DoesSlotFileExist(SlotName)) return;
	
	USlotsManager::SaveSlotData(SaveGameData);
	OnPrepareSave.Broadcast(SaveGameData); // Notify the game that it's going to save, so all the savable objects can push their data to the save game object
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &USaveManager::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameData, SAVES_DIRECTORY + SlotName, 0, SavedDelegate);
}

void USaveManager::LoadFromSlot(const FString& SlotName)
{
	if (!USlotsManager::DoesSlotFileExist(SlotName)) return;
	
	OnPrepareLoad.Broadcast(SaveGameData);
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USaveManager::OnLoadGameCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SAVES_DIRECTORY + SlotName, 0, LoadedDelegate);
}

void USaveManager::SaveOnSelectedSlot()
{
	if (!USlotsManager::IsSelectedSlotValid()) return;
	
	const FString SlotName = USlotsManager::GetSelectedSaveSlotName();
	SaveOnSlot(SlotName);
}

void USaveManager::LoadFromSelectedSlot()
{
	if (!USlotsManager::IsSelectedSlotValid()) return;
	
	const FString SlotName = USlotsManager::GetSelectedSaveSlotName();
	LoadFromSlot(SlotName);
}

void USaveManager::PauseAutoSave()
{
	bIsAutoSaveEnabled = false;
	GetWorldTimerManager().PauseTimer(AutoSaveTimerHandle);
}

void USaveManager::UnPauseAutoSave()
{
	bIsAutoSaveEnabled = true;
	GetWorldTimerManager().UnPauseTimer(AutoSaveTimerHandle);
}

void USaveManager::SaveAsAutoSave()
{
	if (!bAutoSave) return;
	FString AutoSaveSlotName;
	USlotsManager::CreateAutoSaveSlotFile(AutoSaveSlotName);
	SaveOnSlot(AutoSaveSlotName);
}

bool USaveManager::CreateFile(const FString& SlotFileName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotFileName, 0)) return false;

	SaveGameData = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(CurrentSaveGameClass));
	return UGameplayStatics::SaveGameToSlot(SaveGameData, SAVES_DIRECTORY + SlotFileName, 0);
}

void USaveManager::OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const
{
	UE_LOG(LogTemp, Warning, TEXT("Save Game %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	OnSaveGame.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USaveManager::OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData)
{
	UE_LOG(LogTemp, Warning, TEXT("Load Game %s"), LoadedData ? TEXT("Success") : TEXT("Failed"));
	SaveGameData = Cast<UDefaultSaveGame>(LoadedData);
	OnLoadGame.Broadcast(SlotName, UserIndex, SaveGameData);
}

FTimerManager& USaveManager::GetWorldTimerManager() const
{
	return GetWorld()->GetTimerManager();
}
