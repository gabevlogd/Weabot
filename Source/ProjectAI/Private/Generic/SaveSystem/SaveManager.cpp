// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/AutoSaveManager.h"
#include "Generic/SaveSystem/SlotSelectorManager.h"
#include "Generic/SaveSystem/Constants/SaveConstants.h"
#include "Generic/SaveSystem/Utility/SlotsUtility.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): bIsAutoSaveEnabled(false), CurrentSaveGameInstance(nullptr), CurrentSlotInfos(nullptr), CurrentSlotInfoItem(nullptr), bIsLoading(false), bIsSaving(false)
{
}

void USaveManager::Init(const TSubclassOf<USaveGame> SGClass, const TSubclassOf<USlotInfoItem> SIClass, const FAutoSaveData AutoSaveInitData, const bool bCanEverUseAutoSave)
{
	if (!SGClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Save Game Class is not valid."));
		return;
	}
	
	SaveGameClass = SGClass;
	SlotInfoItemClass = SIClass;
	AutoSaveData = AutoSaveInitData;
	bIsAutoSaveEnabled = bCanEverUseAutoSave;
	
	USSUtility::Init(this);
	USlotsUtility::Init(this);
	if (bIsAutoSaveEnabled)
		UAutoSaveManager::Init(this);
	
	CreateSaveGameInstance();
	LoadSlotInfos();
	
	USlotSelectorManager::Init(this);
}

UDefaultSaveGame* USaveManager::GetSaveGameInstance() const
{
	return CurrentSaveGameInstance;
}

bool USaveManager::DeleteSlot(const FString& SlotName)
{
	// Select the most recent slot after deleting the current one to avoid invalid selection
	USlotSelectorManager::TrySelectMostRecentSaveGame();
	if (!USlotsUtility::DoesSlotFileExist(SlotName)) return false;

	RemoveSlotInfo(FName(*SlotName));
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USaveManager::DeleteAllSlots()
{
	CreateSaveGameInstance();
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (FileManager.DeleteDirectory(*SaveDirectory, false, true))
		ClearSlotInfos();
}

void USaveManager::ManualSave()
{
	const int32 SaveSlots = USlotsUtility::GetTotalManualSaveSlots();
	const FString NextManualSlotName = SAVE_SLOT_NAME + FString::FromInt(SaveSlots);
	CreateAndSaveSlot(NextManualSlotName);
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

bool USaveManager::CreateAndSaveSlot(const FString& SlotName)
{
	if (!USlotsUtility::IsSlotNameValid(SlotName) ||
		!CurrentSaveGameInstance ||
		bIsLoading || bIsSaving)
		return false;

	CurrentSaveGameInstance->SetSlotName(FName(*SlotName));
	const FSlotInfoData SlotInfoData = FSlotInfoData(SlotName);
	AddSlotInfo(SlotInfoData);

	USlotSelectorManager::TrySelectSaveGameSlot(SlotName);
	Save(SlotName);
	return true;
}

void USaveManager::Save(const FString& SlotName)
{
	if (!USlotsUtility::IsSlotNameValid(SlotName) ||
		!CurrentSaveGameInstance ||
		bIsLoading || bIsSaving)
		return;

	// Notify the game that it's going to save, so all the saver objects can push their data to the save game object
	bIsSaving = true;
	OnPrepareSave.Broadcast(CurrentSaveGameInstance, CurrentSlotInfoItem);
	FAsyncSaveGameToSlotDelegate AsyncSaveDelegate = FAsyncSaveGameToSlotDelegate();
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
	FAsyncLoadGameFromSlotDelegate AsyncLoadDelegate = FAsyncLoadGameFromSlotDelegate();
	AsyncLoadDelegate.BindUObject(this, &USaveManager::OnLoadCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SAVES_DIRECTORY + SlotName, 0, AsyncLoadDelegate);
}

void USaveManager::OnSaveCompleted(const FString& SlotName, int32 UserIndex, bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Save Game %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	bIsSaving = false;
	UpdateCurrentSaveGameSlotInfoData();
	OnSaveGame.Broadcast(SlotName, UserIndex, bSuccess, CurrentSaveGameInstance);
}

void USaveManager::OnLoadCompleted(const FString& SlotName, int32 UserIndex, USaveGame* SaveGame)
{
	UE_LOG(LogTemp, Warning, TEXT("Load Game %s"), SaveGame ? TEXT("Success") : TEXT("Failed"));
	bIsLoading = false;
	
	if (!SaveGame) return;
	
	CurrentSaveGameInstance = Cast<UDefaultSaveGame>(SaveGame);
	if (!CurrentSaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Loaded Save Game is not valid."));
		return;
	}

	const FName SlotInfoName = CurrentSaveGameInstance->SlotInfoName;
	if (CurrentSlotInfos->SlotInfos.Contains(SlotInfoName))
	{
		if (USlotInfoItem* Item = CurrentSlotInfos->SlotInfos[SlotInfoName].SlotInfoItem)
			CurrentSlotInfoItem = Item;
	}
	
	OnLoadGame.Broadcast(SlotName, UserIndex, CurrentSaveGameInstance);
}

void USaveManager::UpdateCurrentSaveGameSlotInfoData()
{
	const FName SlotName = CurrentSaveGameInstance->SlotInfoName;
	FSlotInfoData& SlotInfoData = CurrentSlotInfos->SlotInfos[SlotName];
	SlotInfoData.LastSaveDate = FDateTime::Now();

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
					SlotInfoData.TimePlayed += TimeToAdd;
				}
			}
		}
	}

	AddSlotInfo(SlotInfoData);
}

