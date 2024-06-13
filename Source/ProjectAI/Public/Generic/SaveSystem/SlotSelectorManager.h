// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlotSelectorManager.generated.h"

UCLASS()
class PROJECTAI_API USlotSelectorManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;
	static FString CurrentSlotName;
	
public:
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static FString GetSelectedSlotName();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool TrySelectSaveGameSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool TrySelectMostRecentSaveGame();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TrySelectMostAncientSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SaveOnSelectedSlot(UObject* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadFromSelectedSlot(UObject* Instigator);
};
