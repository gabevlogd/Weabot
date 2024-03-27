// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): SaveGameData(nullptr)
{
}

UGenericSaveGame* USaveManager::GetSaveGame() const
{
	return SaveGameData;
}

void USaveManager::Init(const TSubclassOf<USaveGame> SaveClass, FString SlotName)
{
	if (!SaveClass || SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Save Game Class or Slot Name is not valid."));
		return;
	}
	
	SaveGameClass = SaveClass;
	SaveSlotName = SlotName;
	
	CheckFileExistence(SaveClass);
	USVUtility::Init(this);
	Load();
}

void USaveManager::Save()
{
	OnPrepareSave.Broadcast(SaveGameData); // Notify the game that it's going to save, so all the savable objects can push their data to the save game object

	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this,  &USaveManager::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameData, SaveSlotName, 0, SavedDelegate);
}

void USaveManager::Load()
{
	OnPrepareLoad.Broadcast(SaveGameData);
	
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USaveManager::OnLoadGameCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, LoadedDelegate);
}

void USaveManager::OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const
{
	UE_LOG(LogTemp, Warning, TEXT("Save Game %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	OnSaveGame.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USaveManager::OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData)
{
	UE_LOG(LogTemp, Warning, TEXT("Load Game %s"), LoadedData ? TEXT("Success") : TEXT("Failed"));
	SaveGameData = Cast<UGenericSaveGame>(LoadedData);
	OnLoadGame.Broadcast(SlotName, UserIndex, SaveGameData);
}

void USaveManager::CheckFileExistence(const TSubclassOf<USaveGame> SaveClass)
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) // If the save file doesn't exist, create a new one
	{
		SaveGameData = Cast<UGenericSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveClass));
		UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, 0);
	}
}
