#include "SECP256K1.h"

const BigInteger SECP256K1::base = BigInteger(2);
const BigInteger SECP256K1::prime = SECP256K1::base.myPow(256) - SECP256K1::base.myPow(32) - BigInteger(977);
const BigInteger SECP256K1::N = N.__parseHexToDecimal("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8D0364141");
const BigInteger SECP256K1::baseX = BigInteger("79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798").parseHexToDecimal();
const SECP256K1IFET<BigInteger> SECP256K1::Gx = SECP256K1IFET<BigInteger>(baseX, prime);