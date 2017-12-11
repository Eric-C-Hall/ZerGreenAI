#pragma once

// This was primarily written by N00byEdge.
//
// ZerGreenOne (me as of writing this comment) did the following
//  - Added toString to each of the classes
//  - Added updateWeights to StandardLayer
//  - Added updateNeuralNetwork to ModularNN
//  - Added runUpToLayer to ModularNN
//  - Changed definition of run, and hence changed signature to take a const reference
//  - Added typedef LayerIterator
//  - Added vectorDistance
//  - Perhaps changed a few other things that I forgot to mention
//  - Perhaps forgot to update this statement
//
// Note: I used http://neuralnetworksanddeeplearning.com/chap1.html to sort of understand what I'm supposed to do to train a neural network.
// Tbh I don't fully know what I'm doing though.

#include <vector>
#include <iostream>
#include <memory>

using fv = std::vector <float>;

constexpr float I = 0.01f;

struct ModularNN {
	struct Layer;
	typedef std::vector<std::unique_ptr<ModularNN::Layer>>::iterator LayerIterator;

	// NN related definitions
	struct Layer {
		virtual ~Layer() { }
		virtual fv run(fv &input) = 0;
		virtual void write(std::ostream &) = 0;
		virtual std::string toString() = 0;
		virtual void updateWeights(fv input, fv correctOutput, LayerIterator remainingLayersBegin, LayerIterator remainingLayersEnd, float learningSpeed) = 0;
	};

	enum class ActivationFunction {
		ReLU,
		Sigmoid,
		Tanh,
		Softmax,
	};

	template <ActivationFunction actFunc>
	static void applyActivationFunction(fv &v);
	template <ActivationFunction actFunc>
	static void applyActivationFunction(float *const data, const unsigned size);

	template <ActivationFunction actFunc>
	struct StandardLayer : public Layer {
		virtual ~StandardLayer();
		StandardLayer(unsigned inputSize, unsigned outputSize);
		StandardLayer(std::istream &);
		virtual fv run(fv &input) override;
		virtual void write(std::ostream &) override;
		virtual std::string toString() override;

		void updateWeights(fv input, fv correctOutput, LayerIterator remainingLayersBegin, LayerIterator remainingLayersEnd, float learningSpeed) override;

	private:
		void genWeights();
		void writeWeights(std::ostream &);
		std::string weightsToString();
		const unsigned inputSize, outputSize;
		float *const weights;
	};

	struct LSTM : public Layer {
		LSTM(unsigned inputSize, unsigned outputSize, unsigned mSize);
		LSTM(std::istream &);
		~LSTM();
		virtual fv run(fv &input) override;
		virtual void write(std::ostream &) override;

		// The following methods have not been overridden for LSTM since I don't use them.
		virtual std::string toString() { return "Error: toString() not overridden for LSTM"; }
		virtual void updateWeights(fv input, fv correctOutput, LayerIterator remainingLayersBegin, LayerIterator remainingLayersEnd, float learningSpeed) override { throw "Error: updateWeights(fv,fv,float) not overridden for LSTM"; }
	private:
		// Sizes
		const unsigned inputSize, outputSize, mSize;

		// Weight matrices
		float *const Wmx, *const Wmh, *const Whx, *const Whm, *const Wix, *const Wim, *const Wox, *const Wom, *const Wfx, *const Wfm;

		// Biases
		float *const bm, *const bhr, *const bi, *const bo, *const bf, *const bc, *const bh;

		// State
		float *const hState, *const cState;
	};

	// ModularNN static functions
	static void mulMatrixVec(float const *const mat, float const *const vec, float *const dest, const unsigned inSize, const unsigned outSize);
	static void mulWeightsVec(float const *const weights, float const *const vec, float *const dest, const unsigned inSize, const unsigned outSize);
	static void addVectors(float *const target, float const *const source, const unsigned count);
	template <ActivationFunction>
	static void initWeightMatrix(float *const weights, unsigned inputSize, unsigned outputSize);
	template <ActivationFunction>
	static void initWeightVector(float *const weights, unsigned inputSize, unsigned outputSize);

	// ModularNN functions
	ModularNN(std::vector <std::unique_ptr<Layer>> &layers);
	ModularNN(std::istream &is);
	inline fv run(const fv &input) { return runUpToLayerExclusive(input, layers.end()); }
	void write(std::ostream &os);
	std::string toString();
	ModularNN genDivNN();

	static fv runLayers(fv input, LayerIterator begin, LayerIterator end);
	fv runUpToLayerExclusive(fv input, LayerIterator whatLayer);

	// updateNeuralNetwork: updates the neural network by a factor of learningSpeed by comparing the output of the neural network to what it should have output.
	void updateNeuralNetwork(fv input, fv correctOutput, float learningSpeed);

private:

	static float vectorDistance(fv a, fv b);
	std::unique_ptr<Layer> readLayer(std::istream &is);
	std::vector <std::unique_ptr<Layer>> layers;
};
