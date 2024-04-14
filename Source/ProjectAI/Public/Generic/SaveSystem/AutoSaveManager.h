// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/AutoSaveData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutoSaveManager.generated.h"

UCLASS()
class PROJECTAI_API UAutoSaveManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;
	static FAutoSaveData* AutoSaveData;
	static FTimerHandle AutoSaveTimerHandle;
	static bool bIsPaused;

public:
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void PauseAutoSave();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void UnPauseAutoSave();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsAutoSavePaused();
	
private:
	static bool GetMostAncientAutoSaveSlotInfoData(FSlotInfoData& OutSlotData);
	static void SaveAsAutoSave();
	static bool CreateAutoSaveSlotFile(FString& SlotName);
	static FTimerManager* GetWorldTimerManager();
};
