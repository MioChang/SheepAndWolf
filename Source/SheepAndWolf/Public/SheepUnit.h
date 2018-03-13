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

	//�O�_�O�p��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sheep")
	bool IsLittle = false;
	//�ϦY�F��ño�����
	int32 ToEatAndGetGold();

	
};
