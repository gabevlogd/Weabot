// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/SlotInfoData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlotsManager.generated.h"

UCLASS()
class PROJECTAI_API USlotsManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;

public:
	static void Init(USaveManager* SaveManager);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSlotFileExist(const FString& SlotName);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesAnySlotFileExist();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool GetSlotFilesNames(TArray<FString>& OutSaveFiles, bool bWithExtension = true);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalSlots();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalAutoSaveSlots();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalManualSaveSlots();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool GetMostRecentSlotInfoData(FSlotInfoData& OutSlotData, bool bIgnoreAutoSaves = false);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool GetMostAncientSlotInfoData(FSlotInfoData& OutSlotData, bool bIgnoreAutoSaves = false);
	
	static bool IsSlotNameValid(const FString& SlotName);
};
