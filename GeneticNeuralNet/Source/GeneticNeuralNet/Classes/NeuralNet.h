// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "nnSettingsSingleton.h"
#include "NeuralNet.generated.h"


USTRUCT()
struct FNeuron
{
	GENERATED_USTRUCT_BODY()
	FNeuron(int32 N = 0) : NumInputs(N+1)
	{
		for (int32 i = 0; i < NumInputs + 1; ++i)
		{
			WeightArray.Add(FMath::FRandRange(-1.0,1.0));
		}
	}

	int32 NumInputs =0;
	
	TArray<float> WeightArray;

};

USTRUCT()
struct FNeuronLayer
{
	GENERATED_USTRUCT_BODY()
	FNeuronLayer(int32 NN = 0, int32 NI = 0) : NumNeurons(NN), NumInputsPerNeuron(NI)
	{
		for (int32 i = 0; i < NumNeurons; ++i)
		{
			NeuronArray.Add(FNeuron(NumInputsPerNeuron));
		}
	}
	int32 NumNeurons;
	int32 NumInputsPerNeuron;
	TArray<FNeuron> NeuronArray;
};

//UCLASS()
class UNeuralNet //: public UObject
{
	//GENERATED_BODY()
private:
	ENeuralNetActivationType ActivationFunction;
	float ActivationResponse;
	float Bias;
	int32 NumNetInputs;
	int32 NumNetOutputs;
	int32 NumHiddenLayers;
	int32 NeuronsPerHiddenLayer;
	TArray<FNeuronLayer> LayerArray;
public:	
	// Sets default values for this actor's properties
	UNeuralNet();

	void CreateNet();
	TArray<float> GetWeights() const;
	int32 GetNumOfWeights() const;
	void SetWeights(TArray<float> &Weights);
	void Update(TArray<float> &Inputs, TArray<float> &Outputs);
	float fSigmoid(float netinput, float response);	
	float fTanh(float netinput);
	float fMax(float netinput,float response);
};
