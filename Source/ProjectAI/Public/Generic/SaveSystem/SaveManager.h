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
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareLoad,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnNewSaveGame
	);

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	double ElapsedTimePlayed;
	
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnPrepareSave OnPrepareSave;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnPrepareLoad OnPrepareLoad;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnSaveGame OnSaveGame;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnLoadGame OnLoadGame;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnNewSaveGame OnNewSaveGame;

private:
	UPROPERTY()
	UDefaultSaveGame* CurrentSaveGameInstance;
	UPROPERTY()
	USlotInfos* CurrentSlotInfos;
	UPROPERTY()
	USlotInfoItem* CurrentSlotInfoItem;
	UPROPERTY()
	UObject* CurrentInstigator;
	bool bIsLoading;
	bool bIsSaving;
	FName PreviousSlotNameKey;
	bool bSaveAsNewGame;
	FTimerHandle TimePlayedTimerHandle;

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
	void ManualSave(UObject* Instigator);

	UFUNCTION(BlueprintPure, Category = "Save System")
	TArray<FSlotInfoData> GetSaveInfos() const;

	UFUNCTION(BlueprintPure, Category = "Save System")
	bool GetStatus(bool& OutbIsLoading, bool& OutbIsSaving) const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void StartTimePlayedTimer();

	void Save(const FString& SlotName, UObject* Instigator);
	void Load(const FString& SlotName, UObject* Instigator);
private:
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);
	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);
	void UpdateSlotInfo(const FName NewSaveSlotNameKey);
	void RemoveSlotInfo(const FName& SlotNameKey) const;
	void ClearSlotInfos() const;
	void LoadSlotInfos();
	void CreateNewSaveInstance();
	void CreateSaveInstances();
	void UpdateTimePlayed();
	void ClearTimePlayed();
	double GetElapsedTimePlayed();
};
