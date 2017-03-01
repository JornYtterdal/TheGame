// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Attack.generated.h"

UCLASS()
class THEGAME_API AAttack : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAttack();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

private:
    UPROPERTY(EditAnywhere)
    float Speed = 400.f;

    UPROPERTY(EditAnywhere)
    float TimeBeforeDestroy = 5.f;

    float TimeLived{0};

    UPROPERTY(EditAnywhere)
    UShapeComponent* RootSphere = nullptr;


    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
                   UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
                   bool bFromSweep, const FHitResult &SweepResult);

};
