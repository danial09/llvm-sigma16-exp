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
    case Sigma16ISD::Jal:
        return "Sigma16ISD::Jal";
    case Sigma16ISD::TailCall:
        return "Sigma16ISD::TailCall";
    case Sigma16ISD::LoadSym:
        return "Sigma16ISD::LoadSym";
    case Sigma16ISD::Movei16:
        return "Sigma16ISD::Movei16";
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
    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {
    // Set up the register classes.
    addRegisterClass(MVT::i16, &Sigma16::CPURegsRegClass);

    // Compute derived properties from the register classes
    computeRegisterProperties(STI.getRegisterInfo());

    // Sigma16 only has i16 registers. So use i16 for everything.

    setBooleanContents(ZeroOrOneBooleanContent);
    setBooleanVectorContents(ZeroOrOneBooleanContent);

    // Load extended operations for i1 must be promoted to i16.
    for (MVT VT : MVT::integer_valuetypes()) {
        setLoadExtAction(ISD::EXTLOAD, VT, MVT::i1, Promote);
        setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i1, Promote);
        setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i1, Promote);
    }

    AddPromotedToType(ISD::SETCC, MVT::i1, MVT::i16);

    setStackPointerRegisterToSaveRestore(
        STI.getRegisterInfo()->getStackRegister());

    setSchedulingPreference(Sched::Source);

    // Nodes that require custom lowering.
    setOperationAction(ISD::GlobalAddress, MVT::i16, Custom);
}

const Sigma16TargetLowering *
Sigma16TargetLowering::create(const Sigma16TargetMachine &TM,
                              const Sigma16Subtarget &STI) {
    return llvm::createSigma16SETargetLowering(TM, STI);
}

SDValue Sigma16TargetLowering::lowerGlobalAddress(SDValue Op,
        SelectionDAG &DAG) const {
    EVT VT = Op.getValueType();
    GlobalAddressSDNode *G = cast<GlobalAddressSDNode>(Op);
    SDValue TargetAddr = DAG.getTargetGlobalAddress(G->getGlobal(), Op, MVT::i16);

    return DAG.getNode(Sigma16ISD::LoadSym, SDLoc(Op), VT, TargetAddr);
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
//            Call Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue
Sigma16TargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                 SmallVectorImpl<SDValue> &InVals) const {

    SelectionDAG &DAG = CLI.DAG;
    SDLoc &Loc = CLI.DL;
    SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
    SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
    SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
    SDValue Chain = CLI.Chain;
    SDValue Callee = CLI.Callee;
    CallingConv::ID CallConv = CLI.CallConv;
    // varargs is not supported
    const bool isVarArg = CLI.IsVarArg;
    // tail call is not supported
    CLI.IsTailCall = false;

    if (isVarArg) {
        llvm_unreachable("Unimplemented");
    }

    // Analyze operands of the call, assigning locations to each operand.
    SmallVector<CCValAssign, 16> ArgLocs;
    CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                   *DAG.getContext());
    CCInfo.AnalyzeCallOperands(Outs, CC_SIGMA16);
}

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
