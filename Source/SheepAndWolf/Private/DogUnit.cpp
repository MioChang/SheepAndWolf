// Fill out your copyright notice in the Description page of Project Settings.

#include "DogUnit.h"
#include "MyScene.h"



ADogUnit::ADogUnit()
{
	Movement = 4;
}

void ADogUnit::ToAttack()
{
	int32 step = MyScene->FindTarget(MapUnit::Wolf, this);
	if (step != 0)
	{
		SetActorLocation(MyScene->PosIndexToLocation(CurrentPos));
	}
}
