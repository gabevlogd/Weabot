// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Saver.h"
#include "TransformSaver.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UTransformSaver : public USaver
{
	GENERATED_BODY()

public:
	UTransformSaver();
	virtual void OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData, USlotInfoItem* SlotInfoItem) override;

protected:
	virtual void OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData) override;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	FTransform GetOwnerTransform() const;

	virtual void BeginPlay() override;
	
private:
	void SetActorLoadedTransform(UDefaultSaveGame* LoadedData) const;
};
