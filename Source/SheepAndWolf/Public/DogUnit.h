// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitActor.h"
#include "DogUnit.generated.h"

/**
 * 
 */
UCLASS()
class SHEEPANDWOLF_API ADogUnit : public AUnitActor
{
	GENERATED_BODY()
	
public:
	ADogUnit();
	
	void ToAttack();
};
