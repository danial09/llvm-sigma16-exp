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
