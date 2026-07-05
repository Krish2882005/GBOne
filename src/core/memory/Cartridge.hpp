#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

namespace Core::Memory
{
enum class CartridgeError : uint8_t
{
    FileReadFailed,
    InvalidChecksum
};

class Cartridge
{
public:
    [[nodiscard]] static std::expected<Cartridge, CartridgeError> Create(const std::string& filepath) noexcept;

    ~Cartridge() = default;

    Cartridge(const Cartridge&) = delete;
    Cartridge& operator=(const Cartridge&) = delete;
    Cartridge(Cartridge&&) noexcept = default;
    Cartridge& operator=(Cartridge&&) noexcept = default;

    [[nodiscard]] uint8_t ReadRom(uint16_t address) const noexcept;
    [[nodiscard]] uint8_t ReadRam(uint16_t address) const noexcept;

    [[nodiscard]] const std::string& GetTitle() const noexcept { return m_title; }

private:
    Cartridge() = default;

    std::vector<uint8_t> m_rom;
    std::vector<uint8_t> m_ram;
    std::string m_title;
    size_t m_romMask{0};
    size_t m_ramMask{0};
};
} // namespace Core::Memory
