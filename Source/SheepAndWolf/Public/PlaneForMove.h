// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaneForMove.generated.h"

class UDecalComponent;

UCLASS()
class SHEEPANDWOLF_API APlaneForMove : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaneForMove();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDecalComponent* DecalComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
