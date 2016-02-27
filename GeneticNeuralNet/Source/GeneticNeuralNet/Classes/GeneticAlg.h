// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "nnSettingsSingleton.h"
#include "GeneticAlg.generated.h"


USTRUCT()
struct FGenome
{
	GENERATED_USTRUCT_BODY()
	FGenome():Fittness(0){}
	FGenome(TArray<float> w, float f) : WeightsArray(w), Fittness(f){}
	TArray<float> WeightsArray;
	float Fittness;
	//overload for sorting
	friend bool operator<(const FGenome& lhs, const FGenome& rhs)
	{
		return (lhs.Fittness < rhs.Fittness);
	}
};


/**
 * 
 */
//UCLASS()
class GeneticAlg //: public UObject
{
	//GENERATED_UCLASS_BODY()
private:
	TArray<FGenome> PopArray; //entire population of chromos TODO: RENAME THIS!
	int32 PopulationSize;
	int32 ChromoLength;
	float TotalFitness;
	float BestFitness;
	float AvgFittness;
	float WorstFittness;
	int32 FittestGenome;
	float MutationRate;
	float CrossoverRate;
	EGeneticCrossoverMode CrossoverMode;
	EGeneticSelectionType SelectionType;
	int32 GenerationCount; //TODO remove as now in controller
	float MaxPurtubation;
	int32 NumCopiesElite;
	int32 NumElites;

	void Reset();
	void CrossOver(const TArray<float> &Mother,
		const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2);
	void Split_CrossOver(const TArray<float> &Mother,
		const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2);
	void Natural_CrossOver(const TArray<float> &Mother,
		const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2);
	void Averaging_CrossOver(const TArray<float> &Mother,
		const TArray<float> &Father, TArray<float> &baby1, TArray<float> &baby2);
	void Mutate(TArray<float> &chromo);
	FGenome GetChromoRoulette();
	void	  GrabNBest(int32	            NBest,
		const int32       NumCopies,
		TArray<FGenome>	&Pop);

	void	  CalculateBestWorstAvTot();
public:
	GeneticAlg(int32 NumWeights);
	~GeneticAlg();
	TArray<FGenome> Epoch(TArray<FGenome> &OldPop); //Runs GA for a generation
	TArray<FGenome> GetChromos()const{ return PopArray; }
	float AverageFittness()const{ return (float)TotalFitness / PopulationSize; }
	float BestFittness()const{ return BestFitness; }



};
