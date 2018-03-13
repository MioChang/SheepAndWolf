// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaneForMove.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstance.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APlaneForMove::APlaneForMove()
{
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	
 	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterialInstance(TEXT("/Game/Materials/SpawnBox_M_Inst"));
	if (FoundMaterialInstance.Object != NULL)
	{
		DecalComp->SetMaterial(0, Cast<UMaterialInterface>(FoundMaterialInstance.Object));
 	}
	DecalComp->SetWorldScale3D(FVector(0.1f,0.05f,0.05f));
	DecalComp->AddWorldRotation(FRotator(90,0,0));
	RootComponent = DecalComp;

}

void APlaneForMove::BeginPlay()
{
	Super::BeginPlay();
}


