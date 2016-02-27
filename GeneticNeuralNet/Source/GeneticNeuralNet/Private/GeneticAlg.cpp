// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneticNeuralNetPrivatePCH.h"
#include "GeneticAlg.h"


GeneticAlg::GeneticAlg(int32 NumWeights)
{
	CrossoverMode = nnSettingsSingleton::GetInstance().CrossoverMode;
	SelectionType = nnSettingsSingleton::GetInstance().SelectionType;
	MaxPurtubation = nnSettingsSingleton::GetInstance().MaxPerturbation;
	NumCopiesElite = nnSettingsSingleton::GetInstance().NumCopiesElite;
	NumElites = nnSettingsSingleton::GetInstance().NumElite;
	PopulationSize = nnSettingsSingleton::GetInstance().PopulationSize;
	MutationRate = nnSettingsSingleton::GetInstance().MutationRate;
	CrossoverRate = nnSettingsSingleton::GetInstance().CrossoverRate;
	ChromoLength = NumWeights;
	TotalFitness = 0.0;
	GenerationCount = 0;
	FittestGenome = 0;
	BestFitness = 0.0;
	WorstFittness = 99999999.0;
	AvgFittness = 0;
	// init population
	for (int32 i = 0; i<PopulationSize; ++i)
	{
		PopArray.Add(FGenome());
		for (int32 j = 0; j < ChromoLength; ++j)
		{
			PopArray[i].WeightsArray.Add(FMath::FRandRange(-1.0,1.0));
		}
	}	
}

void GeneticAlg::Mutate(TArray<float> &chromo)
{
	for (int32 i = 0; i < chromo.Num(); ++i)
	{
		if (FMath::FRand() < MutationRate)
		{
				chromo[i] += (FMath::FRandRange(-1.0,1.0) * MaxPurtubation);
		}
	}
}

FGenome GeneticAlg::GetChromoRoulette(void)
{
	float Slice = (float)(FMath::FRand() * TotalFitness);
	FGenome TheChosenOne;
	float FittnessSoFar = 0;
	for (int32 i = 0; i < PopulationSize; ++i)
	{
		FittnessSoFar += PopArray[i].Fittness;
		if (FittnessSoFar >= Slice)
		{
			TheChosenOne = PopArray[i];
			break;
		}
	}
	return TheChosenOne;
}

void GeneticAlg::CrossOver(const TArray<float> &Mother,
	const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2)
{
	if ((FMath::FRand() > CrossoverRate) || (Mother == Father))
	{
		baby1 = Mother;
		baby2 = Father;
		if (Mother == Father)
		{
			UE_LOG(LogTemp, Warning, TEXT("Crossover Mother = Farther"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("No Crossover carried out"));
		return;
	}
	if (CrossoverMode == EGeneticCrossoverMode::GCM_Split)
		Split_CrossOver(Mother, Father, baby1, baby2);
	else if (CrossoverMode == EGeneticCrossoverMode::GCM_Natural)
		Natural_CrossOver(Mother, Father, baby1, baby2);
	else if (CrossoverMode == EGeneticCrossoverMode::GCM_Average)
		Averaging_CrossOver(Mother, Father, baby1, baby2);
}

void GeneticAlg::Split_CrossOver(const TArray<float> &Mother,
	const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2)
{
	int32 cp = FMath::RandRange(0, ChromoLength - 1);
	for (int32 i = 0; i < cp; ++i)
	{
		baby1.Add(Mother[i]);
		baby2.Add(Father[i]);
	}
	for (int32 i = cp; i < Mother.Num(); ++i)
	{
		baby1.Add(Father[i]);
		baby2.Add(Mother[i]);
	}
	return;

}

void GeneticAlg::Natural_CrossOver(const TArray<float> &Mother,
	const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2)
{
	int32 cp = Mother.Num();
	for (int32 i = 0; i < cp; ++i)
	{
		if (FMath::FRand() <= 0.5) baby1.Add(Mother[i]);
		else baby1.Add(Father[i]);
		if (FMath::FRand() <= 0.5) baby2.Add(Mother[i]);
		else baby2.Add(Father[i]);
	}
}
void GeneticAlg::Averaging_CrossOver(const TArray<float> &Mother,
	const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2)
{
	//int32 cp = ChromoLength - 1;
	int32 cp = Mother.Num();
	for (int32 i = 0; i < cp; ++i)
	{
		baby1.Add((Father[i] + Mother[i]) / 2);
		baby2.Add((Father[i] + Mother[i]) / 2);
	}
}

TArray<FGenome> GeneticAlg::Epoch(TArray<FGenome> &OldPop)
{
	PopArray = OldPop;
	Reset();
	PopArray.Sort(); //sorts Fgenomes by fitness
	CalculateBestWorstAvTot();
	TArray<FGenome> NewPop;
	if (!(NumCopiesElite * NumElites % 2))
	{
		GrabNBest(NumElites, NumCopiesElite, NewPop);
	}
	while (NewPop.Num() < PopulationSize)
	{
		//grab two chromosones
		FGenome mum = GetChromoRoulette();
		FGenome dad = GetChromoRoulette();

		//create some offspring via crossover
		TArray<float>		baby1, baby2;

		CrossOver(mum.WeightsArray, dad.WeightsArray, baby1, baby2);

		//now we mutate
		Mutate(baby1);
		Mutate(baby2);

		//now copy into vecNewPop population
		NewPop.Add(FGenome(baby1, 0));
		NewPop.Add(FGenome(baby2, 0));
	}

	//finished so assign new pop back into m_vecPop
	PopArray = NewPop;

	return PopArray;
	
}

void GeneticAlg::Reset(void)
{
	TotalFitness = 0.0;
	BestFitness = 0.0;
	WorstFittness = 9999999.0;
	AvgFittness = 0.0;
}

void GeneticAlg::GrabNBest(int32 NBest, const int32 NumCopies, TArray<FGenome>	&Pop)
{
	//add the required amount of copies of the n most fittest 
	//to the supplied vector
	while (NBest--)
	{
		for (int32 i = 0; i<NumCopies; ++i)
		{
			Pop.Add(PopArray[(PopulationSize - 1) - NBest]);
		}
	}
}

void GeneticAlg::CalculateBestWorstAvTot()
{
	TotalFitness = 0;

	float HighestSoFar = 0.0;
	float LowestSoFar = 9999999.0;

	for (int32 i = 0; i<PopulationSize; ++i)
	{
		//update fittest if necessary
		if (PopArray[i].Fittness > HighestSoFar)
		{
			HighestSoFar = PopArray[i].Fittness;
			
			FittestGenome = i;

			BestFitness = HighestSoFar;
		}

		//update worst if necessary
		if (PopArray[i].Fittness < LowestSoFar)
		{
			LowestSoFar = PopArray[i].Fittness;

			WorstFittness = LowestSoFar;
		}

		TotalFitness += PopArray[i].Fittness;


	}//next chromo

	AvgFittness = TotalFitness / PopulationSize;
}

GeneticAlg::~GeneticAlg()
{
}
