//===-- Sigma16SEISelDAGToDAG.cpp - A Dag to Dag Inst Selector for Sigma16SE
//----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of Sigma16DAGToDAGISel specialized for sigma1632.
//
//===----------------------------------------------------------------------===//

#include "Sigma16SEISelDAGToDAG.h"

#include "MCTargetDesc/Sigma16BaseInfo.h"
#include "Sigma16.h"
#include "Sigma16MachineFunction.h"
#include "Sigma16RegisterInfo.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
using namespace llvm;

#define DEBUG_TYPE "sigma16-isel"

bool Sigma16SEDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &static_cast<const Sigma16Subtarget &>(MF.getSubtarget());
  return Sigma16DAGToDAGISel::runOnMachineFunction(MF);
}

void Sigma16SEDAGToDAGISel::processFunctionAfterISel(MachineFunction &MF) {}

//@selectNode
bool Sigma16SEDAGToDAGISel::trySelect(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();
  SDLoc DL(Node);

  ///
  // Instruction Selection not handled by the auto-generated
  // tablegen selection should be handled here.
  ///

  ///
  // Instruction Selection not handled by the auto-generated
  // tablegen selection should be handled here.
  ///
  EVT NodeTy = Node->getValueType(0);
  unsigned MultOpc;

  //  switch(Opcode) {
  //  default: break;
  //
  //  }

  return false;
}

FunctionPass *llvm::createSigma16SEISelDag(Sigma16TargetMachine &TM,
                                           CodeGenOpt::Level OptLevel) {
  return new Sigma16SEDAGToDAGISel(TM, OptLevel);
}
