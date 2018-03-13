// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitActor.h"
#include "WolfUnit.generated.h"

/**
 * 
 */
UCLASS()
class SHEEPANDWOLF_API AWolfUnit : public AUnitActor
{
	GENERATED_BODY()
	
	

public:
	AWolfUnit();
	
	//¦Y¹¡¨S?
	bool ToEat();

};
