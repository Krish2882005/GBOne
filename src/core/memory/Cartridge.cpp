#include "Cartridge.hpp"

#include <cstddef>
#include <cstdint>
#include <expected>
#include <fstream>
#include <ios>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "common/Literals.hpp"
#include "common/Logger.hpp"

namespace Core::Memory
{
using namespace Common::Literals;

// NOLINTNEXTLINE(bugprone-exception-escape)
std::expected<Cartridge, CartridgeError> Cartridge::Create(const std::string& filepath) noexcept
{
    Common::LogInfo("[Cartridge] Loading ROM from '{}'", filepath);

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file)
    {
        Common::LogError("[Cartridge] Failed to open file: '{}'", filepath);
        return std::unexpected(CartridgeError::FileReadFailed);
    }

    const std::streamsize fileSize = file.tellg();

    constexpr std::streamsize kMinCartridgeSize = 0x0150;
    if (fileSize < kMinCartridgeSize)
    {
        Common::LogError("[Cartridge] ROM size ({} bytes) is too small", fileSize);
        return std::unexpected(CartridgeError::FileReadFailed);
    }

    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> romData(static_cast<size_t>(fileSize));

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (!file.read(reinterpret_cast<char*>(romData.data()), fileSize))
    {
        Common::LogError("[Cartridge] Failed to read ROM data into memory");
        return std::unexpected(CartridgeError::FileReadFailed);
    }

    constexpr uint16_t kChecksumStart = 0x0134;
    constexpr uint16_t kChecksumEnd = 0x014C;
    constexpr uint16_t kChecksumOffset = 0x014D;
    constexpr uint16_t kTitleStart = 0x0134;
    constexpr uint16_t kOldLicenseeOffset = 0x014B;
    constexpr uint16_t kCgbFlagOffset = 0x0143;
    constexpr uint16_t kRomSizeOffset = 0x0148;
    constexpr uint16_t kRamSizeOffset = 0x0149;

    // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
    uint8_t checksum = 0;
    for (uint16_t i = kChecksumStart; i <= kChecksumEnd; i++)
    {
        checksum = static_cast<uint8_t>(checksum - romData[i] - 1);
    }

    if (checksum != romData[kChecksumOffset])
    {
        Common::LogError("[Cartridge] Header checksum mismatch! Expected: 0x{:02X}, Got: 0x{:02X}",
                         checksum,
                         romData[kChecksumOffset]);
        return std::unexpected(CartridgeError::InvalidChecksum);
    }

    Cartridge cartridge;

    // https://gbdev.io/pandocs/The_Cartridge_Header.html#0148--rom-size
    const auto romSize = static_cast<size_t>(32_KiB << romData[kRomSizeOffset]);

    if (romData.size() < romSize)
    {
        romData.resize(romSize, 0xFF);
    }

    cartridge.m_rom = std::move(romData);
    cartridge.m_romMask = romSize - 1;

    const uint8_t oldLicensee = cartridge.m_rom[kOldLicenseeOffset];
    const uint8_t cgbFlag = cartridge.m_rom[kCgbFlagOffset];

    // https://gbdev.io/pandocs/The_Cartridge_Header.html#0134-0143--title
    uint16_t titleLength = 16;
    if (oldLicensee == 0x33)
    {
        titleLength = 11;
    }
    else if (cgbFlag == 0x80 || cgbFlag == 0xC0)
    {
        titleLength = 15;
    }

    auto titleRange = cartridge.m_rom | std::views::drop(kTitleStart) | std::views::take(titleLength) |
                      std::views::take_while([](uint8_t c) { return c != 0x00; });

    cartridge.m_title = titleRange | std::ranges::to<std::string>();

    // https://gbdev.io/pandocs/The_Cartridge_Header.html#0149--ram-size
    const uint8_t ramSizeByte = cartridge.m_rom[kRamSizeOffset];

    size_t ramSize = 0;
    switch (ramSizeByte)
    {
        case 0x00:
            ramSize = 0;
            break;
        case 0x01:
            ramSize = 2_KiB;
            break;
        case 0x02:
            ramSize = 8_KiB;
            break;
        case 0x03:
            ramSize = 32_KiB;
            break;
        case 0x04:
            ramSize = 128_KiB;
            break;
        case 0x05:
            ramSize = 64_KiB;
            break;
        default:
            std::unreachable();
    }

    if (ramSize > 0)
    {
        cartridge.m_ram.resize(ramSize, 0xFF);
        cartridge.m_ramMask = ramSize - 1;
    }

    Common::LogInfo("[Cartridge] Loaded '{}' (ROM: {} bytes, RAM: {} bytes)", cartridge.m_title, romSize, ramSize);

    return cartridge;
}

uint8_t Cartridge::ReadRom(uint16_t address) const noexcept
{
    return m_rom[address & m_romMask];
}

uint8_t Cartridge::ReadRam(uint16_t address) const noexcept
{
    if (m_ram.empty())
    {
        return 0xFF;
    }

    return m_ram[address & m_ramMask];
}
} // namespace Core::Memory
