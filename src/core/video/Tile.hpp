#pragma once

#include <cstdint>
#include <span>

#include "core/Constants.hpp"

namespace Core::Video
{
enum class AddressingMode : uint8_t
{
    Mode8000,
    Mode8800
};

namespace Tile
{
// https://gbdev.io/pandocs/Tile_Data.html
[[nodiscard]] constexpr uint16_t GetAddress(uint8_t tileId, AddressingMode mode) noexcept
{
    if (mode == AddressingMode::Mode8000)
    {
        return Constants::kVramBlock0Start + (static_cast<uint16_t>(tileId) * Constants::kTileSizeBytes);
    }

    const auto signedTileId = static_cast<int8_t>(tileId);
    return static_cast<uint16_t>(Constants::kVramBlock2Start +
                                 (signedTileId * static_cast<int>(Constants::kTileSizeBytes)));
}

[[nodiscard]] constexpr uint8_t
GetPixelColorId(std::span<const uint8_t> vram, uint16_t tileAddress, uint8_t x, uint8_t y) noexcept
{
    const uint16_t vramOffset = tileAddress - Constants::kVramBaseAddress;

    const uint8_t lowByte = vram[vramOffset + (y * 2)];
    const uint8_t highByte = vram[vramOffset + (y * 2) + 1];

    const uint8_t bitShift = 7 - x;

    const uint8_t lowBit = (lowByte >> bitShift) & 0x01;
    const uint8_t highBit = (highByte >> bitShift) & 0x01;

    return static_cast<uint8_t>((highBit << 1) | lowBit);
}
} // namespace Tile

namespace TileMap
{
// https://gbdev.io/pandocs/Tile_Maps.html
[[nodiscard]] constexpr uint8_t
GetTileId(std::span<const uint8_t> vram, uint16_t mapBaseAddress, uint8_t tileX, uint8_t tileY) noexcept
{
    const uint16_t gridOffset = (static_cast<uint16_t>(tileY) * Constants::kTileMapDimension) + tileX;
    const uint16_t physicalAddress = mapBaseAddress + gridOffset;

    return vram[physicalAddress - Constants::kVramBaseAddress];
}
} // namespace TileMap
} // namespace Core::Video
