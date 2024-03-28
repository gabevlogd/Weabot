// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Generic/SaveSystem/GenericSaveGame.h"
#include "Generic/SaveSystem/SaveManager.h"
#include "Saver.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API USaver : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnPrepareSave OnPrepSave;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnPrepareLoad OnPrepLoad;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnSaveGame OnSaveGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnLoadGame OnLoadGameCompleted;

private:
	FName UniqueGUID;
	
public:
	USaver();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	FName GetUniqueNameID() const;
	
protected:
	UFUNCTION()
	void PrepareSave(UGenericSaveGame* SaveGameData);
	
	UFUNCTION()
	void PrepareLoad(UGenericSaveGame* SaveGameData);

	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void OnPrepareSave(UGenericSaveGame* SaveGameData);

	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void OnPrepareLoad(UGenericSaveGame* SaveGameData);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void OnSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UGenericSaveGame* SaveGameData);

	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void OnLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
