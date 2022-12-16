//===-- Sigma16ELFObjectWriter.cpp - Sigma16 ELF Writer -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/Sigma16BaseInfo.h"
#include "MCTargetDesc/Sigma16FixupKinds.h"
#include "MCTargetDesc/Sigma16MCTargetDesc.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"
#include <list>

using namespace llvm;

namespace {
class Sigma16ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  Sigma16ELFObjectWriter(uint8_t OSABI, bool HasRelocationAddend, bool Is64);

  ~Sigma16ELFObjectWriter() = default;

  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override;
};
} // namespace

Sigma16ELFObjectWriter::Sigma16ELFObjectWriter(uint8_t OSABI,
                                               bool HasRelocationAddend,
                                               bool Is64)
    : MCELFObjectTargetWriter(/*Is64Bit=false*/ Is64, OSABI, ELF::EM_SIGMA16,
                              HasRelocationAddend) {}

unsigned Sigma16ELFObjectWriter::getRelocType(MCContext &Ctx,
                                              const MCValue &Target,
                                              const MCFixup &Fixup,
                                              bool IsPCRel) const {
  // TODO: Implement
  return ELF::R_SIGMA16_NONE;
}

bool Sigma16ELFObjectWriter::needsRelocateWithSymbol(const MCSymbol &Sym,
                                                     unsigned Type) const {
  // TODO: Implement
  return false;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createSigma16ELFObjectWriter(const Triple &TT) {
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  bool IsN64 = false;
  bool HasRelocationAddend = TT.isArch64Bit();
  return std::make_unique<Sigma16ELFObjectWriter>(OSABI, HasRelocationAddend,
                                                  IsN64);
}