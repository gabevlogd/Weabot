// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReadyToSave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UGenericSaveGame*, SaveGameData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UGenericSaveGame*, SaveGameData);


UCLASS(BlueprintType)
class PROJECTAI_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
	TSubclassOf<UGenericSaveGame> SaveGameClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
	FString SaveSlotName;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnReadyToSave OnReadyToSave;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnSaveGame OnGameSaved;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnLoadGame OnGameLoaded;

private:
	UPROPERTY()
	UGenericSaveGame* SaveGameData;
	
public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void Init(const TSubclassOf<USaveGame> SaveClass, FString SlotName);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	UGenericSaveGame* GetSaveGame() const;
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void Save();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void Load();

private:
	void OnSaveGame(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const;
	void OnLoadGame(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData);
};
