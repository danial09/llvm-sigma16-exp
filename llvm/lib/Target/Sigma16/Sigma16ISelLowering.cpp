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
  case Sigma16ISD::GlobalAddress:
    return "Sigma16ISD::GlobalAddress";
    //  case Sigma16ISD::TailCall:
    //    return "Sigma16ISD::TailCall";
    //  case Sigma16ISD::LoadSym:
    //    return "Sigma16ISD::LoadSym";
    //  case Sigma16ISD::Movei16:
    //    return "Sigma16ISD::Movei16";
  case Sigma16ISD::Ret:
    return "Sigma16ISD::Ret";
  case Sigma16ISD::EH_RETURN:
    return "Sigma16ISD::EH_RETURN";
  case Sigma16ISD::DivRem:
    return "Sigma16ISD::DivRem";
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

  // Load extended operations for i1/i8 must be promoted to i16.
  for (MVT VT : MVT::integer_valuetypes()) {
    setLoadExtAction(ISD::EXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::EXTLOAD, VT, MVT::i8, Promote);
    setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i8, Promote);
    setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i8, Promote);
  }

  AddPromotedToType(ISD::SETCC, MVT::i1, MVT::i16);
  AddPromotedToType(ISD::BR_CC, MVT::i1, MVT::i16);
  AddPromotedToType(ISD::SELECT_CC, MVT::i1, MVT::i16);

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

SDValue Sigma16TargetLowering::LowerOperation(SDValue Op,
                                              SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:
    return lowerGlobalAddress(Op, DAG);
  case ISD::BRCOND:
    return Op;
  default:
    llvm_unreachable("unimplemented operand");
  }
}
//===----------------------------------------------------------------------===//
//  Lower helper functions
//===----------------------------------------------------------------------===//

// addLiveIn - This helper function adds the specified physical register to the
// MachineFunction as a live in value.  It also creates a corresponding
// virtual register for it.
static unsigned
addLiveIn(MachineFunction &MF, unsigned PReg, const TargetRegisterClass *RC)
{
  unsigned VReg = MF.getRegInfo().createVirtualRegister(RC);
  MF.getRegInfo().addLiveIn(PReg, VReg);
  return VReg;
}

SDValue Sigma16TargetLowering::lowerGlobalAddress(SDValue Op,
                                                  SelectionDAG &DAG) const {
  const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
  SDLoc DL(Op);
  SDValue GA = DAG.getTargetGlobalAddress(GV, DL, MVT::i16);
  return DAG.getNode(Sigma16ISD::GlobalAddress, DL, MVT::i16, GA);
}

//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//


// Passed in stack only.
static bool CC_Sigma16S16(unsigned ValNo, MVT ValVT, MVT LocVT,
                       CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                       CCState &State) {
  // Do not process byval args here.
  if (ArgFlags.isByVal())
    return true;
  
  // Promote i8 to i16
  if (LocVT == MVT::i8) {
    LocVT = MVT::i16;
    if (ArgFlags.isSExt())
      LocInfo = CCValAssign::SExt;
    else if (ArgFlags.isZExt())
      LocInfo = CCValAssign::ZExt;
    else
      LocInfo = CCValAssign::AExt;
  }

  Align OrigAlign = ArgFlags.getNonZeroOrigAlign();
  unsigned Offset = State.AllocateStack(ValVT.getSizeInBits() >> 3,
                                        OrigAlign);
  State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset, LocVT, LocInfo));
  return false;
}

