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
//��l�ƹC���Ѽ�
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
		//�ϦY��
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
			TArray<ASheepUnit* > queue_bigger;//�n���j���p�Ϫ�����
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
			//���p�Ϫ��j�F
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
		//�ͯ�
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

		//�ͤp��
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


		//�T�}�l�X�{
		if (Turn > 5)
		{
			//�T�w�b���W�n�Y��
			if (Wolf != nullptr)
			{
				//�T�Y���S
				bool isFull = Wolf->ToEat();
				if (isFull)
				{
					Wolf = nullptr;
					WolfComeInterval = FMath::RandRange(6, 10);
				}
			}
			else
			{
				//WolfComeInterval�p���k�s �T�ӤF
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

			//�����T
			if (Wolf != nullptr)
			{
				Dog->ToAttack();
			}

		}
		
		

	}
	else
	{
		//�p�G��F�@�ʦ^�X, �C������
		StartGame();
		CreateScene();
	}
	

}

void AMyScene::ToEndTurn()
{
	//�]��Ĥ@�ʭӦ^�X
	for (int32 i = Turn; i < 100; ++i)
	{
		NextTurn();
	}
	Turn = 100;

}
//�س���
void AMyScene::CreateScene()
{
	StartGame();
	int32 i,j;
	int32 CurrentSheep = InitSheep;
	int32 CurrentDog = InitDog;
	int32 CurrentGrass = InitGrass;
	//�ئa�O�M�a��
	for (i = 0;i < 20 ;++i)
	{
		for (j = 0 ; j < 20 ; ++j)
		{
			FVector location(20+i*30.0f, -180+j*30.0f  , 40.0f);
			FRotator rotator(0, 0, 0);
			//�u���Ĥ@���n�ئa�O
			if (isFirst) 
			{
				GetWorld()->SpawnActor<APlaneForMove>(APlaneForMove::StaticClass(), location, rotator);
			}
			
			SceneMap.Add(i*20 + j, MapUnit::None);
		}
	}
	isFirst = false;
	//��l��
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
	//��l��
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
	//��l��
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

//Position Index 0~399�ഫ���������@�ɮy��
FVector AMyScene::PosIndexToLocation(int32 PosIndex)
{
	int32 i = PosIndex / 20;
	int32 j = PosIndex % 20;
	return FVector(20 + i * 31.0f, -180 + j * 30.5f, 20.0f);
}
//�����̧�M�ؼ�
int32 AMyScene::FindTarget(MapUnit Target, AUnitActor* Attacker)
{
	AUnitActor* target = nullptr;
	int32 index = Attacker->CurrentPos;
	int32 Movement = Attacker->Movement;
	int32 x = index / 20;
	int32 y = index % 20;

	int a,b,dist;
	int32 Res = -1;
	int step = 400;//�̤p���B��

	//�Y���F�誺����
	switch (Target)
	{
	case MapUnit::None:
		break;
	case MapUnit::Sheep: // �T�Y��
	case MapUnit::LittleSheep://�T�Y�p��
		//�S�ϴN����
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
		//�ؼЦb�B��Movement�� �Y�� 
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
		//�����T
		//�S�T�N����
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
		//�b�B�鷺�N����, �T�X�{���j�n���s�p�Ƭ��ü�6~10�^�X
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
		//�ϦY��
		//�S��N����
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
		//�ؼЦb�B�鷺�Y��
		if (step <= Movement && target != nullptr)
		{
			if (Grasses.Contains(target)) Grasses.Remove(target);
			TotalGrassEaten++;
		}
		break;
	default:
		break;
	}
	//�Yor����:�a�ϤW���аO�n��s,�����̪��y�Эn��s
	if (step <= Movement)
	{
		target->Dead();
		SceneMap[Res] = Attacker->UnitType;
		SceneMap[Attacker->CurrentPos] = MapUnit::None;
		Attacker->CurrentPos = Res;

	}else if(Res != -1) //�S�Y��ȶȱ���ؼ�
	{	
		a = Res / 20;
		b = Res % 20;
		int moved = Movement;//�w���B��
		TArray<int32> queue;//�X�k���ʦa�I
		while (moved > 0)
		{
			//�@�B�@�B�� �N�X�k���ؼЦa�s�_��
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
			//�u�n�a�ϨS�F��Y�O�X�k�ؼЦa
			if (SceneMap[x * 20 + y] == MapUnit::None)
			{
				queue.Add(x * 20 + y);
			}
			moved--;
		}
		//�ܤ֦��@�ӦX�k�ؼЦa
		if (queue.Num() > 0)
		{
			//���̷s���Y�O���ʶZ��
			SceneMap[queue[queue.Num()-1]] = Attacker->UnitType;
			SceneMap[Attacker->CurrentPos] = MapUnit::None;
			Attacker->CurrentPos = queue[queue.Num() - 1];
		}
	}
	
	//�^�Ǧ̪ܳ񪺥ؼЪ��B���
	return step;
}

