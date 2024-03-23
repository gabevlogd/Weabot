// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Savable.h"
#include "OwnerTransformSavable.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UOwnerTransformSavable : public USavable
{
	GENERATED_BODY()

public:
	UOwnerTransformSavable();
	virtual void PushChanges_Implementation() override;

protected:
	virtual void OnSuccessfulLoad_Implementation(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData) override;
	UFUNCTION(BlueprintCallable, Category = "Save System")
	FTransform GetOwnerTransform() const;
};
