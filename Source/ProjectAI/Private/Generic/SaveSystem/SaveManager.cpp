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
	
	SaveGameData = Cast<UGenericSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveClass));
	USVUtility::Init(this);
	Load();
}

void USaveManager::Save()
{
	OnReadyToSave.Broadcast(); // Notify the game that it's going to save, so all the savable objects can push their data to the save game object.
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this,  &USaveManager::OnSaveGame);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameData, SaveSlotName, 0, SavedDelegate);
}

void USaveManager::Load()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return;
	
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USaveManager::OnLoadGame);
	UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, LoadedDelegate);
}

void USaveManager::OnSaveGame(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const
{
	UE_LOG(LogTemp, Warning, TEXT("Save Game %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	OnGameSaved.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USaveManager::OnLoadGame(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData)
{
	UE_LOG(LogTemp, Warning, TEXT("Load Game %s"), LoadedData ? TEXT("Success") : TEXT("Failed"));
	SaveGameData = Cast<UGenericSaveGame>(LoadedData);
	OnGameLoaded.Broadcast(SlotName, UserIndex, SaveGameData);
}
