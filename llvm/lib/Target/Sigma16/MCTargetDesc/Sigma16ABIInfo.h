//=== Sigma16ABIInfo.h - Information about SIGMA16 ABI's ----- -*- C++ -*- ===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_MCTARGETDESC_SIGMA16ABIINFO_H
#define LLVM_LIB_TARGET_SIGMA16_MCTARGETDESC_SIGMA16ABIINFO_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/Triple.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/MC/MCRegisterInfo.h"

namespace llvm {

class MCTargetOptions;
class StringRef;
class TargetRegisterClass;

class Sigma16ABIInfo {
public:
    enum class ABI { Unknown, O16, S16 };

protected:
    ABI ThisABI;

public:
    Sigma16ABIInfo(ABI ThisABI) : ThisABI(ThisABI) {}

    static Sigma16ABIInfo Unknown() {
        return Sigma16ABIInfo(ABI::Unknown);
    }
    static Sigma16ABIInfo O16() {
        return Sigma16ABIInfo(ABI::O16);
    }
    static Sigma16ABIInfo S32() {
        return Sigma16ABIInfo(ABI::S16);
    }
    static Sigma16ABIInfo computeTargetABI();

    bool IsKnown() const {
        return ThisABI != ABI::Unknown;
    }
    bool IsO16() const {
        return ThisABI == ABI::O16;
    }
    bool IsS16() const {
        return ThisABI == ABI::S16;
    }
    ABI GetEnumValue() const {
        return ThisABI;
    }

    /// The registers to use for byval arguments.
    const ArrayRef<MCPhysReg> GetByValArgRegs() const;

    /// The registers to use for the variable argument list.
    const ArrayRef<MCPhysReg> GetVarArgRegs() const;

    /// Obtain the size of the area allocated by the callee for arguments.
    /// CallingConv::FastCall affects the value for O32.
    unsigned GetCalleeAllocdArgSizeInBytes(CallingConv::ID CC) const;

    /// Ordering of ABI's
    /// Sigma16GenSubtargetInfo.inc will use this to resolve conflicts when given
    /// multiple ABI options.
    bool operator<(const Sigma16ABIInfo Other) const {
        return ThisABI < Other.GetEnumValue();
    }

    unsigned GetStackPtr() const;
    unsigned GetFramePtr() const;
    unsigned GetNullPtr() const;

    unsigned GetEhDataReg(unsigned I) const;
    int EhDataRegSize() const;
};
} // namespace llvm

#endif
