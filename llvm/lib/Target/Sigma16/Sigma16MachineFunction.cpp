//===-- Sigma16MachineFunctionInfo.cpp - Private data used for Sigma16 ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Sigma16MachineFunction.h"

#include "Sigma16InstrInfo.h"
#include "Sigma16Subtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"

using namespace llvm;

bool FixGlobalBaseReg;

Sigma16FunctionInfo::~Sigma16FunctionInfo() {}

void Sigma16FunctionInfo::anchor() {}
void
test() {}


