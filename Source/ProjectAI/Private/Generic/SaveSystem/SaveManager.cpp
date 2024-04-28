// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/AutoSaveManager.h"
#include "Generic/SaveSystem/SlotSelectorManager.h"
#include "Generic/SaveSystem/Constants/SaveConstants.h"
#include "Generic/SaveSystem/Utility/SlotsUtility.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"

USaveManager::USaveManager()
{
	bIsAutoSaveEnabled = false;
	CurrentSaveGameInstance = nullptr;
	CurrentSlotInfos = nullptr;
	CurrentSlotInfoItem = nullptr;
	bIsLoading = false;
	bIsSaving = false;
	bSaveAsNewGame = false;
}

void USaveManager::Init(const TSubclassOf<USaveGame> SGClass, const TSubclassOf<USlotInfoItem> SIClass,
                        const FAutoSaveData AutoSaveInitData, const bool bCanEverUseAutoSave)
{
	if (!SGClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Save Game Class is not valid."));
		return;
	}

	if (!SIClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Save Info Class is not valid."));
		return;
	}

	SaveGameClass = SGClass;
	SlotInfoItemClass = SIClass;
	AutoSaveData = AutoSaveInitData;
	bIsAutoSaveEnabled = bCanEverUseAutoSave;
	
	CreateSaveInstances();
	LoadSlotInfos(); // Load slot infos before initializing other utilities

	USSUtility::Init(this);
	USlotsUtility::Init(this);
	USlotSelectorManager::Init(this);
	if (bIsAutoSaveEnabled)
		UAutoSaveManager::Init(this);

	UE_LOG(LogSaveSystem, Display, TEXT("Save System Initialized"));
}

UDefaultSaveGame* USaveManager::GetSaveGameInstance() const
{
	return CurrentSaveGameInstance;
}

bool USaveManager::DeleteSlot(const FString& SlotName)
{
	// Select the most recent slot after deleting the current one to avoid invalid selection
	if (!USlotsUtility::DoesSlotFileExist(SlotName)) return false;

	RemoveSlotInfo(FName(*SlotName));
	USlotSelectorManager::TrySelectMostRecentSaveGame();
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USaveManager::DeleteAllSlots()
{
	CreateSaveInstances();
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (FileManager.DeleteDirectory(*SaveDirectory, false, true))
		ClearSlotInfos();
}

void USaveManager::StartNewSaveGame()
{
	if (bIsLoading || bIsSaving) return;

	bSaveAsNewGame = true;
	ManualSave();
}

void USaveManager::ManualSave()
{
	if (bIsLoading || bIsSaving) return;

	const int32 SaveSlots = USlotsUtility::GetTotalManualSaveSlots();
	const FString NextManualSlotName = SAVE_SLOT_NAME + FString::FromInt(SaveSlots);
	Save(NextManualSlotName);
}

TArray<FSlotInfoData> USaveManager::GetSaveInfos() const
{
	TArray<FSlotInfoData> Infos;
	if (!CurrentSlotInfos) return Infos;

	CurrentSlotInfos->SlotInfos.GenerateValueArray(Infos);

	// Sort in descending order, so the most recent save is the first one
	Infos.Sort();
	Algo::Reverse(Infos);
	return Infos;
}

bool USaveManager::GetStatus(bool& OutbIsLoading, bool& OutbIsSaving) const
{
	OutbIsLoading = bIsLoading;
	OutbIsSaving = bIsSaving;
	return bIsLoading || bIsSaving;
}

void USaveManager::Save(const FString& SlotName)
{
	if (!USlotsUtility::IsSlotNameValid(SlotName) ||
		!CurrentSaveGameInstance ||
		bIsLoading || bIsSaving)
		return;

	bIsSaving = true;
	PreviousSlotNameKey = CurrentSaveGameInstance->SlotNameKey;
	CurrentSaveGameInstance->SetSlotNameKey(FName(*SlotName));
	// Notify the game that it's going to save, so all the saver objects can push their data to the save game object
	OnPrepareSave.Broadcast(CurrentSaveGameInstance, CurrentSlotInfoItem);
	FAsyncSaveGameToSlotDelegate AsyncSaveDelegate;
	AsyncSaveDelegate.BindUObject(this, &USaveManager::OnSaveCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSaveGameInstance, SAVES_DIRECTORY + SlotName, 0, AsyncSaveDelegate);
}

void USaveManager::Load(const FString& SlotName)
{
	if (!USlotsUtility::IsSlotNameValid(SlotName) ||
		!CurrentSaveGameInstance ||
		bIsLoading || bIsSaving)
		return;

	bIsLoading = true;
	OnPrepareLoad.Broadcast(CurrentSaveGameInstance);
	FAsyncLoadGameFromSlotDelegate AsyncLoadDelegate;
	AsyncLoadDelegate.BindUObject(this, &USaveManager::OnLoadCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SAVES_DIRECTORY + SlotName, 0, AsyncLoadDelegate);
}

void USaveManager::OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Game saved successfully."));
	}
	else
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Failed to save game."));
	}

	if (bSuccess)
	{
		const FString NewSaveSlotName = CurrentSaveGameInstance->SlotNameKey.ToString();
		UpdateSlotInfo(FName(*NewSaveSlotName));
		USlotSelectorManager::TrySelectSaveGameSlot(NewSaveSlotName);
		OnSaveGame.Broadcast(NewSaveSlotName, UserIndex, bSuccess, CurrentSaveGameInstance);
	}

	bIsSaving = false;
	bSaveAsNewGame = false;
}

