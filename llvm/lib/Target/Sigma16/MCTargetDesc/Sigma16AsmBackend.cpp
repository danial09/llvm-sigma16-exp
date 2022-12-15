//
// Created by dania on 11/12/2022.
//

#include "Sigma16AsmBackend.h"

#include "MCTargetDesc/Sigma16MCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// TODO: Implement this.
std::unique_ptr<MCObjectTargetWriter>
Sigma16AsmBackend::createObjectTargetWriter() const {
    return createSigma16ELFObjectWriter(TheTriple);
}

MCAsmBackend *llvm::createSigma16AsmBackend(const Target &T,
        const MCSubtargetInfo &STI,
        const MCRegisterInfo &MRI,
        const MCTargetOptions &Options) {
    return new Sigma16AsmBackend(T, STI.getTargetTriple());
}

void Sigma16AsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                   const MCValue &Target,
                                   MutableArrayRef<char> Data, uint64_t Value,
                                   bool IsResolved,
                                   const MCSubtargetInfo *STI) const {
    unsigned NumBytes = 2; // FIXME: Get this from the fixup kind.
    unsigned Offset = Fixup.getOffset();

    for (unsigned i = 0; i != NumBytes; ++i) {
        Data[Offset + i] = uint8_t(Value >> (i * 8));
    }
}

const MCFixupKindInfo &
Sigma16AsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
    // TODO: Implement this.

    return llvm::MCAsmBackend::getFixupKindInfo(Kind);
}