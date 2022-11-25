//=- Sigma16.h - Top-level interface for Sigma16 representation -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM Sigma16 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16_H

#include "MCTargetDesc/Sigma16MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class Sigma16TargetMachine;
class FunctionPass;
} // end namespace llvm

#endif // LLVM_LIB_TARGET_SIGMA16_SIGMA16_H