// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Generic/SaveSystem/GenericSaveGame.h"
#include "Generic/SaveSystem/SaveManager.h"
#include "Savable.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API USavable : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnLoadGame OnGameLoaded;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnSaveGame OnGameSaved;

private:
	FName UniqueGUID;
	
public:
	USavable();

	/**
	 * Pushes this object's data to the save game object.
	 * Also called when the Save Game function is called.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Save System")
	void PushChanges();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void OnSuccessfulSave(const FString& SlotName, const int32 UserIndex, bool bSuccess, UGenericSaveGame* SaveGameData);

	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void OnSuccessfulLoad(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	FName GetUniqueNameID() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void CreateGUID();
};
