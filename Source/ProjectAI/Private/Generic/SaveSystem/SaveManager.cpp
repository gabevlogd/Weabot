// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/DefaultSaveGame.h"
#include "Generic/SaveSystem/ProfilesManager.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): SaveGameData(nullptr)
{
}

UDefaultSaveGame* USaveManager::GetSaveGame() const
{
	return SaveGameData;
}

void USaveManager::Init(const TSubclassOf<USaveGame> SaveClass)
{
	if (!SaveClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Save Game Class or Slot Name is not valid."));
		return;
	}
	
	SaveGameClass = SaveClass;
	
	CheckFileExistence(SaveClass);
	USVUtility::Init(this);
	UProfilesManager::Init();
	Load();
}

void USaveManager::Save()
{
	OnPrepareSave.Broadcast(SaveGameData); // Notify the game that it's going to save, so all the savable objects can push their data to the save game object

	const FString SlotName = GetSaveSlotName();
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this,  &USaveManager::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameData, SlotName, 0, SavedDelegate);
}

void USaveManager::Load()
{
	OnPrepareLoad.Broadcast(SaveGameData);

	const FString SlotName = GetSaveSlotName();
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USaveManager::OnLoadGameCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadedDelegate);
}

FString USaveManager::GetSaveSlotName()
{
	return UProfilesManager::GetCurrentProfile().ProfileName;
}

void USaveManager::OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const
{
	UE_LOG(LogTemp, Warning, TEXT("Save Game %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	UProfilesManager::SaveProfiles(SaveGameData); // Save the profiles after the save game is saved
	OnSaveGame.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USaveManager::OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData)
{
	UE_LOG(LogTemp, Warning, TEXT("Load Game %s"), LoadedData ? TEXT("Success") : TEXT("Failed"));
	SaveGameData = Cast<UDefaultSaveGame>(LoadedData);
	UProfilesManager::LoadProfiles(SaveGameData); // Load the profiles after the save game is loaded
	OnLoadGame.Broadcast(SlotName, UserIndex, SaveGameData);
}

void USaveManager::CheckFileExistence(const TSubclassOf<USaveGame> SaveClass)
{
	const FString SlotName = GetSaveSlotName();
	
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) // If the save file doesn't exist, create a new one
	{
		SaveGameData = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveClass));
		UGameplayStatics::SaveGameToSlot(SaveGameData, SlotName, 0);
	}
}
