// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Constants/SaveConstants.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "Data/Structs/AutoSaveData.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPrepareSave,
	UDefaultSaveGame*, SaveGameData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPrepareLoad,
	UDefaultSaveGame*, SaveGameData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData);

UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	TSubclassOf<UDefaultSaveGame> SaveGameClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	bool bIsAutoSaveEnabled;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	FAutoSaveData AutoSaveData;
	
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnPrepareSave OnPrepareSave;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnPrepareLoad OnPrepareLoad;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnSaveGame OnSaveGame;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnLoadGame OnLoadGame;

private:
	UPROPERTY()
	UDefaultSaveGame* CurrentSaveGameInstance;

public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void Init(const TSubclassOf<USaveGame> SGClass, FAutoSaveData ASaveData, bool bCanEverUseAutoSave);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool SelectSaveGameSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool SelectMostRecentSaveGame();

	UFUNCTION(BlueprintPure, Category = "Save System")
	bool SelectMostAncientSaveGame();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	UDefaultSaveGame* GetSaveGameInstance() const;

	UFUNCTION(BlueprintPure, Category = "Save System")
	TArray<UDefaultSaveGame*> GetAllSaveGames();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool DeleteSaveGameFile(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void DeleteAllSaveGameFiles();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveAsManualSave();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveOnSelectedSlot();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void CreateAndSaveSlot(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadFromSelectedSlot();

	bool CreateSaveSlotFile(const FString& SlotName);
	
private:
	void Save(const FString& SlotName);
	void Load(const FString& SlotName);
	void UpdateSaveGameSlotInfoData(UDefaultSaveGame* Save) const;
	void ClearSaveGameInstance();
	void OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const;
	void OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData);
};
