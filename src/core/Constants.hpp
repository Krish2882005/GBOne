#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>

#include "common/Literals.hpp"

namespace Core::Constants
{
using namespace Common::Literals;

constexpr std::size_t kScreenWidth = 160;
constexpr std::size_t kScreenHeight = 144;
constexpr std::size_t kScreenPixels = kScreenWidth * kScreenHeight;

constexpr std::size_t kVramSize = 8_KiB;
constexpr uint16_t kVramBaseAddress = 0x8000;

constexpr uint16_t kTileBlock0Address = 0x8000;
constexpr uint16_t kTileBlock1Address = 0x8800;
constexpr uint16_t kTileBlock2Address = 0x9000;

constexpr std::size_t kTileSize = 16;
constexpr std::size_t kTileWidth = 8;
constexpr std::size_t kTileHeigh = 8;

constexpr uint16_t kTileMap0Address = 0x9800;
constexpr uint16_t kTileMap1Address = 0x9C00;

constexpr std::size_t kTileMapDimension = 32;

} // namespace Core::Constants
