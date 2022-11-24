//===-- Sigma16SERegisterInfo.cpp - SIGMA16 Register Information ------== -------===//
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

#include "Sigma16SERegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16-reg-info"

Sigma16SERegisterInfo::Sigma16SERegisterInfo(const Sigma16Subtarget &ST)
  : Sigma16RegisterInfo(ST) {}

const TargetRegisterClass *
Sigma16SERegisterInfo::intRegClass(unsigned Size) const {
  return &Sigma16::CPURegsRegClass;
}

