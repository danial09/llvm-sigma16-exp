//===-- Sigma16SEInstrInfo.cpp - Sigma16 Instruction Information ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 implementation of the TargetInstrInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "Sigma16SEInstrInfo.h"

#include "Sigma16MachineFunction.h"
#include "Sigma16TargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

Sigma16SEInstrInfo::Sigma16SEInstrInfo(const Sigma16Subtarget &STI)
    : Sigma16InstrInfo(STI), RI(STI) {}

const Sigma16RegisterInfo &Sigma16SEInstrInfo::getRegisterInfo() const {
  return RI;
}

//@expandPostRAPseudo
/// Expand Pseudo instructions into real backend instructions
bool Sigma16SEInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  //@expandPostRAPseudo-body
  MachineBasicBlock &MBB = *MI.getParent();

  switch (MI.getDesc().getOpcode()) {
  default:
    return false;
  case Sigma16::RetLR:
    expandRetLR(MBB, MI);
    break;
  }

  MBB.erase(MI);
  return true;
}

void Sigma16SEInstrInfo::expandRetLR(MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator I) const {
  BuildMI(MBB, I, I->getDebugLoc(), get(Sigma16::RET)).addReg(Sigma16::R13);
}

const Sigma16InstrInfo *
llvm::createSigma16SEInstrInfo(const Sigma16Subtarget &STI) {
  return new Sigma16SEInstrInfo(STI);
}
