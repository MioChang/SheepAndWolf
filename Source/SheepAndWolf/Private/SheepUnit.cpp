// Fill out your copyright notice in the Description page of Project Settings.

#include "SheepUnit.h"
#include "MyScene.h"

ASheepUnit::ASheepUnit()
{
	Movement = 2;
}


int32 ASheepUnit::ToEatAndGetGold()
{
	int32 step = MyScene->FindTarget(MapUnit::Grass,this);
	if (step != 0)
	{
		SetActorLocation(MyScene->PosIndexToLocation(CurrentPos));
		if (step <= Movement)
		{
			NumOfAte++;
			if (NumOfAte >= 3)
			{
				NumOfAte = 0;
				if (IsLittle)
				{
					IsLittle = true;
					Movement = 2;
					return 10;
				}
				return 5;
			}
		}
		
	}
	return 0;
}
