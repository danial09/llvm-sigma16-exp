//===---- Sigma16ABIInfo.cpp - Information about SIGMA16 ABI's ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Sigma16ABIInfo.h"
#include "Sigma16RegisterInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

static cl::opt<bool> EnableSigma16S16Calls(
    "sigma16-s16-calls", cl::Hidden,
    cl::desc("SIGMA16 S16 call: use stack only to pass arguments.\
                    "),
    cl::init(false));

namespace {
static const MCPhysReg O16IntRegs[4] = {Sigma16::R1, Sigma16::R2, Sigma16::R3,
                                        Sigma16::R4
                                       };
static const MCPhysReg S16IntRegs = {};
} // namespace

const ArrayRef<MCPhysReg> Sigma16ABIInfo::GetByValArgRegs() const {
    if (IsO16())
        return makeArrayRef(O16IntRegs);
    if (IsS16())
        return makeArrayRef(S16IntRegs);
    llvm_unreachable("Unhandled ABI");
}

const ArrayRef<MCPhysReg> Sigma16ABIInfo::GetVarArgRegs() const {
    if (IsO16())
        return makeArrayRef(O16IntRegs);
    if (IsS16())
        return makeArrayRef(S16IntRegs);
    llvm_unreachable("Unhandled ABI");
}

unsigned
Sigma16ABIInfo::GetCalleeAllocdArgSizeInBytes(CallingConv::ID CC) const {
    if (IsO16())
        return CC != 0;
    if (IsS16())
        return 0;
    llvm_unreachable("Unhandled ABI");
}

Sigma16ABIInfo Sigma16ABIInfo::computeTargetABI() {
    Sigma16ABIInfo Abi(ABI::S16);

    if (EnableSigma16S16Calls)
        Abi = ABI::S16;
    else
        Abi = ABI::O16;
    // Assert exactly one ABI was chosen.
    assert(Abi.ThisABI != ABI::Unknown);

    return Abi;
}

unsigned Sigma16ABIInfo::GetStackPtr() const {
    return Sigma16::R14;
}

unsigned Sigma16ABIInfo::GetFramePtr() const {
    return Sigma16::R13;
}

unsigned Sigma16ABIInfo::GetNullPtr() const {
    return Sigma16::R0;
}

unsigned Sigma16ABIInfo::GetEhDataReg(unsigned I) const {
    static const unsigned EhDataReg[] = {Sigma16::R1, Sigma16::R2};

    return EhDataReg[I];
}

int Sigma16ABIInfo::EhDataRegSize() const {
    if (ThisABI == ABI::S16)
        return 0;
    return 2;
}
