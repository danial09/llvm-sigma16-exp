//===-- Sigma16RegisterInfo.cpp - SIGMA16 Register Information -== --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the SIGMA16 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "sigma16-reg-info"

#include "Sigma16RegisterInfo.h"

#include "Sigma16.h"
#include "Sigma16MachineFunction.h"
#include "Sigma16Subtarget.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include <array>

#define GET_REGINFO_TARGET_DESC
#include "Sigma16GenRegisterInfo.inc"

using namespace llvm;

Sigma16RegisterInfo::Sigma16RegisterInfo(const Sigma16Subtarget &ST)
    : Sigma16GenRegisterInfo(Sigma16::R1), Subtarget(ST) {}

//===----------------------------------------------------------------------===//
// Callee Saved Registers methods
//===----------------------------------------------------------------------===//
/// Sigma16 Callee Saved Registers
// In Sigma16CallConv.td,
// def CSR_O32 : CalleeSavedRegs<(add LR, FP,
//                                   (sequence "S%u", 2, 0))>;
// llc create CSR_O32_SaveList and CSR_O32_RegMask from above defined.
const MCPhysReg *
Sigma16RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CC_Save_SaveList;
}

const uint32_t *
Sigma16RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                          CallingConv::ID) const {
  return CC_Save_RegMask;
}

// pure virtual method
//@getReservedRegs {
BitVector
Sigma16RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  //@getReservedRegs body {
  std::array<uint16_t, 3> ReservedCPURegs = {Sigma16::R0, Sigma16::R14,
                                             Sigma16::R13};

  BitVector Reserved(getNumRegs());

  for (unsigned I = 0; I < ReservedCPURegs.size(); ++I)
    Reserved.set(ReservedCPURegs[I]);

  return Reserved;
}

//@eliminateFrameIndex {
//- If no eliminateFrameIndex(), it will hang on run.
// pure virtual method
// FrameIndex represent objects inside a abstract stack.
// We must replace FrameIndex with an stack/frame pointer
// direct reference.
bool Sigma16RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  // TODO: Implement remaining cases.

  MachineInstr &MI = *II;
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();

  // Determine whether we can eliminate the frame index.
  // If not, we must spill it.
  unsigned ImmOpIdx = 0;
  switch (MI.getOpcode()) {}
  return false;
}
//}

bool Sigma16RegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return true;
}

bool Sigma16RegisterInfo::trackLivenessAfterRegAlloc(
    const MachineFunction &MF) const {
  return true;
}

// pure virtual method
Register
Sigma16RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Sigma16::R14;
  //  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  //  return TFI->hasFP(MF) ? (Sigma16::R13) : (Sigma16::R14);
}

Register Sigma16RegisterInfo::getStackRegister() const { return Sigma16::R13; }
