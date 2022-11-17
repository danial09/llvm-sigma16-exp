//===-- Sigma16MCTargetDesc.h - Sigma16 Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Sigma16 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Sigma16_MCTARGETDESC_Sigma16MCTARGETDESC_H
#define LLVM_LIB_TARGET_Sigma16_MCTARGETDESC_Sigma16MCTARGETDESC_H

#include "Sigma16Config.h"
#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;
class Triple;

extern Target TheSigma16Target;
} // End llvm namespace

// Defines symbolic names for Sigma16 registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "Sigma16GenRegisterInfo.inc"

// Defines symbolic names for the Sigma16 instructions.
#define GET_INSTRINFO_ENUM
#include "Sigma16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "Sigma16GenSubtargetInfo.inc"

#endif

