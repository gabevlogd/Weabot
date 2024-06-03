// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Actors/Checkpoint.h"
#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	BoxVolume->SetGenerateOverlapEvents(true);
	BoxVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = BoxVolume;

	BoxVolume->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(TriggerActorClass))
	{
		if(CanBeTriggered() && UCSUtility::TrySetCurrentCheckpoint(this))
		{
			bHasBeenTriggered = true;
			UE_LOG(LogTemp, Display, TEXT("Checkpoint %s Set"), *GetName());

			if (bDestroyOnTrigger) Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Checkpoint %s Not Set"), *GetName());
		}
	}
}

bool ACheckpoint::CanBeTriggered()
{
	return !bHasBeenTriggered || bIsRetriggerable;
}
