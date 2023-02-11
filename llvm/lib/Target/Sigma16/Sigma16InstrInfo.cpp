//===-- Sigma16InstrInfo.cpp - Sigma16 Instruction Information ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "Sigma16InstrInfo.h"

#include "Sigma16MachineFunction.h"
#include "Sigma16TargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "Sigma16GenInstrInfo.inc"

// Pin the vtable to this file.
void Sigma16InstrInfo::anchor() {}

//@Sigma16InstrInfo {
Sigma16InstrInfo::Sigma16InstrInfo(const Sigma16Subtarget &STI)
    : Subtarget(STI) {}

const Sigma16InstrInfo *Sigma16InstrInfo::create(Sigma16Subtarget &STI) {
    return llvm::createSigma16SEInstrInfo(STI);
}

//@GetInstSizeInBytes {
/// Return the number of bytes of code the specified instruction may be.
unsigned Sigma16InstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
    //@GetInstSizeInBytes - body
    switch (MI.getOpcode()) {
    default:
        return MI.getDesc().getSize();
    }
}

MachineMemOperand *
Sigma16InstrInfo::GetMemOperand(MachineBasicBlock &MBB, int FI,
                                MachineMemOperand::Flags Flags) const {
    MachineFunction &MF = *MBB.getParent();
    MachineFrameInfo &MFI = MF.getFrameInfo();
    const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
    return MF.getMachineMemOperand(MachinePointerInfo::getFixedStack(MF, FI),
                                   Flags, MFI.getObjectSize(FI),
                                   MFI.getObjectAlign(FI));
}

void Sigma16InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   const DebugLoc &DL, MCRegister DestReg,
                                   MCRegister SrcReg, bool KillSrc) const {
    BuildMI(MBB, MI, DL, get(Sigma16::ADD), DestReg)
    .addReg(SrcReg, getKillRegState(KillSrc))
    .addReg(Sigma16::R0);
}

void Sigma16InstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI, Register SrcReg,
    bool isKill, int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const {
    DebugLoc DL;
    //  MachineMemOperand *MMO =
    //      GetMemOperand(MBB, FrameIndex, MachineMemOperand::MOStore);

    unsigned Opc = 0;

    Opc = Sigma16::STIMMDISP;
    assert(Opc && "Register class not handled!");
    BuildMI(MBB, MBBI, DL, get(Opc))
    .addReg(Sigma16::R13)
    .addImm(FrameIndex)
    .addReg(SrcReg, getKillRegState(isKill));
}

void Sigma16InstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI, Register DestReg,
    int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const {
    DebugLoc DL;
    if (MBBI != MBB.end())
        DL = MBBI->getDebugLoc();
    //  MachineMemOperand *MMO = GetMemOperand(MBB, FI,
    //  MachineMemOperand::MOLoad);
    unsigned Opc = 0;
    Opc = Sigma16::LDIMMDISP;
    assert(Opc && "Register class not handled!");
    BuildMI(MBB, MBBI, DL, get(Opc), DestReg)
    .addImm(FrameIndex)
    .addReg(Sigma16::R14);
}