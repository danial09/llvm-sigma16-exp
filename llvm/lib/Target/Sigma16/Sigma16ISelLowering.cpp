//===-- Sigma16ISelLowering.cpp - Sigma16 DAG Lowering Implementation -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Sigma16 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#include "Sigma16ISelLowering.h"

#include "Sigma16MachineFunction.h"
#include "Sigma16Subtarget.h"
#include "Sigma16TargetMachine.h"
#include "Sigma16TargetObjectFile.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16-lower"

//@3_1 1 {
const char *Sigma16TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case Sigma16ISD::JmpLink:
    return "Sigma16ISD::JmpLink";
  case Sigma16ISD::TailCall:
    return "Sigma16ISD::TailCall";
  case Sigma16ISD::Ret:
    return "Sigma16ISD::Ret";
  case Sigma16ISD::Wrapper:
    return "Sigma16ISD::Wrapper";
  default:
    return NULL;
  }
}
//@3_1 1 }

//@Sigma16TargetLowering {
Sigma16TargetLowering::Sigma16TargetLowering(const Sigma16TargetMachine &TM,
                                             const Sigma16Subtarget &STI)
    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {}

const Sigma16TargetLowering *
Sigma16TargetLowering::create(const Sigma16TargetMachine &TM,
                              const Sigma16Subtarget &STI) {
  return llvm::createSigma16SETargetLowering(TM, STI);
}

//===----------------------------------------------------------------------===//
//  Lower helper functions
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//

#include "Sigma16GenCallingConv.inc"

//===----------------------------------------------------------------------===//
//@            Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

//@LowerFormalArguments {
/// LowerFormalArguments - transform physical registers into virtual registers
/// and generate load operations for arguments places on the stack.
SDValue Sigma16TargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  return Chain;
}
// @LowerFormalArguments }

//===----------------------------------------------------------------------===//
//@              Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

// SDValue
// Sigma16TargetLowering::LowerReturn(SDValue Chain,
//                                 CallingConv::ID CallConv, bool IsVarArg,
//                                 const SmallVectorImpl<ISD::OutputArg> &Outs,
//                                 const SmallVectorImpl<SDValue> &OutVals,
//                                 const SDLoc &DL, SelectionDAG &DAG) const {
//   return DAG.getNode(Sigma16ISD::Ret, DL, MVT::Other,
//                      Chain, DAG.getRegister(Sigma16::LR, MVT::i32));
// }
