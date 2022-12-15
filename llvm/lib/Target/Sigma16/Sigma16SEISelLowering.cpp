//= Sigma16SEISelLowering.cpp - Sigma16SE DAG Lowering Interface -----------==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of Sigma16TargetLowering specialized for sigma1632.
//
//===----------------------------------------------------------------------===//
#include "Sigma16SEISelLowering.h"
#include "Sigma16MachineFunction.h"

#include "Sigma16RegisterInfo.h"
#include "Sigma16TargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16-isel"

static cl::opt<bool>
    EnableSigma16TailCalls("enable-sigma16-tail-calls", cl::Hidden,
                           cl::desc("SIGMA16: Enable tail calls."),
                           cl::init(false));

//@Sigma16SETargetLowering {
Sigma16SETargetLowering::Sigma16SETargetLowering(const Sigma16TargetMachine &TM,
                                                 const Sigma16Subtarget &STI)
    : Sigma16TargetLowering(TM, STI) {
  //@Sigma16SETargetLowering body {
  // Set up the register classes
  addRegisterClass(MVT::i16, &Sigma16::CPURegsRegClass);

  // must, computeRegisterProperties - Once all of the register classes are
  //  added, this allows us to compute derived properties we expose.
  computeRegisterProperties(Subtarget.getRegisterInfo());
}

SDValue Sigma16SETargetLowering::LowerOperation(SDValue Op,
                                                SelectionDAG &DAG) const {

  return Sigma16TargetLowering::LowerOperation(Op, DAG);
}

const Sigma16TargetLowering *
llvm::createSigma16SETargetLowering(const Sigma16TargetMachine &TM,
                                    const Sigma16Subtarget &STI) {
  return new Sigma16SETargetLowering(TM, STI);
}
