// @(#)root/tmva $Id$
// Author: Surya S Dwivedi 26/06/2019

/*************************************************************************
 * Copyright (C) 2019, Surya S Dwivedi                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

////////////////////////////////////////////////////////////////////
// Testing LSTMLayer backpropagation                               //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <climits>
#include "TMVA/DNN/Architectures/TCudnn.h"
#include "TestLSTMBackpropagation.h"
#include "TROOT.h"

using namespace TMVA::DNN;
using namespace TMVA::DNN::RNN;

int main()
{

   std::cout << "Testing LSTM backward pass on GPU with Cudnn\n";

   using Scalar_t = Double_t;
   using Architecture_t = TCudnn<Scalar_t>;

   int seed = 12345;
   gRandom->SetSeed(seed);
   Architecture_t::SetRandomSeed(gRandom->Integer(INT_MAX));

   using Scalar_t = Double_t;

   bool debug = false;

   bool iret = false;

   // timesteps, batchsize, statesize, inputsize  { fixed input, with dense layer, with extra LSTM }
   iret |= testLSTMBackpropagation<Architecture_t>(2, 1, 2, 3, 1e-5, {true, false, false}, debug);

   iret |= testLSTMBackpropagation<Architecture_t>(1, 2, 1, 10, 1e-5, {}, debug);

   iret |= testLSTMBackpropagation<Architecture_t>(1, 2, 3, 2, 1e-5);

   iret |= testLSTMBackpropagation<Architecture_t>(2, 3, 4, 5, 1e-5);

   iret |= testLSTMBackpropagation<Architecture_t>(4, 2, 10, 5, 1e-5);

   iret |= testLSTMBackpropagation<Architecture_t>(5, 16, 10, 20, 1e-5);

   // using a fixed input (input size must be <=6, time steps <=3 and batch size <=1  )
   iret |= testLSTMBackpropagation<Architecture_t>(3, 1, 10, 5, 1e-5, {true, true}, debug);

   // test with a dense layer
   iret |= testLSTMBackpropagation<Architecture_t>(4, 8, 20, 10, 1e-5, {false, true, false}, debug);
   //iret |= testLSTMBackpropagation<Architecture_t>(4, 4, 10, 20, 1e-5, {false, true}, debug);

   // test returning the full sequence and dense layer
   iret |= testLSTMBackpropagation<Architecture_t>(3, 8, 5, 4, 1e-5, {false, true, false, true}, debug);

   // with an additional LSTM layer
   iret |= testLSTMBackpropagation<Architecture_t>(4, 16, 10, 5, 1e-5, {false, true, true}, debug);

   if (iret)
      Error("testLSTMBackPropagationCudnn", "Test failed !!!");
   else
      Info("testLSTMBackPropagationCudnn", "All tests passed !!!");

   return iret;
}
