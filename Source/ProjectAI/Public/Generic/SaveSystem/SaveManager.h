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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnPrepareSave,
	UDefaultSaveGame*, SaveGameData,
	USlotInfoItem*, SlotInfoItem,
	UObject*, Istigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareLoad,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Istigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Istigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Istigator);

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
	UPROPERTY()
	UObject* CurrentIstigator;
	bool bIsLoading;
	bool bIsSaving;
	FName PreviousSlotNameKey;
	bool bSaveAsNewGame;

public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void Init(const TSubclassOf<USaveGame> SaveClass, const TSubclassOf<USlotInfoItem> InfoItemClass, FAutoSaveData AutoSaveInitData, const bool bCanEverUseAutoSave);

	UFUNCTION(BlueprintPure, Category = "Save System")
	UDefaultSaveGame* GetSaveGameInstance() const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool DeleteSlot(const FString& SlotName) const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void DeleteAllSlots();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void StartNewSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void ManualSave(UObject* Istigator);

	UFUNCTION(BlueprintPure, Category = "Save System")
	TArray<FSlotInfoData> GetSaveInfos() const;

	UFUNCTION(BlueprintPure, Category = "Save System")
	bool GetStatus(bool& OutbIsLoading, bool& OutbIsSaving) const;

	void Save(const FString& SlotName, UObject* Istigator);
	void Load(const FString& SlotName, UObject* Istigator);
private:
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);
	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);
	void UpdateSlotInfo(const FName NewSaveSlotNameKey) const;
	void RemoveSlotInfo(const FName& SlotNameKey) const;
	void ClearSlotInfos() const;
	void LoadSlotInfos();
	void CreateSaveInstances();
};
