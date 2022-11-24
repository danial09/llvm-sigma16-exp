//===-- Sigma16SEInstrInfo.cpp - Sigma1632/64 Instruction Information -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma1632/64 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "Sigma16SEInstrInfo.h"

#include "Sigma16MachineFunction.h"
#include "Sigma16TargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Sigma16SEInstrInfo::Sigma16SEInstrInfo(const Sigma16Subtarget &STI)
    : Sigma16InstrInfo(STI),
      RI(STI) {}

const Sigma16RegisterInfo &Sigma16SEInstrInfo::getRegisterInfo() const {
  return RI;
}

const Sigma16InstrInfo *llvm::createSigma16SEInstrInfo(const Sigma16Subtarget &STI) {
  return new Sigma16SEInstrInfo(STI);
}

