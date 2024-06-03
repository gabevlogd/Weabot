// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTAI_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxVolume;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Checkpoint")
	bool bDestroyOnTrigger;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Checkpoint")
	bool bIsRetriggerable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Checkpoint")
	TSubclassOf<AActor> TriggerActorClass;

private:
	bool bHasBeenTriggered;
	
public:
	ACheckpoint();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	bool CanBeTriggered();
};