void USaveManager::OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame)
{
	if (SaveGame)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Game loaded successfully."));
	}
	else
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Failed to load game."));
	}
	
	bIsLoading = false;
	if (!SaveGame) return;

	CurrentSaveGameInstance = Cast<UDefaultSaveGame>(SaveGame);
	if (!CurrentSaveGameInstance)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Failed Cast: Loaded game is not of type %s"), *SaveGameClass->GetName());
		return;
	}

	const FName SlotInfoName = CurrentSaveGameInstance->SlotNameKey;
	if (CurrentSlotInfos->SlotInfos.Contains(SlotInfoName))
	{
		if (USlotInfoItem* Item = CurrentSlotInfos->SlotInfos[SlotInfoName].SlotInfoItem)
			CurrentSlotInfoItem = Item;
	}

	OnLoadGame.Broadcast(SlotInfoName.ToString(), UserIndex, CurrentSaveGameInstance);
}

void USaveManager::UpdateSlotInfo(const FName NewSaveSlotNameKey)
{
	if (!CurrentSlotInfos) return;

	float CurrentPlayedTime = 0.f;
	if (!bSaveAsNewGame && CurrentSlotInfos->SlotInfos.Contains(PreviousSlotNameKey))
		CurrentPlayedTime = CurrentSlotInfos->SlotInfos[PreviousSlotNameKey].TimePlayed;

	if (!CurrentSlotInfos->SlotInfos.Contains(NewSaveSlotNameKey))
	{
		FSlotInfoData NewSlotInfo = FSlotInfoData(NewSaveSlotNameKey.ToString());
		NewSlotInfo.TimePlayed = CurrentPlayedTime;
		NewSlotInfo.SlotInfoItem = CurrentSlotInfoItem;
		CurrentSlotInfos->SlotInfos.Add(NewSaveSlotNameKey, NewSlotInfo);
	}

	CurrentSlotInfos->SlotInfos[NewSaveSlotNameKey].LastSaveDate = FDateTime::Now();

	if (float TimeSinceCreation; USlotsUtility::TryGetTimeSinceCreation(TimeSinceCreation))
		CurrentSlotInfos->SlotInfos[NewSaveSlotNameKey].TimePlayed += TimeSinceCreation;

	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfoItem, META_DIRECTORY + NewSaveSlotNameKey.ToString(), 0);
}

void USaveManager::RemoveSlotInfo(const FName& SlotNameKey)
{
	CurrentSlotInfos->SlotInfos.Remove(SlotNameKey);
	UGameplayStatics::DeleteGameInSlot(META_DIRECTORY + SlotNameKey.ToString(), 0);
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);
}

void USaveManager::ClearSlotInfos() const
{
	CurrentSlotInfos->SlotInfos.Empty();
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);

	const FString FullPath = META_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*META_DIRECTORY_FULLPATH, false, true);
}

void USaveManager::LoadSlotInfos()
{
	if (USlotInfos* Infos = Cast<USlotInfos>(UGameplayStatics::LoadGameFromSlot(META_INFOS_PATH, 0)))
	{
		CurrentSlotInfos = Infos;

		for (auto& SlotInfo : CurrentSlotInfos->SlotInfos)
		{
			USlotInfoItem* LoadedInfoItem = Cast<USlotInfoItem>(
				UGameplayStatics::LoadGameFromSlot(META_DIRECTORY + SlotInfo.Value.SlotInfoName, 0));
			if (!LoadedInfoItem)
				LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));

			SlotInfo.Value.SlotInfoItem = LoadedInfoItem;
		}
	}
}

void USaveManager::CreateSaveInstances()
{
	CurrentSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	CurrentSlotInfos = Cast<USlotInfos>(UGameplayStatics::CreateSaveGameObject(USlotInfos::StaticClass()));
	CurrentSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));
}
