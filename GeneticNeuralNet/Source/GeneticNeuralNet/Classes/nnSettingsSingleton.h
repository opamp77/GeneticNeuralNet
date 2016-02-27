// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "nnSettingsSingleton.generated.h"

UENUM(BlueprintType)
enum class EGeneticSelectionType : uint8
{
	GS_Roulette 	UMETA(DisplayName = "Roulette"),
	GS_Best 		UMETA(DisplayName = "Best")
};

UENUM(BlueprintType)
enum class EGeneticCrossoverMode : uint8
{
	GCM_Split 	UMETA(DisplayName = "Split"),
	GCM_Natural	UMETA(DisplayName = "Natural"),
	GCM_Average UMETA(DisplayName = "Average")
};

UENUM(BlueprintType)
enum class ENeuralNetActivationType : uint8
{
	NNA_Linear		UMETA(DisplayName = "Linear"),
	NNA_TanH		UMETA(DisplayName = "Tanh"),
	NNA_Logistic	UMETA(DisplayName = "Logistic"),
	NNA_Maximum			UMETA(DisplayName = "Max(-1,x)")
	
};



class nnSettingsSingleton 
{
private:
	nnSettingsSingleton(); //constructor
	nnSettingsSingleton(nnSettingsSingleton const&) = delete;
	void operator=(nnSettingsSingleton const&) = delete;
public:
	~nnSettingsSingleton();
	static nnSettingsSingleton& GetInstance()
	{
		static nnSettingsSingleton instance;
		return instance;
	}

	void Init(int32 iNumInputs,
	int32 iNumHiddenLayers,
	int32 iNeuronsPerHiddenLayer,
	int32 iNumOutputs,
	float iActivationResponse,
	float iBias,
	int32 iPopulationSize,
	int32 iTicksPerGen,
	float iCrossoverRate,
	float iMutationRate,
	float iMaxPerturbation,
	int32 iNumCopiesElite,
	int32 iNumElite,
	EGeneticSelectionType iSelectionType,
	EGeneticCrossoverMode iCrossoverMode, ENeuralNetActivationType  iActivationFunction);
	

//Neural Network Settings
	ENeuralNetActivationType ActivationFunction;
	int32 NumInputs;
	int32 NumHiddenLayers;
	int32 NeuronsPerHiddenLayer;
	int32 NumOutputs;
	float ActivationResponse;
	float Bias;
//GeneticAlg Parameters
	EGeneticSelectionType SelectionType;
	EGeneticCrossoverMode CrossoverMode;
	int32 PopulationSize;
	int32 TicksPerGen;
	float CrossoverRate;
	float MutationRate;
	float MaxPerturbation; //the max amount a weight may be mutated by
	int32 NumCopiesElite;
	int32 NumElite;
};

