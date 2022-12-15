//===-- Sigma16ISelLowering.h - Sigma16 DAG Lowering Interface --*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16ISELLOWERING_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16ISELLOWERING_H

#include "MCTargetDesc/Sigma16ABIInfo.h"
#include "Sigma16.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/Function.h"
#include <deque>

namespace llvm {
namespace Sigma16ISD {
enum NodeType {
  // Start the numbering from where ISD NodeType finishes.
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  // Jump and link (call)
  Jal,

  // Tail call
  TailCall,

  //  Load a symbol into a register.
  LoadSym,

  // Load a 16-bit immediate into a register.
  Movei16,

  // Return
  Ret,

  Wrapper
};
} // end namespace Sigma16ISD

//===--------------------------------------------------------------------===//
// TargetLowering Implementation
//===--------------------------------------------------------------------===//
class Sigma16FunctionInfo;
class Sigma16Subtarget;

//@class Sigma16TargetLowering
class Sigma16TargetLowering : public TargetLowering {
public:
  explicit Sigma16TargetLowering(const Sigma16TargetMachine &TM,
                                 const Sigma16Subtarget &STI);

  static const Sigma16TargetLowering *create(const Sigma16TargetMachine &TM,
                                             const Sigma16Subtarget &STI);

  /// getTargetNodeName - This method returns the name of a target specific
  /// DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

protected:
  /// ByValArgInfo - Byval argument information.
  struct ByValArgInfo {
    unsigned FirstIdx; // Index of the first register used.
    unsigned NumRegs;  // Number of registers used for this argument.
    unsigned Address;  // Offset of the stack area used to pass this argument.

    ByValArgInfo() : FirstIdx(0), NumRegs(0), Address(0) {}
  };

protected:
  // Subtarget Info
  const Sigma16Subtarget &Subtarget;
  // Cache the ABI from the TargetMachine, we use it everywhere.
  const Sigma16ABIInfo &ABI;

private:
  // Lower Operand specifics
  SDValue lowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

  //- must exist even without function all
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &Dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerCall(CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  //  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool
  //  IsVarArg,
  //                      const SmallVectorImpl<ISD::OutputArg> &Outs,
  //                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc
  //                      &dl, SelectionDAG &DAG) const override;
};
const Sigma16TargetLowering *
createSigma16SETargetLowering(const Sigma16TargetMachine &TM,
                              const Sigma16Subtarget &STI);
} // namespace llvm

#endif // LLVM_LIB_TARGET_SIGMA16_SIGMA16ISELLOWERING_H
