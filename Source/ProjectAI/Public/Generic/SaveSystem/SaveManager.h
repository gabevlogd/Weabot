// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "Data/Saves/SlotInfoItem.h"
#include "Data/Saves/SlotInfos.h"
#include "Data/Structs/AutoSaveData.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogSaveSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareSave,
	UDefaultSaveGame*, SaveGameData,
	USlotInfoItem*, SlotInfoItem);

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
	TSubclassOf<USlotInfoItem> SlotInfoItemClass;
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
	UPROPERTY()
	USlotInfos* CurrentSlotInfos;
	UPROPERTY()
	USlotInfoItem* CurrentSlotInfoItem;
	bool bIsLoading;
	bool bIsSaving;
	FName PreviousSlotNameKey;
	bool bSaveAsNewGame;

public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void Init(const TSubclassOf<USaveGame> SGClass, const TSubclassOf<USlotInfoItem> SIClass, FAutoSaveData AutoSaveInitData, const bool bCanEverUseAutoSave);

	UFUNCTION(BlueprintPure, Category = "Save System")
	UDefaultSaveGame* GetSaveGameInstance() const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool DeleteSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void DeleteAllSlots();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void StartNewSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void ManualSave();

	UFUNCTION(BlueprintPure, Category = "Save System")
	TArray<FSlotInfoData> GetSaveInfos() const;

	UFUNCTION(BlueprintPure, Category = "Save System")
	bool GetStatus(bool& OutbIsLoading, bool& OutbIsSaving) const;
	
	void Save(const FString& SlotName);
	void Load(const FString& SlotName);
private:
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);
	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);
	void UpdateSlotInfo(const FName NewSaveSlotNameKey);
	void RemoveSlotInfo(const FName& SlotNameKey);
	void ClearSlotInfos() const;
	void LoadSlotInfos();
	void CreateSaveInstances();
};
