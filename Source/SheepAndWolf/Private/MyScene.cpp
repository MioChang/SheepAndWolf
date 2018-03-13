// Fill out your copyright notice in the Description page of Project Settings.

#include "MyScene.h"
#include "PlaneForMove.h"
#include "Logging/LogMacros.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
AMyScene::AMyScene()
{

}

// Called when the game starts or when spawned
void AMyScene::BeginPlay()
{
	Super::BeginPlay();
	CreateScene();

}
//初始化遊戲參數
void AMyScene::StartGame()
{
	Turn = 0;
	Cold = 0;
	TotalLittleSheepEaten = 0;
	TotalSheepEaten = 0;
	TotalGrassEaten = 0;
	TotalLittleSheepFull = 0;
	TotalSheepFull = 0;
	TotalDogAttack = 0;
	WolfComeInterval = 0;
	SceneMap.Empty();
	if (Sheeps.Num() > 0)
	{
		for (int32 i = 0; i < Sheeps.Num(); ++i)
		{
			Sheeps[i]->Dead();
		}
	}
	Sheeps.Empty();

	if (LittleSheeps.Num() > 0)
	{
		for (int32 i = 0; i < LittleSheeps.Num(); ++i)
		{
			LittleSheeps[i]->Dead();
		}
	}
	LittleSheeps.Empty();

	if (Grasses.Num() > 0)
	{
		for (int32 i = 0; i < Grasses.Num(); ++i)
		{
			Grasses[i]->Dead();
		}
	}
	Grasses.Empty();

	if (Wolf != nullptr) Wolf->Dead();
	Wolf = nullptr;
	if (Dog != nullptr) Dog->Dead();
	Dog = nullptr;
}

void AMyScene::NextTurn()
{
	if (Turn < 100)
	{
		Turn++;
		//羊吃草
		if (Grasses.Num() > 0)
		{
			for (int32 i = 0; i < Sheeps.Num(); ++i)
			{
				ASheepUnit* sheep = Sheeps[i];
				int32 earnCold = sheep->ToEatAndGetGold();
				if (earnCold > 0)
				{
					Cold += earnCold;
					TotalSheepFull++;
				}
			}
			TArray<ASheepUnit* > queue_bigger;//要長大的小羊的索引
			for (int32 i = 0; i < LittleSheeps.Num(); ++i)
			{
				ASheepUnit* little = LittleSheeps[i];
				int32 earnCold = little->ToEatAndGetGold();
				if (earnCold > 0)
				{
					Cold += earnCold;
					TotalLittleSheepFull++;
					queue_bigger.Add(little);

				}
			}
			//有小羊長大了
			if (queue_bigger.Num() > 0)
			{
				for (int32 i = 0; i < queue_bigger.Num();++i)
				{
					ASheepUnit* little = queue_bigger[i];
					int x = little->CurrentPos;
					if (LittleSheeps.Contains(little)) LittleSheeps.Remove(little);
					little->Dead();
					if (bp_sheep != nullptr)
					{
						ASheepUnit* sheep = GetWorld()->SpawnActor<ASheepUnit>(bp_sheep, PosIndexToLocation(x), FRotator::ZeroRotator);
						sheep->UnitType = MapUnit::Sheep;
						sheep->CurrentPos = x;
						sheep->MyScene = this;
						Sheeps.Add(sheep);
						UE_LOG(LogClass, Log, TEXT("Success"));
					}

				}
			}
		}
		//生草
		int32 n_grass = 0, n_little=0;
		if (Sheeps.Num() + LittleSheeps.Num() >= 10) n_grass = 3;
		else n_grass = 5;
		while (n_grass > 0)
		{
			int32 x = FMath::RandRange(0, 399);
			if (SceneMap[x] == MapUnit::None)
			{
				SceneMap[x] = MapUnit::Grass;
				n_grass--;
				if (bp_grass != nullptr) {
					AUnitActor* grass = GetWorld()->SpawnActor<AUnitActor>(bp_grass, PosIndexToLocation(x), FRotator::ZeroRotator);
					grass->UnitType = MapUnit::Grass;
					grass->CurrentPos = x;
					grass->MyScene = this;
					Grasses.Add(grass);
					UE_LOG(LogClass, Log, TEXT("Success"));
				}
			}
		}

		//生小羊
		if (Turn % 5 == 0 && (Sheeps.Num() + LittleSheeps.Num()) < MaxSheep)
		{
			int32 n_little = 0;
			if(Sheeps.Num() + LittleSheeps.Num() > 10) n_little = 1;
			else n_little = 2;

			while (n_little > 0)
			{
				int32 x = FMath::RandRange(0, 399);
				if (SceneMap[x] == MapUnit::None)
				{
					SceneMap[x] = MapUnit::LittleSheep;
					n_little--;
					if (bp_little != nullptr) {
						ASheepUnit* little = GetWorld()->SpawnActor<ASheepUnit>(bp_little, PosIndexToLocation(x), FRotator::ZeroRotator);
						little->UnitType = MapUnit::LittleSheep;
						little->IsLittle = true;
						little->Movement = 1;
						little->CurrentPos = x;
						little->MyScene = this;
						LittleSheeps.Add(little);
						UE_LOG(LogClass, Log, TEXT("Success"));
					}
				}
			}
			
		}


		//狼開始出現
		if (Turn > 5)
		{
			//狼已在場上要吃羊
			if (Wolf != nullptr)
			{
				//狼吃飽沒
				bool isFull = Wolf->ToEat();
				if (isFull)
				{
					Wolf = nullptr;
					WolfComeInterval = FMath::RandRange(6, 10);
				}
			}
			else
			{
				//WolfComeInterval計數歸零 狼來了
				if (WolfComeInterval == 0)
				{
					
					while (Wolf == nullptr)
					{
						int32 x = FMath::RandRange(0, 399);
						if (SceneMap[x] == MapUnit::None)
						{
							SceneMap[x] = MapUnit::Wolf;

							if (bp_wolf != nullptr) {
								AWolfUnit* wolf = GetWorld()->SpawnActor<AWolfUnit>(bp_wolf, PosIndexToLocation(x), FRotator::ZeroRotator);
								wolf->UnitType = MapUnit::Wolf;
								wolf->CurrentPos = x;
								wolf->MyScene = this;
								Wolf = wolf;
								UE_LOG(LogClass, Log, TEXT("Success"));
							}
						}
					}
				}

				WolfComeInterval--;
			}

			//狗打狼
			if (Wolf != nullptr)
			{
				Dog->ToAttack();
			}

		}
		
		

	}
	else
	{
		//如果到達一百回合, 遊戲重來
		StartGame();
		CreateScene();
	}
	

}

