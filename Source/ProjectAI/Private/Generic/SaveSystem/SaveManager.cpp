// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/SlotsManager.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): SaveGameData(nullptr)
{
}

UDefaultSaveGame* USaveManager::GetSaveGame() const
{
	return SaveGameData;
}

void USaveManager::Init(const TSubclassOf<USaveGame> SaveGameClass)
{
	if (!SaveGameClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Save Game Class or Slot Name is not valid."));
		return;
	}

	CurrentSaveGameClass = SaveGameClass;

	USVUtility::Init(this);
	USlotsManager::Init();
	Load();
}

void USaveManager::Save()
{
	const FString SlotName = USlotsManager::GetSelectedSlotName();
	USlotsManager::SaveSlotData(SaveGameData->SaveSlotData);
	OnPrepareSave.Broadcast(SaveGameData); // Notify the game that it's going to save, so all the savable objects can push their data to the save game object
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &USaveManager::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameData, SlotName, 0, SavedDelegate);
}

void USaveManager::Load()
{
	const FString SlotName = USlotsManager::GetSelectedSlotName();
	OnPrepareLoad.Broadcast(SaveGameData);
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USaveManager::OnLoadGameCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadedDelegate);
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

bool USaveManager::CreateFile(const FString& SlotFileName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotFileName, 0)) return false;

	SaveGameData = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(CurrentSaveGameClass));
	return UGameplayStatics::SaveGameToSlot(SaveGameData, SlotFileName, 0);
}
