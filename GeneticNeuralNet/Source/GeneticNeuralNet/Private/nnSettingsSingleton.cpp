// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneticNeuralNetPrivatePCH.h"
#include "nnSettingsSingleton.h"


nnSettingsSingleton::nnSettingsSingleton()
{
	//Neural Network Settings
	 NumInputs = 0;
	 NumHiddenLayers = 0;
	 NeuronsPerHiddenLayer = 0;
	 NumOutputs = 0;
	 ActivationResponse = 0.0;
	 Bias = 0.0;
	//GeneticAlg Parameters
	 PopulationSize = 0;
	 TicksPerGen = 0;
	 CrossoverRate = 0.0;
	 MutationRate = 0.0;
	 MaxPerturbation = 0.0; //the max amount a weight may be mutated by
	 NumCopiesElite = 0;
	 NumElite = 0;

}

void nnSettingsSingleton::Init(int32 iNumInputs,
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
	int32 iNumElite,EGeneticSelectionType iSelectionType,EGeneticCrossoverMode iCrossoverMode,ENeuralNetActivationType iActivationFunction)
{
	ActivationFunction = iActivationFunction;
	NumInputs= iNumInputs;
	NumHiddenLayers= iNumHiddenLayers;
	NeuronsPerHiddenLayer= iNeuronsPerHiddenLayer;
	NumOutputs= iNumOutputs;
	ActivationResponse= iActivationResponse;
	Bias= iBias;
	PopulationSize= iPopulationSize;
	TicksPerGen= iTicksPerGen;
	CrossoverRate= iCrossoverRate;
	MutationRate= iMutationRate;
	MaxPerturbation= iMaxPerturbation;
	NumCopiesElite= iNumCopiesElite;
	NumElite= iNumElite;
}
nnSettingsSingleton::~nnSettingsSingleton()
{
}