void AMyScene::ToEndTurn()
{
	//跑到第一百個回合
	for (int32 i = Turn; i < 100; ++i)
	{
		NextTurn();
	}
	Turn = 100;

}
//建場景
void AMyScene::CreateScene()
{
	StartGame();
	int32 i,j;
	int32 CurrentSheep = InitSheep;
	int32 CurrentDog = InitDog;
	int32 CurrentGrass = InitGrass;
	//建地板和地圖
	for (i = 0;i < 20 ;++i)
	{
		for (j = 0 ; j < 20 ; ++j)
		{
			FVector location(20+i*30.0f, -180+j*30.0f  , 40.0f);
			FRotator rotator(0, 0, 0);
			//只有第一次要建地板
			if (isFirst) 
			{
				GetWorld()->SpawnActor<APlaneForMove>(APlaneForMove::StaticClass(), location, rotator);
			}
			
			SceneMap.Add(i*20 + j, MapUnit::None);
		}
	}
	isFirst = false;
	//初始羊
	while (CurrentSheep > 0)
	{
		int32 x = FMath::RandRange(0,399);
		if (SceneMap[x] == MapUnit::None)
		{
			SceneMap[x] = MapUnit::Sheep;
			CurrentSheep--;
	
			if (bp_sheep != nullptr) {

				ASheepUnit* sheep = GetWorld()->SpawnActor<ASheepUnit>(bp_sheep, PosIndexToLocation(x), FRotator::ZeroRotator);
				sheep->UnitType = MapUnit::Sheep;
				sheep->CurrentPos = x;
				sheep->MyScene = this;
				Sheeps.Add(sheep);
				UE_LOG(LogClass, Log, TEXT("Success"));
			}
		}
	}
	//初始狗
	while (CurrentDog > 0)
	{
		int32 x = FMath::RandRange(0, 399);
		if (SceneMap[x] == MapUnit::None)
		{
			SceneMap[x] = MapUnit::Dog;
			CurrentDog--;

			if (bp_dog != nullptr) {
				Dog = GetWorld()->SpawnActor<ADogUnit>(bp_dog, PosIndexToLocation(x), FRotator::ZeroRotator);
				Dog->UnitType = MapUnit::Dog;
				Dog->CurrentPos = x;
				Dog->MyScene = this;
				UE_LOG(LogClass, Log, TEXT("Success")); 
			} 
			
		}
	}
	//初始草
	while (CurrentGrass > 0)
	{
		int32 x = FMath::RandRange(0, 399);
		if (SceneMap[x] == MapUnit::None)
		{
			SceneMap[x] = MapUnit::Grass;
			CurrentGrass--;

			
			if (bp_grass != nullptr) {

				AUnitActor* grass = GetWorld()->SpawnActor<AUnitActor>(bp_grass, PosIndexToLocation(x), FRotator::ZeroRotator);
				grass->UnitType = MapUnit::Grass;
				grass->CurrentPos = x;
				grass->MyScene = this;
				Grasses.Add(grass);
				UE_LOG(LogClass, Log, TEXT("Success"));
			}
		}
	}
}

