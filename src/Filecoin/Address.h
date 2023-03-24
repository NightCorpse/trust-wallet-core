// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <optional>
#include <vector>
#include <string>

namespace TW::Filecoin {

class Address {
  public:
    /// The actor ID of the Ethereum Address Manager singleton.
    static constexpr uint64_t ETHEREUM_ADDRESS_MANAGER_ACTOR_ID = 10;

    enum class Type : uint8_t {
        ID = 0,
        SECP256K1 = 1,
        ACTOR = 2,
        BLS = 3,
        DELEGATED = 4,
        Invalid,
    };

    /// Type of the Address.
    Type type = Type::Invalid;

    /// Actor ID.
    /// This is used if `type` is either `ID` or `DELEGATED`.
    uint64_t actorID = 0;

    /// Address data payload (without prefixes and checksum).
    Data payload;

    /// Decodes `encoded` as a Filecoin address.
    /// Returns `std::nullopt` on fail.
    static std::optional<Address> fromBytes(const Data& encoded);

    /// Parses `string` as a Filecoin address and validates the checksum.
    /// Returns `std::nullopt` if `string` is not a valid address.
    static std::optional<Address> fromString(const std::string& string);

    /// Determines whether a string makes a valid encoded address.
    static bool isValid(const std::string& string);

    /// Determines whether a collection of bytes makes a valid address.
    static bool isValid(const Data& encoded);

    /// Initializes a Secp256k1 address with a secp256k1 public key.
    static Address secp256k1Address(const PublicKey& publicKey);

    /// Initializes a Delegated address with a secp256k1 public key.
    static Address delegatedAddress(const PublicKey& publicKey);

    /// Initializes a Delegated address with a secp256k1 public key.
    static Address delegatedAddress(uint64_t actorID, const Data& payload);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& encoded);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;

    /// Returns encoded bytes of Address including the protocol byte and actorID (if required)
    /// without the checksum.
    Data toBytes() const;

    /// Address prefix
    static constexpr char PREFIX = 'f';

private:
    /// Initializes an address with a type, actorID and payload.
    explicit Address(Type type, uint64_t actorID, const Data& payload);

    /// Attempts to get the type by number.
    static Type getType(uint8_t raw) {
        switch (raw) {
        case 0:
            return Type::ID;
        case 1:
            return Type::SECP256K1;
        case 2:
            return Type::ACTOR;
        case 3:
            return Type::BLS;
        case 4:
            return Type::DELEGATED;
        default:
            return Type::Invalid;
        }
    }

    /// Attempts to get the type by ASCII.
    static Type parseType(char c) {
        if (c >= '0' && c <= '4') {
            return static_cast<Type>(c - '0');
        } else {
            return Type::Invalid;
        }
    }

    /// Returns ASCII character of type
    static char typeAscii(Type t) { return '0' + static_cast<char>(t); }

    /// Validates if the payload size (excluding any prefixes and checksum) of an address type has an expected value.
    static bool isValidPayloadSize(Type t, std::size_t payloadSize) {
        switch (t) {
            case Type::ID:
                return payloadSize == 0;
            case Type::SECP256K1:
            case Type::ACTOR:
                return payloadSize == 20;
            case Type::BLS:
                return payloadSize == 48;
            case Type::DELEGATED:
                return payloadSize <= 54;
            default:
                return false;
        }
    }
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.type == rhs.type
           && lhs.actorID == rhs.actorID
           && lhs.payload == rhs.payload;
}

} // namespace TW::Filecoin
