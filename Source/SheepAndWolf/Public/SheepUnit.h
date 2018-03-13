// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitActor.h"
#include "SheepUnit.generated.h"


/**
 * 
 */
UCLASS()
class SHEEPANDWOLF_API ASheepUnit : public AUnitActor
{
	GENERATED_BODY()
	
public:
	ASheepUnit();

	//是否是小羊
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sheep")
	bool IsLittle = false;
	//羊吃東西並得到金幣
	int32 ToEatAndGetGold();

	
};
