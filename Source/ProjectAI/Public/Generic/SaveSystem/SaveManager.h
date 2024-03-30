// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DefaultSaveGame.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
	TSubclassOf<UDefaultSaveGame> SaveGameClass;
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
	
public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void Init(const TSubclassOf<USaveGame> SaveClass);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	UDefaultSaveGame* GetSaveGame() const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void Save();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	virtual void Load();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static FString GetSaveSlotName();

private:
	void OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, const bool bSuccess) const;
	void OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedData);
	void CheckFileExistence(TSubclassOf<USaveGame> SaveClass);
};
