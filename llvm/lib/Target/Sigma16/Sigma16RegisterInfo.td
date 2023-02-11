//===-- Sigma16RegisterInfo.td - Sigma16 Register Defs -----*- tablegen -*-===//
// Copyright 2022 Danial Tariq
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Sigma-16 Register
class Sigma16Reg<bits<16> Enc, string n> : Register<n> {
  // For tablegen(... -gen-emitter)  in CMakeLists.txt
  let HWEncoding = Enc;
  
  let Namespace = "Sigma16";
}

// Register File
foreach i = 0-15 in {
  def R#i : Sigma16Reg<i, "r"#i>, DwarfRegNum<[i]>;
}

def CPURegs : RegisterClass<"Sigma16", [i16], 16,
    (add
    R0,                         // Constant zero
    R1,                         // First function parameter and result
    (sequence "R%i", 2, 12),    // Volatile registers
    R13,                        // Return address                     
    R14,                        // Stack Pointer
    R15                         // Reserved
    )>;

def SR     : RegisterClass<"Sigma16", [i16], 16, (add R15)>;
