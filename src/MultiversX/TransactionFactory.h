// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "GasEstimator.h"
#include "NetworkConfig.h"
#include "Transaction.h"
#include "uint256.h"
#include "../proto/MultiversX.pb.h"

namespace TW::MultiversX {

/// Creates specific transaction objects, wrt. the provided "NetworkConfig".
class TransactionFactory {
private:
    NetworkConfig networkConfig;
    GasEstimator gasEstimator;

public:
    TransactionFactory();
    TransactionFactory(const NetworkConfig& networkConfig);

    /// Creates the appropriate transaction object, with respect to the "oneof" field (substructure) of Proto::SigningInput.
    Transaction create(const Proto::SigningInput& input);

    Transaction fromGenericAction(const Proto::SigningInput& input);

    /// This should be used to transfer EGLD.
    /// For reference, see: https://docs.multiversx.com/developers/signing-transactions/signing-transactions.
    Transaction fromEGLDTransfer(const Proto::SigningInput& input);

    /// This should be used to transfer regular ESDTs (fungible tokens).
    /// For reference, see: https://docs.multiversx.com/developers/esdt-tokens
    ///
    /// The "regular" ESDT tokens held by an account can be fetched from https://api.multiversx.com/accounts/{address}/tokens.
    Transaction fromESDTTransfer(const Proto::SigningInput& input);

    /// This should be used to transfer NFTs, SFTs and Meta ESDTs.
    /// For reference, see: https://docs.multiversx.com/developers/nft-tokens
    ///
    /// The semi-fungible and non-fungible tokens held by an account can be fetched from https://api.multiversx.com/accounts/{address}/nfts?type=SemiFungibleESDT,NonFungibleESDT.
    /// The Meta ESDTs (a special kind of SFTs) held by an account can be fetched from https://api.multiversx.com/accounts/{address}/nfts?type=MetaESDT.
    ///
    /// The fields "token_collection" and "token_nonce" are found as well in the HTTP response of the API call (as "collection" and "nonce", respectively).
    Transaction fromESDTNFTTransfer(const Proto::SigningInput& input);

private:
    uint64_t coalesceGasLimit(uint64_t providedGasLimit, uint64_t estimatedGasLimit);
    uint64_t coalesceGasPrice(uint64_t gasPrice);
    std::string coalesceChainId(std::string chainID);
    std::string prepareFunctionCall(const std::string& function, std::initializer_list<const std::string> arguments);
};

} // namespace TW::MultiversX
