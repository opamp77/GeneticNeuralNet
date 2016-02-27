// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneticNeuralNetPrivatePCH.h"
#include "nnSettingsSingleton.h"
#include "nnCreature.h"


// Sets default values
AnnCreature::AnnCreature()
{
	Brain = new UNeuralNet();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Fitness = 0.0; //Creatures Fitness

}

// Called when the game starts or when spawned
void AnnCreature::BeginPlay()
{
	InputArray.SetNum(nnSettingsSingleton::GetInstance().NumInputs);
	OutputArray.SetNum(nnSettingsSingleton::GetInstance().NumOutputs);
	Super::BeginPlay();
	
}

// Called every frame
void AnnCreature::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AnnCreature::SetWeights(TArray<float> &Weights)
{
	Brain->SetWeights(Weights);
}
int32 AnnCreature::GetNumberOfWeights()const
{ 
	return Brain->GetNumOfWeights(); 
}
void AnnCreature::SetFitness(float f)
{
	Fitness = f;
}
float AnnCreature::GetFitness(void)
{
	return (float)Fitness;
}
void AnnCreature::Think(void)
{
	Brain->Update(InputArray,OutputArray);
}

AnnCreature::~AnnCreature()
{
	if (Brain) delete Brain;
}

void AnnCreature::Reset_Implementation(void)
{
	Fitness = 0;
}
void AnnCreature::Simulate_Implementation(void)
{
	UE_LOG(LogTemp, Warning, TEXT("nn_Creature simulate_implementation"));
}


