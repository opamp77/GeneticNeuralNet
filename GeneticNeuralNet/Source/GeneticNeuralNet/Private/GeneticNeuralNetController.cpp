// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneticNeuralNetPrivatePCH.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "nnSettingsSingleton.h"
#include "GeneticNeuralNetController.h"


// Sets default values
AGeneticNeuralNetController::AGeneticNeuralNetController()
{
	
	ActivationFunction = ENeuralNetActivationType::NNA_Maximum;
	bSimulate = false;
	CrossoverMode = EGeneticCrossoverMode::GCM_Split;
	SelectionType = EGeneticSelectionType::GS_Roulette;
	ActivationFunction = ENeuralNetActivationType::NNA_Linear;
	GenerationCount = 0;
	PopulationSize = 30;
	PrimaryActorTick.bCanEverTick = true;
	TicksPerGen = 5000;
	TickCounter = 0;
	NumInputs = 1;
	NumHiddenLayers = 1;
	NeuronsPerHiddenLayer = 4;
	NumOutputs = 1;
	ActivationResponse = 1.0;
	Bias = -1.0;
	CrossoverRate = 0.6;
	MutationRate = 0.1;
	MaxPerturbation = 0.2;
	NumElite = 4;
	NumCopiesElite = 1;

}

// Called when the game starts or when spawned
void AGeneticNeuralNetController::BeginPlay()
{
	nnSettingsSingleton::GetInstance().Init(NumInputs, NumHiddenLayers, NeuronsPerHiddenLayer, NumOutputs, ActivationResponse,
		Bias, PopulationSize, TicksPerGen, CrossoverRate, MutationRate, MaxPerturbation, NumCopiesElite, NumElite,SelectionType,
		CrossoverMode,ActivationFunction);
	AvgFitnessHistory.MaxSamples = 10000;
	BestFitnessHistory.MaxSamples = 10000;
	AvgFitnessHistory.bAutoAdjustMinMax = true;
	BestFitnessHistory.bAutoAdjustMinMax = true;
	Super::BeginPlay(); //super
	
}

void AGeneticNeuralNetController::InitCreatures(void)
{
	for (int32 i = 0; i<PopulationSize; ++i) //create creatures
	{
		FVector Location = FVector::ZeroVector;
		class UWorld* w;
		w = GetWorld();
		
		AnnCreature* c = w->SpawnActor<AnnCreature>(CreatureClass);
		CreatureArray.Add(c);	
	}
	NumWeightsPerNN = CreatureArray[0]->GetNumberOfWeights();
	//init the GA
	Ga = new GeneticAlg(NumWeightsPerNN);
	ThePopulation = Ga->GetChromos();
	for (int32 i = 0; i<PopulationSize; ++i)
	{
		CreatureArray[i]->SetWeights(ThePopulation[i].WeightsArray);
	}
}


// Called every frame
void AGeneticNeuralNetController::Tick( float DeltaTime )
{
	//Super::Tick(DeltaTime);
	//CreatureArray[0]->Simulate();
	if (bSimulate && TickCounter < TicksPerGen) //simulate 1 generation
	{
		for (int i = 0; i < TicksPerGen; ++i) //creatures simulate event
		{
			for (int32 c = 0; c < CreatureArray.Num(); ++c)
			{
				CreatureArray[c]->Simulate();
				
			} //end creatures
		} //end generation
		TickCounter = TicksPerGen;
		Super::Tick(DeltaTime);
		return;
	}
	//update fitness
	for (int32 i = 0; i<CreatureArray.Num(); ++i)
	{
		ThePopulation[i].Fittness = CreatureArray[i]->GetFitness(); //update chromos fittess
	}
	TickCounter++;
	if (TickCounter>TicksPerGen)
	{
		//epoch done...
		UE_LOG(LogTemp, Warning, TEXT("Generation Complete"));
		//update stats
		float avgfitness = Ga->AverageFittness();
		float bestfitness = Ga->BestFittness();

		AvgFitnessHistory.AddSample(avgfitness);
		BestFitnessHistory.AddSample(bestfitness);
		
		if (avgfitness > MaxAvgFitness) MaxAvgFitness = avgfitness;
		if (bestfitness > MaxBestFitness) MaxBestFitness = bestfitness;

		//run the Ga
		ThePopulation = Ga->Epoch(ThePopulation);
		//update brains
		for (int32 i = 0; i < CreatureArray.Num(); ++i)
		{
			CreatureArray[i]->SetWeights(ThePopulation[i].WeightsArray);
			CreatureArray[i]->Reset(); //reset event
		}

		TickCounter = 0;
		GenerationCount++;
		UE_LOG(LogTemp, Warning, TEXT("Generation %i"), GenerationCount);
	}
	Super::Tick(DeltaTime);
	
}

AGeneticNeuralNetController::~AGeneticNeuralNetController()
{
	if (Ga) delete Ga;
}