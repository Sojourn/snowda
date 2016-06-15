#include "snw_util.h"

using namespace Snowda;

uint8_t Snowda::serializeBitTree(BitTree<8> tree)
{
    uint8_t buf;
    memcpy(&buf, tree.bits().data(), tree.bits().size());
    return buf;
}

uint16_t Snowda::serializeBitTree(BitTree<16> tree)
{
    uint16_t buf;
    memcpy(&buf, tree.bits().data(), tree.bits().size());
    return buf;
}

uint32_t Snowda::serializeBitTree(BitTree<32> tree)
{
    uint32_t buf;
    memcpy(&buf, tree.bits().data(), tree.bits().size());
    return buf;
}

uint64_t Snowda::serializeBitTree(BitTree<64> tree)
{
    uint64_t buf;
    memcpy(&buf, tree.bits().data(), tree.bits().size());
    return buf;
}

BitTree<8> Snowda::deserializeBitTree(uint8_t value)
{
    std::array<uint8_t, sizeof(uint8_t)> buf;
    memcpy(buf.data(), &value, buf.size());
    return BitTree<8>(buf);
}

BitTree<16> Snowda::deserializeBitTree(uint16_t value)
{
    std::array<uint8_t, sizeof(uint16_t)> buf;
    memcpy(buf.data(), &value, buf.size());
    return BitTree<16>(buf);
}

BitTree<32> Snowda::deserializeBitTree(uint32_t value)
{
    std::array<uint8_t, sizeof(uint32_t)> buf;
    memcpy(buf.data(), &value, buf.size());
    return BitTree<32>(buf);
}

BitTree<64> Snowda::deserializeBitTree(uint64_t value)
{
    std::array<uint8_t, sizeof(uint64_t)> buf;
    memcpy(buf.data(), &value, buf.size());
    return BitTree<64>(buf);
}