// Passed first two i16 arguments in registers and others in stack.
static bool CC_Sigma16O16(unsigned ValNo, MVT ValVT, MVT LocVT,
                       CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                       CCState &State) {
  static const MCPhysReg IntRegs[] = { Sigma16::R2, Sigma16::R3 };

  // Do not process byval args here.
  if (ArgFlags.isByVal())
    return true;

  // floating point not supported.
  if (ValVT == MVT::f32 || ValVT == MVT::f64)
    return true;

  // Promote i8 to i16
  if (LocVT == MVT::i8) {
    LocVT = MVT::i16;
    if (ArgFlags.isSExt())
      LocInfo = CCValAssign::SExt;
    else if (ArgFlags.isZExt())
      LocInfo = CCValAssign::ZExt;
    else
      LocInfo = CCValAssign::AExt;
  }

  unsigned Reg;
  Align OrigAlign = ArgFlags.getNonZeroOrigAlign();

  if (ValVT == MVT::i16) {
    Reg = State.AllocateReg(IntRegs);
  } else {
    llvm_unreachable("Only i16 supported");
  }

  if (!Reg) {
    unsigned Offset = State.AllocateStack(ValVT.getSizeInBits() >> 3,
                                          Align(OrigAlign));
    State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset, LocVT, LocInfo));
  } else
    State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));

  return false;
}

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
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  Sigma16FunctionInfo *Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();

  Sigma16FI->setVarArgsFrameIndex(0);

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(),
                 ArgLocs, *DAG.getContext());
  Sigma16CC Sigma16CCInfo(CallConv,
                    CCInfo);

  const Function &Func = DAG.getMachineFunction().getFunction();
  Function::const_arg_iterator FuncArg = Func.arg_begin();

  bool UseSoftFloat = Subtarget.abiUsesSoftFloat();

  Sigma16CCInfo.analyzeFormalArguments(Ins, UseSoftFloat, FuncArg);
  Sigma16FI->setFormalArgInfo(CCInfo.getNextStackOffset(),
                           Sigma16CCInfo.hasByValArg());

  // Used with vargs to acumulate store chains.
  std::vector<SDValue> OutChains;

  unsigned CurArgIdx = 0;
  Sigma16CC::byval_iterator ByValArg = Sigma16CCInfo.byval_begin();

  //@2 {
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    //@2 }
    CCValAssign &VA = ArgLocs[i];
    if (Ins[i].isOrigArg()) {
      std::advance(FuncArg, Ins[i].getOrigArgIndex() - CurArgIdx);
      CurArgIdx = Ins[i].getOrigArgIndex();
    }
    EVT ValVT = VA.getValVT();
    ISD::ArgFlagsTy Flags = Ins[i].Flags;
    bool IsRegLoc = VA.isRegLoc();

    //@byval pass {
    if (Flags.isByVal()) {
      assert(Flags.getByValSize() &&
             "ByVal args of size 0 should have been ignored by front-end.");
      assert(ByValArg != Sigma16CCInfo.byval_end());
      copyByValRegs(Chain, DL, OutChains, DAG, Flags, InVals, &*FuncArg,
                    Sigma16CCInfo, *ByValArg);
      ++ByValArg;
      continue;
    }
    //@byval pass }
    // Arguments stored on registers
    if (ABI.IsO16() && IsRegLoc) {
      MVT RegVT = VA.getLocVT();
      unsigned ArgReg = VA.getLocReg();
      const TargetRegisterClass *RC = getRegClassFor(RegVT);

      // Transform the arguments stored on
      // physical registers into virtual ones
      unsigned Reg = addLiveIn(DAG.getMachineFunction(), ArgReg, RC);
      SDValue ArgValue = DAG.getCopyFromReg(Chain, DL, Reg, RegVT);

      // If this is an 8-bit value, it has been passed promoted
      // to 16 bits.  Insert an assert[sz]ext to capture this, then
      // truncate to the right size.
      if (VA.getLocInfo() != CCValAssign::Full) {
        unsigned Opcode = 0;
        if (VA.getLocInfo() == CCValAssign::SExt)
          Opcode = ISD::AssertSext;
        else if (VA.getLocInfo() == CCValAssign::ZExt)
          Opcode = ISD::AssertZext;
        if (Opcode)
          ArgValue = DAG.getNode(Opcode, DL, RegVT, ArgValue,
                                 DAG.getValueType(ValVT));
        ArgValue = DAG.getNode(ISD::TRUNCATE, DL, ValVT, ArgValue);
      }
    } else { // VA.isRegLoc()
      MVT LocVT = VA.getLocVT();

      // sanity check
      assert(VA.isMemLoc());

      // The stack pointer offset is relative to the caller stack frame.
      int FI = MFI.CreateFixedObject(ValVT.getSizeInBits()/8,
                                     VA.getLocMemOffset(), true);

      // Create load nodes to retrieve arguments from the stack
      SDValue FIN = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
      SDValue Load = DAG.getLoad(
          LocVT, DL, Chain, FIN,
          MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI));
      InVals.push_back(Load);
      OutChains.push_back(Load.getValue(1));
    }
  }

  //@Ordinary struct type: 1 {
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    // The Sigma16 ABIs for returning structs by value requires that we copy
    // the sret argument into $v0 for the return. Save the argument into
    // a virtual register so that we can access it from the return points.
    if (Ins[i].Flags.isSRet()) {
      unsigned Reg = Sigma16FI->getSRetReturnReg();
      if (!Reg) {
        Reg = MF.getRegInfo().createVirtualRegister(
            getRegClassFor(MVT::i16));
        Sigma16FI->setSRetReturnReg(Reg);
      }
      SDValue Copy = DAG.getCopyToReg(DAG.getEntryNode(), DL, Reg, InVals[i]);
      Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, Copy, Chain);
      break;
    }
  }
  //@Ordinary struct type: 1 }

  // All stores are grouped in one node to allow the matching between
  // the size of Ins and InVals. This only happens when on varg functions
  if (!OutChains.empty()) {
    OutChains.push_back(Chain);
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, OutChains);
  }

  return Chain;
}
// @LowerFormalArguments }

