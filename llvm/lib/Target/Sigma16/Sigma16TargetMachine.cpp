//===-- Sigma16TargetMachine.cpp - Define TargetMachine for Sigma16 -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Sigma16 target spec.
//
//===----------------------------------------------------------------------===//

#include "Sigma16TargetMachine.h"
#include "Sigma16.h"

#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16"

extern "C" void LLVMInitializeSigma16Target() {
}

