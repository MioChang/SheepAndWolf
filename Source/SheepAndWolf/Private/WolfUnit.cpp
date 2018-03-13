// Fill out your copyright notice in the Description page of Project Settings.

#include "WolfUnit.h"
#include "MyScene.h"



AWolfUnit::AWolfUnit()
{
	Movement = 3;
}

bool AWolfUnit::ToEat()
{
	int32 step = MyScene->FindTarget(MapUnit::Sheep, this);
	if (step != 0)
	{
		SetActorLocation(MyScene->PosIndexToLocation(CurrentPos));
		if (step <= Movement)
		{
			NumOfAte++;
			if (NumOfAte >= 3)
			{
				Destroy();
				return true;
			}
		}

	}
	return false;
}