//===----------------------------------------------------------------------===//
//            Call Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue
Sigma16TargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                              SmallVectorImpl<SDValue> &InVals) const {
  return CLI.Chain;
}

//===----------------------------------------------------------------------===//
//@              Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue
Sigma16TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                   bool IsVarArg,
                                   const SmallVectorImpl<ISD::OutputArg> &Outs,
                                   const SmallVectorImpl<SDValue> &OutVals,
                                   const SDLoc &DL, SelectionDAG &DAG) const {
  // CCValAssign - represent the assignment of
  // the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;
  MachineFunction &MF = DAG.getMachineFunction();

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, *DAG.getContext());
  Sigma16CC Sigma16CCInfo(CallConv, CCInfo);

  // Analyze return values.
  Sigma16CCInfo.analyzeReturn(Outs, MF.getFunction().getReturnType());

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    if (RVLocs[i].getValVT() != RVLocs[i].getLocVT())
      Val = DAG.getNode(ISD::BITCAST, DL, RVLocs[i].getLocVT(), Val);

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  //@Ordinary struct type: 2 {
  // The Sigma16 ABIs for returning structs by value requires that we copy
  // the sret argument into $v0 for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into $v0.
  //  if (MF.getFunction().hasStructRetAttr()) {
  //    Sigma16FunctionInfo *Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();
  //    unsigned Reg = Sigma16FI->getSRetReturnReg();
  //
  //    if (!Reg)
  //      llvm_unreachable("sret virtual register not created in the entry
  //      block");
  //    SDValue Val =
  //        DAG.getCopyFromReg(Chain, DL, Reg,
  //        getPointerTy(DAG.getDataLayout()));
  //    unsigned V0 = Sigma16::V0;
  //
  //    Chain = DAG.getCopyToReg(Chain, DL, V0, Val, Flag);
  //    Flag = Chain.getValue(1);
  //    RetOps.push_back(DAG.getRegister(V0,
  //    getPointerTy(DAG.getDataLayout())));
  //  }
  //@Ordinary struct type: 2 }

  RetOps[0] = Chain; // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return on Sigma16 is always a "jump    0[R13]"
  return DAG.getNode(Sigma16ISD::Ret, DL, MVT::Other, RetOps);
}

