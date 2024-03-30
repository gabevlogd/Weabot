// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Generic/SaveSystem/SaveManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SVUtility.generated.h"


UCLASS()
class PROJECTAI_API USVUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;
	
public:
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static USaveManager* GetSaveManager();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static USaveGame* GetSaveGameData();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void LoadGame();
};
