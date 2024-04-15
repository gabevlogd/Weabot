// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/AutoSaveManager.h"
#include "Generic/SaveSystem/SlotSelectorManager.h"
#include "Generic/SaveSystem/Utility/SlotsUtility.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): bIsAutoSaveEnabled(false), CurrentSaveGameInstance(nullptr), CurrentSaveInfos(nullptr)
{
}

void USaveManager::Init(const TSubclassOf<USaveGame> SGClass, const FAutoSaveData ASaveData, bool bCanEverUseAutoSave)
{
	if (!SGClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Save Game Class is not valid."));
		return;
	}

	SaveGameClass = SGClass;
	AutoSaveData = ASaveData;
	bIsAutoSaveEnabled = bCanEverUseAutoSave;

	USSUtility::Init(this);
	USlotsUtility::Init(this);
	USlotSelectorManager::Init(this);
	if (bIsAutoSaveEnabled)
		UAutoSaveManager::Init(this);

	LoadSaveInfos();
	CreateSaveGameInstance();
}

UDefaultSaveGame* USaveManager::GetSaveGameInstance() const
{
	return CurrentSaveGameInstance;
}

bool USaveManager::DeleteSaveGameFile(const FString& SlotName)
{
	// Select the most recent slot after deleting the current one to avoid invalid selection
	USlotSelectorManager::TrySelectMostRecentSaveGame();
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USaveManager::DeleteAllSaveGameFiles()
{
	CreateSaveGameInstance();
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*SaveDirectory, false, true);
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
	if (!CurrentSaveInfos) return Infos;

	CurrentSaveInfos->SlotInfos.GenerateValueArray(Infos);

	// Sort in descending order, so the most recent save is the first one
	Infos.Sort();
	Algo::Reverse(Infos);
	return Infos;
}

bool USaveManager::CreateAndSaveSlot(const FString& SlotName)
{
	if (!USlotsUtility::IsSlotNameValid(SlotName)) return false;

	CurrentSaveGameInstance->CreateSlotInfoData(SlotName);
	UpdateSaveInfos();
	
	USlotSelectorManager::TrySelectSaveGameSlot(SlotName);
	Save(SlotName);
	return true;
}

void USaveManager::Save(const FString& SlotName)
{
	if (!CurrentSaveGameInstance) return;

	UpdateSaveGameSlotInfoData();
	// Notify the game that it's going to save, so all the saver objects can push their data to the save game object
	OnPrepareSave.Broadcast(CurrentSaveGameInstance);
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &USaveManager::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSaveGameInstance, SAVES_DIRECTORY + SlotName, 0, SavedDelegate);
}

void USaveManager::Load(const FString& SlotName)
{
	if (!USlotsUtility::DoesSlotFileExist(SlotName)) return;

	OnPrepareLoad.Broadcast(CurrentSaveGameInstance);
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &USaveManager::OnLoadGameCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SAVES_DIRECTORY + SlotName, 0, LoadedDelegate);
}

void USaveManager::OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const
{
	UE_LOG(LogTemp, Warning, TEXT("Save Game %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	OnSaveGame.Broadcast(SlotName, UserIndex, bSuccess, CurrentSaveGameInstance);
}

void USaveManager::OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData)
{
	UE_LOG(LogTemp, Warning, TEXT("Load Game %s"), LoadedData ? TEXT("Success") : TEXT("Failed"));
	CurrentSaveGameInstance = Cast<UDefaultSaveGame>(LoadedData);
	OnLoadGame.Broadcast(SlotName, UserIndex, CurrentSaveGameInstance);
}

void USaveManager::UpdateSaveGameSlotInfoData() const
{
	CurrentSaveGameInstance->SlotInfoData.LastSaveDate = FDateTime::Now();

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
					CurrentSaveGameInstance->SlotInfoData.TimePlayed += TimeToAdd;
				}
			}
		}
	}

	UpdateSaveInfos();
}

void USaveManager::UpdateSaveInfos() const
{
	const FName SlotName = *CurrentSaveGameInstance->SlotInfoData.SlotName;

	if (CurrentSaveInfos->SlotInfos.Contains(SlotName))
	{
		CurrentSaveInfos->SlotInfos[SlotName] = CurrentSaveGameInstance->SlotInfoData;
	}
	else
	{
		CurrentSaveInfos->SlotInfos.Add(SlotName, CurrentSaveGameInstance->SlotInfoData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveInfos, SAVE_INFOS_NAME, 0);
}

void USaveManager::LoadSaveInfos()
{
	USaveGame* Save = UGameplayStatics::LoadGameFromSlot(SAVE_INFOS_NAME, 0);
	if (USaveInfos* SaveInfos = Cast<USaveInfos>(Save))
		CurrentSaveInfos = SaveInfos;
	else
	{
		CurrentSaveInfos = NewObject<USaveInfos>(this);
	}
}

void USaveManager::CreateSaveGameInstance()
{
	CurrentSaveGameInstance = NewObject<UDefaultSaveGame>(this, SaveGameClass);
}
