//===-- Sigma16ISelDAGToDAG.cpp - A dag to dag inst selector for Sigma16 --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the Sigma16 target.
//
//===----------------------------------------------------------------------===//

#include "Sigma16ISelDAGToDAG.h"
#include "Sigma16.h"

#include "Sigma16MachineFunction.h"
#include "Sigma16RegisterInfo.h"
#include "Sigma16TargetMachine.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
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

//===----------------------------------------------------------------------===//
// Instruction Selector Implementation
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Sigma16DAGToDAGISel - Sigma16 specific code to select Sigma16 machine
// instructions for SelectionDAG operations.
//===----------------------------------------------------------------------===//

bool Sigma16DAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
    bool Ret = SelectionDAGISel::runOnMachineFunction(MF);

    return Ret;
}

// SelectAddr - Complex Pattern for Load/Store Address
bool Sigma16DAGToDAGISel::SelectAddr(SDValue Addr,
                                     SDValue &Base, SDValue &Offset) {
    EVT ValTy = Addr.getValueType();
    SDLoc DL(Addr);

    // if Address is FI, get the TargetFrameIndex.
    if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
        Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
        Offset = CurDAG->getTargetConstant(0, DL, ValTy);
        return true;
    }

    if (Addr.getOpcode() == Sigma16ISD::Wrapper) {
        Base = Addr.getOperand(1);
        Offset = Addr.getOperand(0);
        return true;
    }

    //@relocation model: static
    if (TM.getRelocationModel() != Reloc::PIC_) {
        if ((Addr.getOpcode() == ISD::TargetExternalSymbol ||
                Addr.getOpcode() == ISD::TargetGlobalAddress))
            return false;
    }

    Base   = Addr;
    Offset = CurDAG->getTargetConstant(0, DL, ValTy);
    return true;
}

void Sigma16DAGToDAGISel::Select(SDNode *Node) {
    unsigned Opcode = Node->getOpcode();

    // If we have a custom node, we already have selected!
    if (Node->isMachineOpcode()) {
        LLVM_DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
        Node->setNodeId(-1);
        return;
    }

    // See if subclasses can handle this node.
    if (trySelect(Node))
        return;

    switch (Opcode) {
    default:
        break;

    // Get target GOT address.
    case ISD::GLOBAL_OFFSET_TABLE: // TODO: Handle this.
        report_fatal_error("GLOBAL_OFFSET_TABLE not implemented");
        //    ReplaceNode(Node, getGlobalBaseReg());
        return;
    }

    // Select the default instruction
    SelectCode(Node);
}
