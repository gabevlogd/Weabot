// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

#define SAVE_EXTENSION ".sav"
#define SAVES_DIRECTORY "Saves/"
#define SAVES_DIRECTORY_FULLPATH FPaths::ProjectSavedDir() + TEXT("SaveGames/" + SAVES_DIRECTORY)
#define SAVE_SLOT_NAME "save_"
#define AUTO_SAVE_SLOT_NAME "auto_save_"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System", meta = (ClampMin = "1"))
	int MaxAutoSaves = 3;
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
	void Init(const TSubclassOf<USaveGame> SaveGameClass, const bool bUseAutoSave, const float AutoSaveIntervalSeconds, const int MaxAutoSavesNumber);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	UDefaultSaveGame* GetSaveGame() const;

	UFUNCTION(BlueprintPure, Category = "Save System")
	TArray<UDefaultSaveGame*> GetAllSaveGames();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveAsNewSlot();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveOnSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadFromSlot(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveOnSelectedSlot();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadFromSelectedSlot();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void PauseAutoSave();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void UnPauseAutoSave();

	void SaveAsAutoSave();
	bool CreateFile(const FString& SlotFileName);
private:
	void OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const;
	void OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData);
	FTimerManager& GetWorldTimerManager() const;
};
