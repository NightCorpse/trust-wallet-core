// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::c_string_standalone;
use tw_memory::ffi::free_string;

#[test]
fn tests_ffi_string() {
    unsafe {
        free_string(c_string_standalone("foo".to_string()));
    }
}