void USaveManager::AddSlotInfo(const FSlotInfoData& SlotInfoData)
{
	const FName InKey = FName(*SlotInfoData.SlotName);

	if (CurrentSlotInfos->SlotInfos.Contains(InKey))
		CurrentSlotInfos->SlotInfos[InKey] = SlotInfoData;
	else
	{
		CurrentSlotInfos->SlotInfos.Add(InKey, SlotInfoData);
		CurrentSlotInfos->SlotInfos[InKey].SlotInfoItem = CurrentSlotInfoItem;
	}

	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, SLOT_INFOS_PATH_NAME, 0);
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfoItem, META_DIRECTORY + SlotInfoData.SlotName, 0);
}

void USaveManager::RemoveSlotInfo(const FName& SlotName)
{
	CurrentSlotInfos->SlotInfos.Remove(SlotName);
	UGameplayStatics::DeleteGameInSlot(META_DIRECTORY + SlotName.ToString(), 0);
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, SLOT_INFOS_PATH_NAME, 0);
}

void USaveManager::ClearSlotInfos() const
{
	CurrentSlotInfos->SlotInfos.Empty();
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, SLOT_INFOS_PATH_NAME, 0);

	const FString FullPath = META_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*META_DIRECTORY_FULLPATH, false, true);
}

void USaveManager::LoadSlotInfos()
{
	USaveGame* SaveGameInfo = UGameplayStatics::LoadGameFromSlot(SLOT_INFOS_PATH_NAME, 0);
	CurrentSlotInfos = Cast<USlotInfos>(SaveGameInfo);
	if (!CurrentSlotInfos)
		CurrentSlotInfos = Cast<USlotInfos>(UGameplayStatics::CreateSaveGameObject(USlotInfos::StaticClass()));
	
	for (auto& SlotInfo : CurrentSlotInfos->SlotInfos)
	{
		USlotInfoItem* LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::LoadGameFromSlot(META_DIRECTORY + SlotInfo.Value.SlotName, 0));
		if (!LoadedInfoItem)
			LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));
	
		SlotInfo.Value.SlotInfoItem = LoadedInfoItem;
	}
}

void USaveManager::CreateSaveGameInstance()
{
	CurrentSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	CurrentSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));
}
