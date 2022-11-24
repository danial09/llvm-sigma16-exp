//===-- Sigma16TargetInfo.cpp - Sigma16 Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Sigma16.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target llvm::TheSigma16Target;

extern "C" void LLVMInitializeSigma16TargetInfo() {
  RegisterTarget<Triple::sigma16> X(TheSigma16Target, "sigma16", "The Sigma16 Architecture", "Sigma16");
}
