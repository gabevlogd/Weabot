// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

#define SAVES_DIRECTORY "Saves/"
#define SAVE_SLOT_NAME "save_"
#define AUTO_SAVE_SLOT_NAME "auto_save_"
#define SLOTS_INFO_SLOT_NAME "saveinfo"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
	TSubclassOf<UDefaultSaveGame> CurrentSaveGameClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
	bool bAutoSave = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System", meta = (ClampMin = "30.0"))
	float AutoSaveInterval = 60.0f;
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
	UDefaultSaveGame* SaveGameData;
	UPROPERTY()
	FTimerHandle AutoSaveTimerHandle;
	UPROPERTY()
	bool bIsAutoSaveEnabled = true;

public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void Init(const TSubclassOf<USaveGame> SaveGameClass, const bool bUseAutoSave, const float AutoSaveIntervalSeconds);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	UDefaultSaveGame* GetSaveGame() const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void SaveAsNewSlot();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void SaveOnSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void LoadFromSlot(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void SaveOnSelectedSlot();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void LoadFromSelectedSlot();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void PauseAutoSave();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void UnPauseAutoSave();

	FTimerManager& GetWorldTimerManager() const;
	void SaveAsAutoSave();
	bool CreateFile(const FString& SlotFileName);
private:
	void OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const;
	void OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData);
};
