// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/SaveSlotData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlotsManager.generated.h"


UCLASS()
class PROJECTAI_API USlotsManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static FString SelectedSaveSlotName;
	
public:
	static void Init();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool SelectSlot(const FString& SlotName);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static FString GetSelectedSlotName();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool CreateSlotFile(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool DeleteSlotFile(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void DeleteAllSlotFiles();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSlotFileExist(const FString& SlotName);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesAnySaveFileExist();

	static void SaveSlotData(FSaveSlotData& SlotData);
	static void SaveLastSelectedSaveSlot();
	static void LoadLastSelecetedSaveSlot();
};
