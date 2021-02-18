#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

const int8_t bit_7 = 7, bit_6 = 6, bit_5 = 5, bit_4 = 4, bit_3 = 3, bit_2 = 2, bit_1 = 1,
bit_0 = 0;

const int8_t INTERRUPT_VBLANK = 0, INTERRUPT_LCDC = 1, INTERRUPT_TIMER = 2, INTERRUPT_SERIAL = 3, INTERRUPT_JOYPAD = 4;
    
const int A = 0, B = 18, START = 25, SELECT = 23, LEFT  = 71, RIGHT = 72,
UP = 73, DOWN = 74;

//buttons are at FF00 first two are zero, takes A buncha steps+ masking to get an input

// Helper functions
uint8_t high_byte(uint16_t target);
uint8_t low_byte(uint16_t target);
uint8_t high_nibble(uint8_t target);
uint8_t low_nibble(uint8_t target);
uint16_t combine(uint8_t high, uint8_t low);
bool between(uint8_t target, int low, int high);
uint8_t set_bit(uint8_t data, uint8_t bit);
uint8_t clear_bit(uint8_t data, uint8_t bit);
bool is_bit_set(uint8_t data, uint8_t bit);

// Register pair helper class
class Pair
{
    private:
        uint8_t& high;
        uint8_t& low;

    public:
        Pair(uint8_t& high_, uint8_t& low_)
            : high(high_), low(low_) {}
        void inc();
        void dec();
        void set(uint16_t value);
        void set(uint8_t upper, uint8_t lower);
        uint16_t get();

        uint16_t address();
};

// Memory register helper class
class MemoryRegister
{
    private:
        uint8_t *value;

    public:
        MemoryRegister::MemoryRegister();
        MemoryRegister::MemoryRegister(uint8_t *_data);
        uint8_t get();
        void set(uint8_t data);
        void clear();
        void set_bit(uint8_t bit);
        void clear_bit(uint8_t bit);
        bool is_bit_set(uint8_t bit);
};

//boom

class MemoryController
{
	protected:
		// $0000 - $7FFF, 32kB Cartridge (potentially dynamic)
		vector<uint8_t> CART_ROM;
		// $A000 - $BFFF, 8kB Cartridge external switchable RAM bank
		vector<uint8_t> ERAM;

		// Bank selectors
		uint8_t ROM_bank_id = 1;
		uint8_t RAM_bank_id = 0;

		bool RAM_bank_enabled = false;
		bool RAM_access_enabled = false;

		// Mode selector
		uint8_t mode = 0;
		const uint8_t MODE_ROM = 0;
		const uint8_t MODE_RAM = 1;

	public:
		void init(vector<uint8_t> cartridge_buffer);
		virtual uint8_t read(Address location) = 0;
		virtual void write(Address location, uint8_t data) = 0;

		// Save states
		vector<uint8_t> get_ram();
		void set_ram(vector<uint8_t> data);
		virtual void save_state(ofstream &file);
		virtual void load_state(ifstream &file);
};

// This class represents games that only use the exact 32kB of cartridge space
class MemoryController0 : public MemoryController {
	uint8_t read(Address location);
	void write(Address location, uint8_t data);
};

// MBC1 (max 2MByte ROM and/or 32KByte RAM)
class MemoryController1 : public MemoryController {
	uint8_t read(Address location);
	void write(Address location, uint8_t data);
	void save_state(ofstream &file);
	void load_state(ifstream &file);
};

// MBC2 (max 256KByte ROM and 512x4 bits RAM)
class MemoryController2 : public MemoryController {
	uint8_t read(Address location);
	void write(Address location, uint8_t data);
};

// MBC3(max 2MByte ROM and / or 32KByte RAM and Timer)
class MemoryController3 : public MemoryController {
	
	bool RTC_enabled = false;

	uint8_t read(Address locatison);
	void write(Address location, uint8_t data);
	void save_state(ofstream &file);
	void load_state(ifstream &file);
};

//BOOM 2

class Memory
{
	private:

		// Dynamic Memory Controller
		MemoryController* controller;

		// Memory Regions
		vector<uint8_t> VRAM;		// $8000 - $9FFF, 8kB Video RAM
		vector<uint8_t> OAM;		// $FE00 - $FEA0, OAM Sprite RAM
		vector<uint8_t> WRAM;		// $C000 - $DFFF, 8kB Working RAM
		vector<uint8_t> ZRAM;		// $FF80 - $FFFF, 128 bytes of RAM

		void do_dma_transfer();
		uint8_t get_joypad_state();

	public:

		MemoryRegister
			P1,
			DIV, TIMA, TMA, TAC,
			LCDC, STAT, SCY, SCX, LYC, LY, DMA,
			BGP, OBP0, OBP1, WY, WX,
			IF, IE;

		uint8_t video_mode;
		uint8_t joypad_buttons;
		uint8_t joypad_arrows;

		string rom_name;

		Memory::Memory();
		void reset();
		void load_rom(std::string location);

		uint8_t read(uint16_t location);

		void write_vector(ofstream &file, vector<uint8_t> &vec);
		void load_vector(ifstream &file, vector<uint8_t> &vec);
		void save_state(ofstream &file);
		void load_state(ifstream &file);

		void write(uint16_t location, uint8_t data);
		void write_zero_page(uint16_t location, uint8_t data);
};