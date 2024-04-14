// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Generic/SaveSystem/AutoSaveManager.h"
#include "Generic/SaveSystem/SaveManager.h"
#include "Generic/SaveSystem/SlotsManager.h"


USaveManager* UAutoSaveManager::CurrentSaveManager = nullptr;
FAutoSaveData* UAutoSaveManager::AutoSaveData = nullptr;
FTimerHandle UAutoSaveManager::AutoSaveTimerHandle;
bool UAutoSaveManager::bIsPaused = false;

void UAutoSaveManager::Init(USaveManager* SaveManager)
{
	CurrentSaveManager = SaveManager;
	AutoSaveData = &SaveManager->AutoSaveData;

	AutoSaveTimerHandle = FTimerHandle();
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateStatic(&UAutoSaveManager::SaveAsAutoSave);

	if (FTimerManager* TimerManager = GetWorldTimerManager())
	{
		TimerManager->SetTimer(AutoSaveTimerHandle, TimerDelegate, AutoSaveData->AutoSaveIntervalSeconds, true);
		if (AutoSaveData->bInitPauseState)
			PauseAutoSave();
		else
			UnPauseAutoSave();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World Timer Manager is not valid."));
	}
}

void UAutoSaveManager::SaveAsAutoSave()
{
	if (bIsPaused) return; // Safe check, should not happen since the timer is paused

	FString AutoSaveSlotName;
	CreateAutoSaveSlotFile(AutoSaveSlotName);
	CurrentSaveManager->SaveOnSelectedSlot();
}

bool UAutoSaveManager::CreateAutoSaveSlotFile(FString& SlotName)
{
	const int32 CurrentAutoSavesNumber = USlotsManager::GetTotalAutoSaveSlots();

	SlotName = AUTO_SAVE_SLOT_NAME + FString::FromInt(CurrentAutoSavesNumber);
	if (CurrentAutoSavesNumber >= AutoSaveData->MaxAutoSaves)
	{
		FSlotInfoData SlotData;
		GetMostAncientAutoSaveSlotInfoData(SlotData);
		SlotName = SlotData.SlotName;

		const FString Message = FString::Printf(TEXT("Auto Save limit reached. Overwriting the most ancient auto save slot. %s"), *SlotName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	}

	return CurrentSaveManager->CreateSaveSlotFile(SlotName);
}

void UAutoSaveManager::PauseAutoSave()
{
	bIsPaused = true;
	GetWorldTimerManager()->PauseTimer(AutoSaveTimerHandle);
}

void UAutoSaveManager::UnPauseAutoSave()
{
	bIsPaused = false;
	GetWorldTimerManager()->UnPauseTimer(AutoSaveTimerHandle);
}

bool UAutoSaveManager::IsAutoSavePaused()
{
	return bIsPaused;
}

bool UAutoSaveManager::GetMostAncientAutoSaveSlotInfoData(FSlotInfoData& OutSlotData)
{
	TArray<UDefaultSaveGame*> SaveGames = CurrentSaveManager->GetAllSaveGames();

	SaveGames.RemoveAll([](const UDefaultSaveGame* SaveGame)
	{
		return SaveGame->SlotInfoData.SlotName.Contains(SAVE_SLOT_NAME);
	});

	if (SaveGames.Num() <= 0) return false;

	OutSlotData = SaveGames[SaveGames.Num() - 1]->SlotInfoData;
	return true;
}

FTimerManager* UAutoSaveManager::GetWorldTimerManager()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("Engine is not valid."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Engine is not valid."));
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(CurrentSaveManager, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is not valid."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("World is not valid."));
		return nullptr;
	}

	return &World->GetTimerManager();
}
