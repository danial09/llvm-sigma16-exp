//===--- Sigma16.cpp - Implement Sigma16 target feature support
//-------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements Sigma16 TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "Sigma16.h"
#include "clang/Basic/MacroBuilder.h"

using namespace clang;
using namespace clang::targets;

const char *const Sigma16TargetInfo::GCCRegNames[] = {
    "R0", "R1", "R2",  "R3",  "R4",  "R5",  "R6",  "R7",
    "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};

ArrayRef<const char *> Sigma16TargetInfo::getGCCRegNames() const {
  return llvm::makeArrayRef(GCCRegNames);
}

void Sigma16TargetInfo::getTargetDefines(const LangOptions &Opts,
                                         MacroBuilder &Builder) const {
  Builder.defineMacro("sigma16");
  Builder.defineMacro("__sigma16__");
  Builder.defineMacro("__ELF__");
  // FIXME: defines for different 'flavours' of MCU
}