Sigma16TargetLowering::Sigma16CC::Sigma16CC(
    CallingConv::ID CC, CCState &Info,
    Sigma16CC::SpecialCallingConvType SpecialCallingConv)
    : CCInfo(Info), CallConv(CC) {
  // Pre-allocate reserved argument area.
  CCInfo.AllocateStack(reservedArgArea(), Align(2));
}

template <typename Ty>
void Sigma16TargetLowering::Sigma16CC::analyzeReturn(
    const SmallVectorImpl<Ty> &RetVals, const SDNode *CallNode,
    const Type *RetTy) const {
  CCAssignFn *Fn;

  Fn = RetCC_SIGMA16;

  for (unsigned I = 0, E = RetVals.size(); I < E; ++I) {
    MVT VT = RetVals[I].VT;
    ISD::ArgFlagsTy Flags = RetVals[I].Flags;

    if (Fn(I, VT, VT, CCValAssign::Full, Flags, this->CCInfo)) {
#ifndef NDEBUG
      dbgs() << "Call result #" << I << " has unhandled type "
             << EVT(VT).getEVTString() << '\n';
#endif
      llvm_unreachable(nullptr);
    }
  }
}

void Sigma16TargetLowering::Sigma16CC::handleByValArg(unsigned ValNo, MVT ValVT,
                                                MVT LocVT,
                                                CCValAssign::LocInfo LocInfo,
                                                ISD::ArgFlagsTy ArgFlags) {
  assert(ArgFlags.getByValSize() && "Byval argument's size shouldn't be 0.");

  struct ByValArgInfo ByVal;
  unsigned RegSize = regSize();
  unsigned ByValSize = alignTo(ArgFlags.getByValSize(), RegSize);
  Align Alignment = std::min(std::max(ArgFlags.getNonZeroByValAlign(), Align(RegSize)),
                             Align(RegSize * 2));

  if (useRegsForByval())
    allocateRegs(ByVal, ByValSize, Alignment.value());

  // Allocate space on caller's stack.
  ByVal.Address = CCInfo.AllocateStack(ByValSize - RegSize * ByVal.NumRegs,
                                       Alignment);
  CCInfo.addLoc(CCValAssign::getMem(ValNo, ValVT, ByVal.Address, LocVT,
                                    LocInfo));
  ByValArgs.push_back(ByVal);
}

static const MCPhysReg O16IntRegs[] = {
    Sigma16::R2, Sigma16::R3
};

const ArrayRef<MCPhysReg> Sigma16TargetLowering::Sigma16CC::intArgRegs() const {
  return makeArrayRef(O16IntRegs);
}

unsigned Sigma16TargetLowering::Sigma16CC::numIntArgRegs() const {
  // FIXME: Use tablegen.
  return 2;
}

llvm::CCAssignFn *Sigma16TargetLowering::Sigma16CC::fixedArgFn() const {
    return CC_Sigma16O16;
}

void Sigma16TargetLowering::Sigma16CC::allocateRegs(ByValArgInfo &ByVal,
                                              unsigned ByValSize,
                                              unsigned Align) {
  unsigned RegSize = regSize(), NumIntArgRegs = numIntArgRegs();
  const ArrayRef<MCPhysReg> IntArgRegs = intArgRegs();
  assert(!(ByValSize % RegSize) && !(Align % RegSize) &&
         "Byval argument's size and alignment should be a multiple of"
         "RegSize.");
  
  ByVal.FirstIdx = CCInfo.getFirstUnallocated(IntArgRegs);

  // If Align > RegSize, the first arg register must be even.
  if ((Align > RegSize) && (ByVal.FirstIdx % 2)) {
    CCInfo.AllocateReg(IntArgRegs[ByVal.FirstIdx]);
    ++ByVal.FirstIdx;
  }

  // Mark the registers allocated.
  for (unsigned I = ByVal.FirstIdx; ByValSize && (I < NumIntArgRegs);
       ByValSize -= RegSize, ++I, ++ByVal.NumRegs)
    CCInfo.AllocateReg(IntArgRegs[I]);
}

