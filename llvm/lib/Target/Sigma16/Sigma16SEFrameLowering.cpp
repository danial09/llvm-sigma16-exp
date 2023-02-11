//===-- Sigma16SEFrameLowering.cpp - Sigma16 Frame Information ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "Sigma16SEFrameLowering.h"

#include "Sigma16MachineFunction.h"
#include "Sigma16SEInstrInfo.h"
#include "Sigma16Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

Sigma16SEFrameLowering::Sigma16SEFrameLowering(const Sigma16Subtarget &STI)
    : Sigma16FrameLowering(STI, STI.stackAlignment()) {}

//@emitPrologue {
void Sigma16SEFrameLowering::emitPrologue(MachineFunction &MF,
        MachineBasicBlock &MBB) const {
    MachineFrameInfo &MFI = MF.getFrameInfo();
    Sigma16FunctionInfo *Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();

    const Sigma16SEInstrInfo &TII =
        *static_cast<const Sigma16SEInstrInfo *>(STI.getInstrInfo());
    const Sigma16RegisterInfo &RegInfo =
        *static_cast<const Sigma16RegisterInfo *>(STI.getRegisterInfo());

    MachineBasicBlock::iterator MBBI = MBB.begin();
    DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
    Sigma16ABIInfo ABI = STI.getABI();

    // First, compute final stack size.
    uint64_t StackSize = MFI.getStackSize();

    // No need to allocate space on the stack.
    if (StackSize == 0 && !MFI.adjustsStack())
        return;

    unsigned SP = RegInfo.getStackRegister();   // R13
    unsigned FP = RegInfo.getFrameRegister(MF); // R14
    const TargetRegisterClass *RC = &Sigma16::CPURegsRegClass;

    // Store the old stack pointer in the stack frame.
    // store R13,1[R14]
    BuildMI(MBB, MBBI, dl, TII.get(Sigma16::STIMMDISP))
    .addReg(Sigma16::R13)
    .addImm(1)
    .addReg(FP);
}

//@emitEpilogue {
void Sigma16SEFrameLowering::emitEpilogue(MachineFunction &MF,
        MachineBasicBlock &MBB) const {
    // Restore the callee-saved registers, and restore the previous stack pointer.
    // R1 contains the return value, so it is not restored here.
    // Then pop the stack frame.

    MachineBasicBlock::iterator MBBI = MBB.getFirstTerminator();
    MachineFrameInfo &MFI = MF.getFrameInfo();
    Sigma16FunctionInfo *Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();

    const Sigma16SEInstrInfo &TII =
        *static_cast<const Sigma16SEInstrInfo *>(STI.getInstrInfo());
    const Sigma16RegisterInfo &RegInfo =
        *static_cast<const Sigma16RegisterInfo *>(STI.getRegisterInfo());

    DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
    Sigma16ABIInfo ABI = STI.getABI();

    uint64_t StackSize = MFI.getStackSize();

    if (!StackSize && !MFI.adjustsStack())
        return;

    Register SP = RegInfo.getStackRegister();   // R13
    Register FP = RegInfo.getFrameRegister(MF); // R14

    // Restore the previous stack pointer.
    // load R13,1[R14]
    BuildMI(MBB, MBBI, DL, TII.get(Sigma16::LDIMMDISP), SP).addImm(1).addReg(FP);

    // Pop the stack frame.
    // load R14,0[R14]
    BuildMI(MBB, MBBI, DL, TII.get(Sigma16::LDIMMDISP), FP).addImm(0).addReg(FP);
}

bool
Sigma16SEFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
    const MachineFrameInfo &MFI = MF.getFrameInfo();

    // Reserve call frame if the size of the maximum call frame fits into 16-bit
    // immediate field and there are no variable sized objects on the stack.
    // Make sure the second register scavenger spill slot can be accessed with one
    // instruction.
    return isInt<16>(MFI.getMaxCallFrameSize() + getStackAlignment()) &&
           !MFI.hasVarSizedObjects();
}

static void setAliasRegs(MachineFunction &MF, BitVector &SavedRegs, unsigned Reg) {
    const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
    for (MCRegAliasIterator AI(Reg, TRI, true); AI.isValid(); ++AI)
        SavedRegs.set(*AI);
}

void Sigma16SEFrameLowering::determineCalleeSaves(MachineFunction &MF,
        BitVector &SavedRegs,
        RegScavenger *RS) const {
    //@determineCalleeSaves-body
    TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
    Sigma16FunctionInfo *Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();

    if (MF.getFrameInfo().hasCalls())
        setAliasRegs(MF, SavedRegs, Sigma16::R1);

    return;
}

const Sigma16FrameLowering *
llvm::createSigma16SEFrameLowering(const Sigma16Subtarget &ST) {
    return new Sigma16SEFrameLowering(ST);
}
