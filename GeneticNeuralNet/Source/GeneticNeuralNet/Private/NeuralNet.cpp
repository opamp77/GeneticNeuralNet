// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneticNeuralNetPrivatePCH.h"
#include "NeuralNet.h"
#include <math.h> 


// Sets default values
UNeuralNet::UNeuralNet()
{
	ActivationFunction = nnSettingsSingleton::GetInstance().ActivationFunction;
	ActivationResponse = nnSettingsSingleton::GetInstance().ActivationResponse;
	Bias = nnSettingsSingleton::GetInstance().Bias;
	NumNetInputs = nnSettingsSingleton::GetInstance().NumInputs;
	NumNetOutputs = nnSettingsSingleton::GetInstance().NumOutputs;
	NumHiddenLayers = nnSettingsSingleton::GetInstance().NumHiddenLayers;
	NeuronsPerHiddenLayer = nnSettingsSingleton::GetInstance().NeuronsPerHiddenLayer;
	CreateNet();	
	
}

void UNeuralNet::CreateNet(void)
{
	if (NumHiddenLayers > 0)
	{
		LayerArray.Add(FNeuronLayer(NeuronsPerHiddenLayer, NumNetInputs)); //input layer
		for (int32 i = 0; i < NumHiddenLayers - 1; ++i) //hidden layers
		{
			LayerArray.Add(FNeuronLayer(NeuronsPerHiddenLayer, NeuronsPerHiddenLayer));
		}
		LayerArray.Add(FNeuronLayer(NumNetOutputs, NeuronsPerHiddenLayer)); //output layer
	} //end if
	else
	{
		LayerArray.Add(FNeuronLayer(NumNetOutputs, NumNetInputs)); //output layer
	}

}
TArray<float> UNeuralNet::GetWeights()const
{
	TArray<float> WeightsArray;
	for (int32 i = 0; i < NumHiddenLayers + 1; ++i)
	{
		for (int32 j = 0; j < LayerArray[i].NumNeurons; ++j)
		{
			for (int32 k = 0; k < LayerArray[i].NeuronArray[j].NumInputs; ++k)
			{
				WeightsArray.Add(LayerArray[i].NeuronArray[j].WeightArray[k]);
			}
		}
	}
	return WeightsArray;
}
int32 UNeuralNet::GetNumOfWeights() const
{
	int32 Weights = 0;
	for (int32 i = 0; i < NumHiddenLayers + 1; ++i)
	{
		for (int32 j = 0; j < LayerArray[i].NumNeurons; ++j)
		{
			for (int32 k = 0; k < LayerArray[i].NeuronArray[j].NumInputs; ++k)
			{
				Weights++;
			}
			
		}
	}
	return Weights;

}
void UNeuralNet::SetWeights(TArray<float> &Weights)
{
	int32 cWeight = 0;

	for (int32 i = 0; i < NumHiddenLayers + 1; ++i)
	{
		for (int32 j = 0; j < LayerArray[i].NumNeurons; ++j)
		{
			for (int32 k = 0; k < LayerArray[i].NeuronArray[j].NumInputs; ++k)
			{
				LayerArray[i].NeuronArray[j].WeightArray[k] = Weights[cWeight++];
			}
		}

	}
}

void UNeuralNet::Update(TArray<float> &Inputs, TArray<float> &Outputs)
{
	//UE_LOG(LogTemp, Warning, TEXT("begin OUTPUTS size %i"), Outputs.Num());
	TArray<float> Inputsl = Inputs;
	int32 cWeight = 0;
	if (Inputs.Num() != NumNetInputs)
	{
		UE_LOG(LogTemp, Error, TEXT("NeuralNet.CPP: inputarray size does not match NumInputs!"));
		UE_LOG(LogTemp, Error, TEXT("               inputarray size = %i  NumInputs = %i"), Inputs.Num(), NumNetInputs);
		return;
	}
	
	for (int32 i = 0; i < NumHiddenLayers + 1; ++i) //EACH LAYER
	{
		if (i>0) 
		{
			//UE_LOG(LogTemp, Warning, TEXT("before INPUTS = OUTPUTS %i"), Inputsl.Num());
			Inputsl.Empty();
			//UE_LOG(LogTemp, Warning, TEXT("OUTPUTS size %i"), Outputs.Num());
			
			Inputsl = Outputs;
			//UE_LOG(LogTemp, Warning, TEXT("INPUTS = OUTPUTS %i"),Inputsl.Num());
		}

		Outputs.Empty();
		//UE_LOG(LogTemp, Warning, TEXT("emtpyied OUTPUTS size %i"), Outputs.Num());
		cWeight = 0;
		for (int32 j = 0; j < LayerArray[i].NumNeurons; ++j) //EACH NEURON
		{
			float NetInput = 0;
			int32 InputCount = LayerArray[i].NeuronArray[j].NumInputs;

			for (int32 k = 0; k < InputCount - 1; ++k) //FOR EACH WEIGHT
			{
				NetInput += LayerArray[i].NeuronArray[j].WeightArray[k] * Inputsl[cWeight++];
			}
			NetInput += LayerArray[i].NeuronArray[j].WeightArray[InputCount - 1] * Bias; //ADD BIAS
			if (ActivationFunction == ENeuralNetActivationType::NNA_Linear)
				Outputs.Add(NetInput);
			else if (ActivationFunction == ENeuralNetActivationType::NNA_Logistic)
				Outputs.Add(fSigmoid(NetInput, ActivationResponse));
			else if (ActivationFunction == ENeuralNetActivationType::NNA_TanH)
				Outputs.Add(fTanh(NetInput));
			else if (ActivationFunction == ENeuralNetActivationType::NNA_Maximum)
				Outputs.Add(fMax(NetInput,ActivationResponse));
			cWeight = 0;
		} //end for each neuron
	} //end for each layer
}

float UNeuralNet::fSigmoid(float netinput, float response)
{
	return (1 / (1 + exp(-netinput / response))); // logistic sigmoid
}
float UNeuralNet::fTanh(float netinput)
{
	return tanhf(netinput);
}

float UNeuralNet::fMax(float netinput,float response)
{
	return fmaxf(-1.0, response*netinput);
}