//Position Index 0~399轉換成對應的世界座標
FVector AMyScene::PosIndexToLocation(int32 PosIndex)
{
	int32 i = PosIndex / 20;
	int32 j = PosIndex % 20;
	return FVector(20 + i * 31.0f, -180 + j * 30.5f, 20.0f);
}
//攻擊者找尋目標
int32 AMyScene::FindTarget(MapUnit Target, AUnitActor* Attacker)
{
	AUnitActor* target = nullptr;
	int32 index = Attacker->CurrentPos;
	int32 Movement = Attacker->Movement;
	int32 x = index / 20;
	int32 y = index % 20;

	int a,b,dist;
	int32 Res = -1;
	int step = 400;//最小的步伐

	//吃的東西的種類
	switch (Target)
	{
	case MapUnit::None:
		break;
	case MapUnit::Sheep: // 狼吃羊
	case MapUnit::LittleSheep://狼吃小羊
		//沒羊就不動
		if (Sheeps.Num() == 0 && LittleSheeps.Num() == 0) return 0;
		for (int32 i = 0; i < Sheeps.Num();++i)
		{
			a = Sheeps[i]->CurrentPos / 20;
			b = Sheeps[i]->CurrentPos % 20;
			dist = FMath::Abs(x - a) + FMath::Abs(y - b);
			if (dist < step)
			{
				step = dist;
				Res = Sheeps[i]->CurrentPos;
				target = Sheeps[i];
			}
			
		}
		for (int32 i = 0; i < LittleSheeps.Num();++i)
		{
			a = LittleSheeps[i]->CurrentPos / 20;
			b = LittleSheeps[i]->CurrentPos % 20;
			dist = FMath::Abs(x - a) + FMath::Abs(y - b);
			if (dist < step)
			{
				step = dist;
				Res = LittleSheeps[i]->CurrentPos;
				target = LittleSheeps[i];
			}
		}
		//目標在步伐Movement內 吃掉 
		if (step <= Movement && target != nullptr)
		{
			ASheepUnit* target_sheep = Cast<ASheepUnit>(target);
			if (Sheeps.Contains(target_sheep)) {
				Sheeps.Remove(target_sheep);
				TotalSheepEaten++;
			}
			else if (LittleSheeps.Contains(target_sheep)) {
				LittleSheeps.Remove(target_sheep);
				TotalLittleSheepEaten++;
			}
		}
		break;
	case MapUnit::Wolf:
		//狗打狼
		//沒狼就不動
		if (Wolf == nullptr) return 0;
		a = Wolf->CurrentPos / 20;
		b = Wolf->CurrentPos % 20;
		dist = FMath::Abs(x - a) + FMath::Abs(y - b);
		if (dist < step)
		{
			step = dist;
			Res = Wolf->CurrentPos;
			target = Wolf;
		}
		//在步伐內就攻擊, 狼出現間隔要重新計數為亂數6~10回合
		if (step <= Movement)
		{
			Wolf->Dead();
			Wolf = nullptr;
			WolfComeInterval = FMath::RandRange(6, 10);
			TotalDogAttack++;
		}
		break;
	case MapUnit::Dog:
		break;
	case MapUnit::Grass:
		//羊吃草
		//沒草就不動
		if (Grasses.Num() == 0) return 0;
		for (int32 i = 0; i < Grasses.Num();++i)
		{
			a = Grasses[i]->CurrentPos / 20;
			b = Grasses[i]->CurrentPos % 20;
			dist = FMath::Abs(x - a) + FMath::Abs(y - b);
			if (dist < step)
			{
				step = dist;
				Res = Grasses[i]->CurrentPos;
				target = Grasses[i];
			}
		}
		//目標在步伐內吃掉
		if (step <= Movement && target != nullptr)
		{
			if (Grasses.Contains(target)) Grasses.Remove(target);
			TotalGrassEaten++;
		}
		break;
	default:
		break;
	}
	//吃or攻擊:地圖上的標記要更新,攻擊者的座標要更新
	if (step <= Movement)
	{
		target->Dead();
		SceneMap[Res] = Attacker->UnitType;
		SceneMap[Attacker->CurrentPos] = MapUnit::None;
		Attacker->CurrentPos = Res;

	}else if(Res != -1) //沒吃到僅僅接近目標
	{	
		a = Res / 20;
		b = Res % 20;
		int moved = Movement;//已走步數
		TArray<int32> queue;//合法移動地點
		while (moved > 0)
		{
			//一步一步走 將合法的目標地存起來
			if (FMath::Abs(x - a) > FMath::Abs(y - b))
			{
				if (a > x) x++;
				else x--;
			}
			else
			{
				if (b > y) y++;
				else y--;
			}
			//只要地圖沒東西即是合法目標地
			if (SceneMap[x * 20 + y] == MapUnit::None)
			{
				queue.Add(x * 20 + y);
			}
			moved--;
		}
		//至少有一個合法目標地
		if (queue.Num() > 0)
		{
			//取最新的即是移動距離
			SceneMap[queue[queue.Num()-1]] = Attacker->UnitType;
			SceneMap[Attacker->CurrentPos] = MapUnit::None;
			Attacker->CurrentPos = queue[queue.Num() - 1];
		}
	}
	
	//回傳至最近的目標的步伐數
	return step;
}

