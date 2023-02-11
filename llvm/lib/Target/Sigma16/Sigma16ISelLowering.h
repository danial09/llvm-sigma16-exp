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
  JmpLink,

  // Global address
  GlobalAddress,

  // Tail call
//  TailCall,

  //  Load a symbol into a register.
//  LoadSym,

  // Load a 16-bit immediate into a register.
//  Movei16,

  // Return
  Ret,
  EH_RETURN,

  DivRem,

  Wrapper
};
} // end namespace Sigma16ISD

//===--------------------------------------------------------------------===//
// TargetLowering Implementation
//===--------------------------------------------------------------------===//
class Sigma16FunctionInfo;
class Sigma16Subtarget;

//@class Sigma16TargetLowering
class Sigma16TargetLowering : public TargetLowering  {
public:
  explicit Sigma16TargetLowering(const Sigma16TargetMachine &TM,
                              const Sigma16Subtarget &STI);

  static const Sigma16TargetLowering *create(const Sigma16TargetMachine &TM,
                                          const Sigma16Subtarget &STI);

  /// lowerOperation - Provide custom lowering hooks for some operations.
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  /// getTargetNodeName - This method returns the name of a target specific
  //  DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

protected:
  /// ByValArgInfo - Byval argument information.
  struct ByValArgInfo {
    unsigned FirstIdx; // Index of the first register used.
    unsigned NumRegs;  // Number of registers used for this argument.
    unsigned Address;  // Offset of the stack area used to pass this argument.

    ByValArgInfo() : FirstIdx(0), NumRegs(0), Address(0) {}
  };

  //@CH3_4 1 {
  /// Sigma16CC - This class provides methods used to analyze formal and call
  /// arguments and inquire about calling convention information.
  class Sigma16CC {
  public:
    enum SpecialCallingConvType {
      NoSpecialCallingConv
    };

    Sigma16CC(CallingConv::ID CallConv, CCState &Info,
           SpecialCallingConvType SpecialCallingConv = NoSpecialCallingConv);

    void analyzeFormalArguments(const SmallVectorImpl<ISD::InputArg> &Ins,
                                bool IsSoftFloat,
                                Function::const_arg_iterator FuncArg);

    void analyzeCallResult(const SmallVectorImpl<ISD::InputArg> &Ins,const SDNode *CallNode,
                           const Type *RetTy) const;

    void analyzeReturn(const SmallVectorImpl<ISD::OutputArg> &Outs, const Type *RetTy) const;

    const CCState &getCCInfo() const { return CCInfo; }

    /// hasByValArg - Returns true if function has byval arguments.
    bool hasByValArg() const { return !ByValArgs.empty(); }

    /// regSize - Size (in number of bits) of integer registers.
    unsigned regSize() const { return 4; }
    /// numIntArgRegs - Number of integer registers available for calls.
    unsigned numIntArgRegs() const;
    /// Return pointer to array of integer argument registers.
    const ArrayRef<MCPhysReg> intArgRegs() const;

    unsigned reservedArgArea() const;

    typedef SmallVectorImpl<ByValArgInfo>::const_iterator byval_iterator;
    byval_iterator byval_begin() const { return ByValArgs.begin(); }
    byval_iterator byval_end() const { return ByValArgs.end(); }

  private:
    void handleByValArg(unsigned ValNo, MVT ValVT, MVT LocVT,
                        CCValAssign::LocInfo LocInfo,
                        ISD::ArgFlagsTy ArgFlags);

    /// useRegsForByval - Returns true if the calling convention allows the
    /// use of registers to pass byval arguments.
    bool useRegsForByval() const { return CallConv != CallingConv::Fast; }

    /// Return the function that analyzes fixed argument list functions.
    llvm::CCAssignFn *fixedArgFn() const;

    void allocateRegs(ByValArgInfo &ByVal, unsigned ByValSize,
                      unsigned Align);

    /// Return the type of the register which is used to pass an argument or
    /// return a value. This function returns f64 if the argument is an i64
    /// value which has been generated as a result of softening an f128 value.
    /// Otherwise, it just returns VT.
//    MVT getRegVT(MVT VT) const;

    template<typename Ty>
    void analyzeReturn(const SmallVectorImpl<Ty> &RetVals,
                       const SDNode *CallNode, const Type *RetTy) const;

    CCState &CCInfo;
    CallingConv::ID CallConv;
    SmallVector<ByValArgInfo, 2> ByValArgs;
  };
  //@CH3_4 1 }

protected:
  // Subtarget Info
  const Sigma16Subtarget &Subtarget;
  // Cache the ABI from the TargetMachine, we use it everywhere.
  const Sigma16ABIInfo &ABI;

private:
  // Lower Operand specifics
//  SDValue lowerBR_JT(SDValue Op, SelectionDAG &DAG) const;
//  SDValue lowerBRCOND(SDValue Op, SelectionDAG &DAG) const;
  SDValue lowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  
  virtual bool
  isEligibleForTailCallOptimization(const Sigma16CC &Sigma16CCInfo,
                                    unsigned NextStackOffset,
                                    const Sigma16FunctionInfo& FI) const {
        return false;
  };
  
  /// copyByValArg - Copy argument registers which were used to pass a byval
  /// argument to the stack. Create a stack frame object for the byval
  /// argument.
  void copyByValRegs(SDValue Chain, const SDLoc &DL,
                     std::vector<SDValue> &OutChains, SelectionDAG &DAG,
                     const ISD::ArgFlagsTy &Flags,
                     SmallVectorImpl<SDValue> &InVals,
                     const Argument *FuncArg,
                     const Sigma16CC &CC, const ByValArgInfo &ByVal) const;
  
  //- must be exist even without function call
  SDValue
  LowerFormalArguments(SDValue Chain,
                       CallingConv::ID CallConv, bool IsVarArg,
                       const SmallVectorImpl<ISD::InputArg> &Ins,
                       const SDLoc &dl, SelectionDAG &DAG,
                       SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain,
                      CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals,
                      const SDLoc &dl, SelectionDAG &DAG) const override;

};
const Sigma16TargetLowering *
createSigma16SETargetLowering(const Sigma16TargetMachine &TM, const Sigma16Subtarget &STI);
}

#endif // LLVM_LIB_TARGET_SIGMA16_SIGMA16ISELLOWERING_H
