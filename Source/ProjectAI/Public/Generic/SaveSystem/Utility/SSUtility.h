// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Generic/SaveSystem/SaveManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSUtility.generated.h"

UCLASS()
class PROJECTAI_API USSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;
	
public:
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static USaveManager* GetSaveManager();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static UDefaultSaveGame* GetSaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void StartNewSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ManualSave();

private:
	static bool Check();
};
