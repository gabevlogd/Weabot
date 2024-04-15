// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/AutoSaveManager.h"
#include "Generic/SaveSystem/Utility/SlotsUtility.h"
#include "Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"


USaveManager::USaveManager(): bIsAutoSaveEnabled(false), CurrentSaveGameInstance(nullptr)
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
	
	if (!SelectMostRecentSaveGame())
	{
		// Create a new save game object to avoid nullptr exceptions
		// so if the game tries to save before selecting a slot, it will not crash,
		// but the save game object will be empty
		CurrentSaveGameInstance = NewObject<UDefaultSaveGame>(this, SaveGameClass);
	}
		
	USSUtility::Init(this);
	USlotsUtility::Init(this);

	if (bIsAutoSaveEnabled)
		UAutoSaveManager::Init(this);
}

bool USaveManager::SelectSaveGameSlot(const FString& SlotName)
{
	if (!USlotsUtility::DoesSlotFileExist(SlotName)) return false;
	CurrentSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::LoadGameFromSlot(SAVES_DIRECTORY + SlotName, 0));
	return CurrentSaveGameInstance != nullptr;
}

UDefaultSaveGame* USaveManager::GetSaveGameInstance() const
{
	return CurrentSaveGameInstance;
}

TArray<UDefaultSaveGame*> USaveManager::GetAllSaveGames()
{
	TArray<UDefaultSaveGame*> SaveGames;
	TArray<FString> SlotNames;
	USlotsUtility::GetSlotFilesNames(SlotNames, false);
	
	for (const FString& SaveGameName : SlotNames)
	{
		USaveGame* Save = UGameplayStatics::LoadGameFromSlot(SAVES_DIRECTORY + SaveGameName, 0);
		if (UDefaultSaveGame* DefSaveGame = Cast<UDefaultSaveGame>(Save))
		{
			SaveGames.Add(DefSaveGame);
		}
	}

	// Sort in descending order, so the most recent save is the first one
	SaveGames.Sort();
	Algo::Reverse(SaveGames);
	return SaveGames;
}

bool USaveManager::SelectMostRecentSaveGame()
{
	if(FSlotInfoData SlotData; USlotsUtility::GetMostRecentSlotInfoData(SlotData))
	{
		SelectSaveGameSlot(SlotData.SlotName);
		return true;
	}

	return false;
}

bool USaveManager::SelectMostAncientSaveGame()
{
	if(FSlotInfoData SlotData; USlotsUtility::GetMostAncientSlotInfoData(SlotData))
	{
		SelectSaveGameSlot(SlotData.SlotName);
		return true;
	}

	return false;
}

bool USaveManager::DeleteSaveGameFile(const FString& SlotName)
{
	// Select the most recent slot after deleting the current one to avoid invalid selection
	SelectMostRecentSaveGame();
	
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USaveManager::DeleteAllSaveGameFiles()
{
	ClearSaveGameInstance(); // Clear the current save game instance to avoid invalid selection
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*SaveDirectory, false, true);
}

void USaveManager::SaveAsManualSave()
{
	const int32 SaveSlots = USlotsUtility::GetTotalManualSaveSlots();
	const FString NextManualSlotName = SAVE_SLOT_NAME + FString::FromInt(SaveSlots);
	
	CreateAndSaveSlot(NextManualSlotName);
}

void USaveManager::SaveOnSelectedSlot()
{
	if (!CurrentSaveGameInstance) return;
	const FString SlotName = CurrentSaveGameInstance->SlotInfoData.SlotName;
	Save(SlotName);
}

void USaveManager::CreateAndSaveSlot(const FString& SlotName)
{
	CreateSaveSlotFile(SlotName);
	SaveOnSelectedSlot();
}

void USaveManager::LoadFromSelectedSlot()
{
	if (!CurrentSaveGameInstance) return;
	const FString SlotName = CurrentSaveGameInstance->SlotInfoData.SlotName;
	Load(SlotName);
}

bool USaveManager::CreateSaveSlotFile(const FString& SlotName)
{
	if (!USlotsUtility::IsSlotNameValid(SlotName)) return false;
	
	UDefaultSaveGame* Save = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	Save->CreateSlotInfoData(SlotName);
	UpdateSaveGameSlotInfoData(Save);

	if (UGameplayStatics::SaveGameToSlot(Save, SAVES_DIRECTORY + SlotName, 0))
	{
		SelectSaveGameSlot(SlotName);
		return true;
	}

	return false;
}

void USaveManager::Save(const FString& SlotName)
{
	if (!CurrentSaveGameInstance) return;
	
	UpdateSaveGameSlotInfoData(CurrentSaveGameInstance);
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

void USaveManager::UpdateSaveGameSlotInfoData(UDefaultSaveGame* Save) const
{
	Save->SlotInfoData.LastSaveDate = FDateTime::Now();
	
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
					Save->SlotInfoData.TimePlayed += TimeToAdd;
				}
			}
		}
	}
}

void USaveManager::ClearSaveGameInstance()
{
	CurrentSaveGameInstance = nullptr;
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
