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
    std::array<uint16_t, 4> ReservedCPURegs = {Sigma16::R0, Sigma16::R14,
                                               Sigma16::R13, Sigma16::R15
                                              };

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
    MachineInstr &MI = *II;
    MachineFunction &MF = *MI.getParent()->getParent();
    MachineFrameInfo &MFI = MF.getFrameInfo();
    Sigma16FunctionInfo *Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();

    unsigned i = FIOperandNum;
    //  while (!MI.getOperand(i).isFI()) {
    //    ++i;
    //    assert(i < MI.getNumOperands() &&
    //           "Instr doesn't have FrameIndex operand!");
    //  }

    LLVM_DEBUG(errs() << "\nFunction : " << MF.getFunction().getName() << "\n";
               errs() << "<--------->\n"
               << MI);

    int FrameIndex = MI.getOperand(i).getIndex();
    uint64_t stackSize = MF.getFrameInfo().getStackSize();
    int64_t spOffset = MF.getFrameInfo().getObjectOffset(FrameIndex);

    LLVM_DEBUG(errs() << "FrameIndex : " << FrameIndex << "\n"
               << "spOffset   : " << spOffset << "\n"
               << "stackSize  : " << stackSize << "\n");

    const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
    int MinCSFI = 0;
    int MaxCSFI = -1;

    if (CSI.size()) {
        MinCSFI = CSI[0].getFrameIdx();
        MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
    }

    // The following stack frame objects are always referenced relative to $sp:
    //  1. Outgoing arguments.
    //  2. Pointer to dynamically allocated stack space.
    //  3. Locations for callee-saved registers.
    // Everything else is referenced relative to whatever register
    // getFrameRegister() returns.
    unsigned FrameReg;

    FrameReg = getFrameRegister(MF);

    // Calculate final offset.
    // - There is no need to change the offset if the frame object is one of the
    //   following: an outgoing argument, pointer to a dynamically allocated
    //   stack space or a $gp restore location,
    // - If the frame object is any of the following, its offset must be adjusted
    //   by adding the size of the stack:
    //   incoming argument, callee-saved register location or local variable.
    int64_t Offset;
    Offset = spOffset + (int64_t)stackSize;
    bool hasImm = (MI.getNumOperands() > i + 1) && MI.getOperand(i + 1).isImm();
    if (hasImm) {
        Offset += MI.getOperand(i + 1).getImm();
    }

    LLVM_DEBUG(errs() << "Offset     : " << Offset << "\n"
               << "<--------->\n");

    // If MI is not a debug value, make sure Offset fits in the 16-bit immediate
    // field.
    if (!MI.isDebugValue() && !isInt<16>(Offset)) {
        errs() << "!!!ERROR!!! Offset is too big for 16-bit immediate field.\n";
        assert(0 && "(!MI.isDebugValue() && !isInt<16>(Offset))");
    }

    Offset = (Offset >> 1) - 6;
    MI.getOperand(i).ChangeToRegister(FrameReg, false);
    if (hasImm) {
        MI.getOperand(i + 1).ChangeToImmediate(Offset);
    }
    return false; // TODO: Is this correct?
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

Register Sigma16RegisterInfo::getStackRegister() const {
    return Sigma16::R13;
}