void Sigma16TargetLowering::Sigma16CC::
    analyzeFormalArguments(const SmallVectorImpl<ISD::InputArg> &Args,
                           bool IsSoftFloat, Function::const_arg_iterator FuncArg) {
  unsigned NumArgs = Args.size();
  llvm::CCAssignFn *FixedFn = fixedArgFn();
  unsigned CurArgIdx = 0;
  
  for (unsigned I = 0; I != NumArgs; ++I) {
    MVT ArgVT = Args[I].VT;
    ISD::ArgFlagsTy ArgFlags = Args[I].Flags;
    if (Args[I].isOrigArg()) {
      std::advance(FuncArg, Args[I].getOrigArgIndex() - CurArgIdx);
      CurArgIdx = Args[I].getOrigArgIndex();
    }
    CurArgIdx = Args[I].OrigArgIndex;

    if (ArgFlags.isByVal()) {
      handleByValArg(I, ArgVT, ArgVT, CCValAssign::Full, ArgFlags);
      continue;
    }


    if (!FixedFn(I, ArgVT, ArgVT, CCValAssign::Full, ArgFlags, CCInfo))
      continue;

#ifndef NDEBUG
    dbgs() << "Formal Arg #" << I << " has unhandled type "
           << EVT(ArgVT).getEVTString();
#endif
    llvm_unreachable(nullptr);
  }
}


void Sigma16TargetLowering::Sigma16CC::analyzeCallResult(
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDNode *CallNode,
    const Type *RetTy) const {
  analyzeReturn(Ins, CallNode, RetTy);
}

void Sigma16TargetLowering::Sigma16CC::analyzeReturn(
    const SmallVectorImpl<ISD::OutputArg> &Outs, const Type *RetTy) const {
  analyzeReturn(Outs, nullptr, RetTy);
}

unsigned Sigma16TargetLowering::Sigma16CC::reservedArgArea() const {
  return ((CallConv != CallingConv::Fast)) ? 16 : 0;
}

void Sigma16TargetLowering::
    copyByValRegs(SDValue Chain, const SDLoc &DL, std::vector<SDValue> &OutChains,
                  SelectionDAG &DAG, const ISD::ArgFlagsTy &Flags,
                  SmallVectorImpl<SDValue> &InVals, const Argument *FuncArg,
                  const Sigma16CC &CC, const ByValArgInfo &ByVal) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  unsigned RegAreaSize = ByVal.NumRegs * CC.regSize();
  unsigned FrameObjSize = std::max(Flags.getByValSize(), RegAreaSize);
  int FrameObjOffset;
  
  const ArrayRef<MCPhysReg> ByValArgRegs = CC.intArgRegs();

  if (RegAreaSize)
    FrameObjOffset = (int)CC.reservedArgArea() -
                     (int)((CC.numIntArgRegs() - ByVal.FirstIdx) * CC.regSize());
  else
    FrameObjOffset = ByVal.Address;

  // Create frame object.
  EVT PtrTy = getPointerTy(DAG.getDataLayout());
  int FI = MFI.CreateFixedObject(FrameObjSize, FrameObjOffset, true);
  SDValue FIN = DAG.getFrameIndex(FI, PtrTy);
  InVals.push_back(FIN);

  if (!ByVal.NumRegs)
    return;

  // Copy arg registers.
  MVT RegTy = MVT::getIntegerVT(CC.regSize() * 8);
  const TargetRegisterClass *RC = getRegClassFor(RegTy);

  for (unsigned I = 0; I < ByVal.NumRegs; ++I) {
    unsigned ArgReg = ByValArgRegs[ByVal.FirstIdx + I];
    unsigned VReg = addLiveIn(MF, ArgReg, RC);
    unsigned Offset = I * CC.regSize();
    SDValue StorePtr = DAG.getNode(ISD::ADD, DL, PtrTy, FIN,
                                   DAG.getConstant(Offset, DL, PtrTy));
    SDValue Store = DAG.getStore(Chain, DL, DAG.getRegister(VReg, RegTy),
                                 StorePtr, MachinePointerInfo(FuncArg, Offset));
    OutChains.push_back(Store);
  }
}

