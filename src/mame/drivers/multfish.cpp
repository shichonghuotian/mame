// license:BSD-3-Clause
// copyright-holders:David Haywood, MetalliC
/*
 todo: bank handlers etc. should be installed on a per-game basis
       to make it clearer why all the sets with hacked bank
       setup existed in the wild

    note:
     this file contains the originals only
     for bootlegs see multfish_boot.c
     for reference information about undumped sets see multfish_Ref.c - if adding a new set ALWAYS check that, anything not listed in there is
                                                                        almost certainly a bootleg.

*/

/*

   Igrosoft gambling hardware

   +--+ +-----+ +-----------------------------------+
+--+  +-+PRINT+-+                                   |
|            +--------------+                       |
|            |     Z80B     |                       |
|      VOL   +----+---------+                       |
+-+ +-----------+ | PRG ROM |                       |
  | |  KC89C72  | +---------+                       |
+-+ +-----------+ | M48T35Y |  24MHz                -+
|                 +---------+                ADM690 V|
|                |---Connector------------------|   G|
|E                                                  A|
|d                                                  -+
|g                                                  |
|e                                                  |
|                                                   |
|C                                                  |
|o                                                  |
|n                                                  |
|n                                                  |
|e                                                  |
|c                           Connector              |
|t               |---Connector------------------|   |
|o           +------+ +------+ +------+ +------+    |
|r           |ALTERA| |ALTERA| |ALTERA| |ALTERA| R  |
|            | EPM  | | EPM  | | EPM  | | EPM  | A  |
|            | 3032 | | 3032 | | 3032 | | 3032 | M  |
|            +------+ +------+ +------+ +------+    |
+-+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ |
  | |   | |   | |   | |   | |   | |   | |   | |   | |
+-+ | 8 | | 7 | | 6 | | 5 | | 4 | | 3 | | 2 | | 1 | |
|   |   | |   | |   | |   | |   | |   | |   | |   | |
|   |   | |   | |   | |   | |   | |   | |   | |   | |
|   +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ |
+---------------------------------------------------+

"Head" sub board:
+------------------------------+
||--Connector-----------------||
|            61256AK-15        |
|                              |
|          +---------+         |
|          | ALTERA  |         |
|          |   MAX   |         |
|          | EPM3256 |         |
|          |         |         |
|          +---------+         |
|                              |
|           Connector          |
||--Connector-----------------||
+------------------------------+

  CPU: Z80
Video: ALTERA EPM3032ALC44-10 (x4) + ALTERA MAX EPM3256AOC208-10 (on the HEAD sub board)
       later board revisions comes with EPM3064 and EPM3512 ALTERA's
Sound: File KC98C72 (compatible to YM2149 / AY3-9810)
  OSC: 24MHz
  RAM: UMC UM61256AK-15 (x2 on mainboard, one on the HEAD)
NVRAM: Timekeeper M48T35-70PC1 (used as main Z80 RAM)
Other: ADM ADM690AN (Microprocessor Supervisory Circuit AKA Watchdog timer)
       VGA connector for video output

Edge Connector is 36 count dual (IE: both sides) JAMMA-like connection (standard 8-liner??)

  RAM has E000 address

  To Init the games

  Turn Service Mode ON (press 'F2')
  Reset the game (press 'F3')
  Use 'C' to move pointer to INIT
  Press '1' (Start) to enter INIT menu
  Hold 'Z' (Bet/Double) for 5 seconds while counter counts down
  Turn Service Mode OFF (press 'F2')
  Reset the game (press 'F3')

  To Init Roll Fruit

  Turn Service Mode ON (press 'F2')
  Press and hold Service 1 ('9')
  Reset the game (press 'F3')
  Use Service 2 ('0') to move pointer to INIT
  Hold Service 1 ('9') for 5 seconds
  Turn Service Mode OFF (press 'F2')

  Todo:
  -------------------------------------------------------------------------
  Layouts

  NOTE:
  Revision information comes from Igrosoft's website, not all of them can be
  tested because some program rom revisions don't seem to be dumped.

  For sets where the program rom hasn't been verified, the SHA1 hash is given
  but not the CRC32 hash.

Banking addresses are likely controlled via a GAL/PAL and was added at some
point to try and prevent rom swaps and conversions. Many of the hacked sets
below are simply made to the banking address to run on other boards.

  Bank addresses
  ---------------------
  Island 2           E1
  Gnome              E5
  Sweet Life 2       E8
  Fruit Cocktail 2   EA
  Multi Fish         F8
  Crazy Monkey       F9
  Fruit Cocktail     F9
  Garage             F9
  Resident           F9
  Lucky Hunter       F9
  Rock Climber       F9
  Roll Fruit         F9
  Pirate             FA
  Sweet Life         FA
  Island             FB
  Keks               FC
  Pirate 2           FD


  Edge Connector pinout

+-------------------------+--------------------------+
|     COMPONENT SIDE      |     SOLDER SIDE          |
+---+-----------------+---+---+------------------+---+
|   |                 |1A |1B |                  |   |
|   |                 |2A |2B |                  |   |
|Out|SPEAKER          |3A |3B |GND               |   |
|In |HOLD 1 SW        |4A |4B |BILL ACCEPTOR 1   |In |
|In |HOLD 2 SW        |5A |5B |BILL ACCEPTOR 2   |In |
|In |HOLD 3 SW        |6A |6B |BILL ACCEPTOR 3   |In |
|In |HOLD 4 SW        |7A |7B |BILL ACCEPTOR 4   |In |
|In |HOLD 5 SW        |8A |8B |HOPPER INHIBIT*   |In |
|In |START SW         |9A |9B |                  |In |
|In |BET/DOUBLE SW    |10A|10B|                  |In |
|In |reserved         |11A|11B|                  |In |
|In |reserved         |12A|12B|                  |In |
|In |reserved         |13A|13B|                  |In |
|In |MAXBET SW        |14A|14B|                  |In |
|In |HELP SW          |15A|15B|                  |In |
|In |FRONT DOOR SW    |16A|16B|                  |In |
|In |BACK DOOR SW     |17A|17B|                  |In |
|In |COIN A           |18A|18B|COIN B            |In |
|In |COIN C           |19A|19B|COIN D            |In |
|In |STATISTIC SW     |20A|20B|SERVICE SW        |In |
|In |PAY OUT SW*      |21A|21B|KEY OUT SW        |In |
|Out|                 |22A|22B|HOPPER COIN SW    |In |
|Out|COIN+BILL COUNTER|23A|23B|COIN LOCK         |Out|
|Out|KEY IN COUNTER   |24A|24B|BILL ACCEPTOR LOCK|Out|
|Out|TOTAL IN COUNTER |25A|25B|UPPER LAMP GREEN  |Out|
|Out|TOTAL OUT COUNTER|26A|26B|UPPER LAMP RED    |Out|
|Out|KEY OUT COUNTER  |27A|27B|UPPER LAMP YELLOW |Out|
|Out|                 |28A|28B|TOTAL BET COUNTER |Out|
|Out|HOLD 1 LAMP      |29A|29B|BET / DOUBLE LAMP |Out|
|Out|HOLD 2 LAMP      |30A|30B|MAXBET LAMP       |Out|
|Out|HOLD 3 LAMP      |31A|31B|PAYOUT LAMP       |Out|
|Out|HOLD 4 LAMP      |32A|32B|                  |Out|
|Out|HOLD 5 LAMP      |33A|33B|HOPPER MOTOR      |Out|
|Out|START LAMP       |34A|34B|HELP LAMP         |Out|
|In |KEY IN SW        |35A|35B|                  |In |
|   |GND              |36A|36B|GND               |   |
+---+-----------------+---+---+------------------+---+
*/

#include "emu.h"
#include "includes/multfish.h"

#include "speaker.h"


TILE_GET_INFO_MEMBER(igrosoft_gamble_state::get_igrosoft_gamble_tile_info)
{
	int code = m_vid[tile_index*2+0x0000] | (m_vid[tile_index*2+0x0001] << 8);
	int attr = m_vid[tile_index*2+0x1000] | (m_vid[tile_index*2+0x1001] << 8);

	tileinfo.category = (attr&0x100)>>8;

	tileinfo.set(0,
			code&0x1fff,
			attr&0x7,
			0);
}

TILE_GET_INFO_MEMBER(igrosoft_gamble_state::get_igrosoft_gamble_reel_tile_info)
{
	int code = m_vid[tile_index*2+0x2000] | (m_vid[tile_index*2+0x2001] << 8);

	tileinfo.set(0,
			(code&0x1fff)+0x2000,
			(code>>14)+0x8,
			0);
}

void igrosoft_gamble_state::video_start()
{
	memset(m_vid,0x00,sizeof(m_vid));
	save_item(NAME(m_vid));

	m_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(igrosoft_gamble_state::get_igrosoft_gamble_tile_info)), TILEMAP_SCAN_ROWS, 16,16, 64, 32);
	m_tilemap->set_transparent_pen(255);

	m_reel_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(igrosoft_gamble_state::get_igrosoft_gamble_reel_tile_info)), TILEMAP_SCAN_ROWS, 16,16, 64, 64);
	m_reel_tilemap->set_transparent_pen(255);
	m_reel_tilemap->set_scroll_cols(64);
}

uint32_t igrosoft_gamble_state::screen_update_igrosoft_gamble(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	int i;

	bitmap.fill(m_palette->black_pen(), cliprect);

	if (!m_disp_enable) return 0;

	/* Draw lower part of static tilemap (low pri tiles) */
	m_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_CATEGORY(1),0);

	/* Setup the column scroll and draw the reels */
	for (i=0;i<64;i++)
	{
		int colscroll = (m_vid[i*2] | m_vid[i*2+1] << 8);
		m_reel_tilemap->set_scrolly(i, colscroll );
	}
	m_reel_tilemap->draw(screen, bitmap, cliprect, 0,0);

	/* Draw upper part of static tilemap (high pri tiles) */
	m_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_CATEGORY(0),0);

	return 0;
}

void igrosoft_gamble_state::igrosoft_gamble_vid_w(offs_t offset, uint8_t data)
{
	m_vid[offset]=data;

	// 0x0000 - 0x1fff is normal tilemap
	if (offset < 0x2000)
	{
		m_tilemap->mark_tile_dirty((offset&0xfff)/2);

	}
	// 0x2000 - 0x2fff is for the reels
	else if (offset < 0x4000)
	{
		m_reel_tilemap->mark_tile_dirty((offset&0x1fff)/2);
	}
	else if (offset < 0x6000)
	{
		int r,g,b;
		int coldat;

		coldat = m_vid[(offset&0xfffe)] | (m_vid[(offset&0xfffe)^1] << 8);

		/* xor and bitswap palette */
		switch (m_xor_paltype) {
			case 1:
				coldat ^= m_xor_palette;
				coldat ^= ((coldat&0x2) >>1) | ((coldat&0x80) >>3) ;
				coldat = bitswap<16>(coldat,10,15,5,13,8,12,11,2,0,4,7,14,9,3,1,6);
				break;
			case 2:
				coldat ^= m_xor_palette;
				coldat ^= ((coldat&0x0001) <<1) ^ ((coldat&0x0010) <<1) ^ ((coldat&0x0010) <<2) ^ ((coldat&0x0020) <<1) ^ ((coldat&0x0080) >>1);
				coldat = bitswap<16>(coldat,4,10,13,14,8,11,15,12,2,6,5,0,7,3,1,9);
				break;
			case 3:
				// WRONG
				coldat ^= m_xor_palette;
				//if (offset&1) printf("col %04x, %04x\n", (offset-0x4000), coldat);
				break;
		}

		r = ( (coldat &0x001f)>> 0);
		g = ( (coldat &0x1f00)>> 8);
		b = ( (coldat &0x00e0)>> (5));
		b|= ( (coldat &0xe000)>> (8+5-3));

		m_palette->set_pen_color((offset-0x4000)/2, r<<3, g<<3, b<<2);
	}
	else
	{
		// probably just work ram
	}
}

void igrosoft_gamble_state::igrosoft_gamble_bank_w(uint8_t data)
{
	membank("bank1")->set_entry(data & 0x0f);
}

uint8_t igrosoft_gamble_state::igrosoft_gamble_timekeeper_r(offs_t offset)
{
	return m_m48t35->read(offset + 0x6000);
}

void igrosoft_gamble_state::igrosoft_gamble_timekeeper_w(offs_t offset, uint8_t data)
{
	m_m48t35->write(offset + 0x6000, data);
}

uint8_t igrosoft_gamble_state::bankedram_r(offs_t offset)
{
	if ((m_rambk & 0x80) == 0x00)
	{
		return m_m48t35->read(offset + 0x2000*(m_rambk & 0x03));
	}
	else
	{
		return m_vid[offset+0x2000*(m_rambk & 0x03)];
	}

}

void igrosoft_gamble_state::bankedram_w(offs_t offset, uint8_t data)
{
	if ((m_rambk & 0x80) == 0x00)
	{
		m_m48t35->write(offset + 0x2000*(m_rambk & 0x03), data);
	}
	else
	{
		igrosoft_gamble_vid_w(offset+0x2000*(m_rambk & 0x03), data);
	}
}

void igrosoft_gamble_state::igrosoft_gamble_rambank_w(uint8_t data)
{
	m_rambk = data;
}


uint8_t igrosoft_gamble_state::ray_r()
{
	// the games read the raster beam position as part of the hardware checks..
	// with a 6mhz clock and 640x480 resolution this seems to give the right results.
	return m_screen->vpos();
}

void igrosoft_gamble_state::igrosoft_gamble_hopper_w(uint8_t data)
{
/*  Port 0x33

    7654 3210
    ---- ---X Coin Lock 23B
    ---- -X-- Bill Acceptor Lock 24B
    ---X ---- Hopper Motor 33B
*/


	m_hopper->motor_w(data & 0x10);
	machine().bookkeeping().coin_lockout_w(0, data & 0x01);
	machine().bookkeeping().coin_lockout_w(1, data & 0x01);
	machine().bookkeeping().coin_lockout_w(2, data & 0x01);
	machine().bookkeeping().coin_lockout_w(3, data & 0x01);
	machine().bookkeeping().coin_lockout_w(4, data & 0x04);
	machine().bookkeeping().coin_lockout_w(5, data & 0x04);
	machine().bookkeeping().coin_lockout_w(6, data & 0x04);
	machine().bookkeeping().coin_lockout_w(7, data & 0x04);
}

void igrosoft_gamble_state::rollfr_hopper_w(uint8_t data)
{
/*
    By default RollFruit use inverted coinlock bit.
*/


	m_hopper->motor_w(data & 0x10);
	machine().bookkeeping().coin_lockout_w(0,~data & 0x01);
	machine().bookkeeping().coin_lockout_w(1,~data & 0x01);
	machine().bookkeeping().coin_lockout_w(2,~data & 0x01);
	machine().bookkeeping().coin_lockout_w(3,~data & 0x01);
	machine().bookkeeping().coin_lockout_w(4, data & 0x04);
	machine().bookkeeping().coin_lockout_w(5, data & 0x04);
	machine().bookkeeping().coin_lockout_w(6, data & 0x04);
	machine().bookkeeping().coin_lockout_w(7, data & 0x04);
}

void igrosoft_gamble_state::init_customl()
{
/*
rom 1
 swap msb/lsb
D0 = D0 xor A15
D1 = D1 xor A14
D2 = D2 xor A07
D3 = D3 xor A06

rom 3
 swap msb/lsb
D0 = D0 xor A16
D1 = D1 xor A17
D2 = D2 xor A08
D3 = D3 xor A09

rom 2,4
msb = msb xor lsb

All roms address lines swapped:
A06 <-> A15
A07 <-> A14
A08 <-> A16
A09 <-> A17
A10 <-> A18
A12 <-> A13
*/

	uint32_t i,j,jscr,romoffset;
	uint8_t *igrosoft_gamble_gfx = memregion("gfx")->base();
	std::vector<uint8_t> temprom(igrosoft_gamble_ROM_SIZE);


	/* ROM 1 decode */
	romoffset = 0x000000;
	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		jscr = ((i & 0x8000)>>15) | ((i & 0x4000)>>13) | ((i & 0x0080)>>5) | ((i & 0x0040)>>3);
		igrosoft_gamble_gfx[romoffset+i] = (((0x0f & igrosoft_gamble_gfx[romoffset+i])<<4) | ((0xf0 & igrosoft_gamble_gfx[romoffset+i])>>4)) ^ jscr;
	}
		/* ROM 2 decode */
	romoffset = 0x100000;
	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		igrosoft_gamble_gfx[romoffset+i] ^=  (0x0f & igrosoft_gamble_gfx[romoffset+i])<<4;
	}
	/* ROM 3 decode */
	romoffset = 0x200000;
	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		jscr = ((i & 0x300)>>6) | ((i & 0x30000)>>16);
		igrosoft_gamble_gfx[romoffset+i] = (((0x0f & igrosoft_gamble_gfx[romoffset+i])<<4) | ((0xf0 & igrosoft_gamble_gfx[romoffset+i])>>4)) ^ jscr;
	}
	/* ROM 4 decode */
	romoffset = 0x300000;
	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		igrosoft_gamble_gfx[romoffset+i] ^= (0x0f & igrosoft_gamble_gfx[romoffset+i])<<4;
	}

	/* Deshuffle all roms*/
	for (i = 0;i < 8;i++)
	{
		romoffset = i * igrosoft_gamble_ROM_SIZE;

		for (j = 0; j < (igrosoft_gamble_ROM_SIZE/0x40); j++)
		{
			jscr =  bitswap<16>(j,15,14,13,4,3,2,0,1,6,7,5,12,11,10,8,9);
			memcpy(&temprom[j*0x40],&igrosoft_gamble_gfx[romoffset+(jscr*0x40)],0x40);

		}
		memcpy(&igrosoft_gamble_gfx[romoffset],&temprom[0],igrosoft_gamble_ROM_SIZE);
	}
}

static inline void rom_decodel(uint8_t *romptr, uint8_t *tmprom, uint8_t xor_data, uint32_t xor_add)
{
	uint32_t i, jscr;

	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		jscr =  bitswap<24>(i,23,22,21,20,19,17,14,18,16,15,12,13,11,9,6,10,8,7,4,5,3,2,1,0) ^ xor_add ^ 8;
		tmprom[i] = romptr[jscr] ^ xor_data;
	}
	memcpy(romptr,tmprom,igrosoft_gamble_ROM_SIZE);
}
static inline void rom_decodeh(uint8_t *romptr, uint8_t *tmprom, uint8_t xor_data, uint32_t xor_add)
{
	uint32_t i, jscr;

	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		jscr =  bitswap<24>(i,23,22,21,20,19,17,14,18,16,15,12,13,11,9,6,10,8,7,4,5,2,3,1,0) ^ xor_add;
		tmprom[i] = romptr[jscr] ^ xor_data;
	}
	memcpy(romptr,tmprom,igrosoft_gamble_ROM_SIZE);
}

static void lottery_decode(running_machine &machine, uint8_t xor12, uint8_t xor34, uint8_t xor56, uint8_t xor78, uint32_t xor_addr)
{
	uint8_t *igrosoft_gamble_gfx = machine.root_device().memregion("gfx")->base();
	std::vector<uint8_t> temprom(igrosoft_gamble_ROM_SIZE);

	/* ROMs decode */
	rom_decodel(&igrosoft_gamble_gfx[0x000000], &temprom[0], xor12, xor_addr);
	rom_decodel(&igrosoft_gamble_gfx[0x100000], &temprom[0], xor12, xor_addr);
	rom_decodel(&igrosoft_gamble_gfx[0x200000], &temprom[0], xor34, xor_addr);
	rom_decodel(&igrosoft_gamble_gfx[0x300000], &temprom[0], xor34, xor_addr);
	rom_decodeh(&igrosoft_gamble_gfx[0x080000], &temprom[0], xor56, xor_addr);
	rom_decodeh(&igrosoft_gamble_gfx[0x180000], &temprom[0], xor56, xor_addr);
	rom_decodeh(&igrosoft_gamble_gfx[0x280000], &temprom[0], xor78, xor_addr);
	rom_decodeh(&igrosoft_gamble_gfx[0x380000], &temprom[0], xor78, xor_addr);
}

static inline void roment_decodel(uint8_t *romptr, uint8_t *tmprom, uint8_t xor_data, uint32_t xor_add)
{
	uint32_t i, jscr;

	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		jscr =  bitswap<24>(i,23,22,21,20,19,16,18,17,14,15,12,13,11,8,10,9,6,7,4,5,3,2,1,0) ^ xor_add ^ 8;
		tmprom[i] = romptr[jscr] ^ xor_data;
	}
	memcpy(romptr,tmprom,igrosoft_gamble_ROM_SIZE);
}
static inline void roment_decodeh(uint8_t *romptr, uint8_t *tmprom, uint8_t xor_data, uint32_t xor_add)
{
	uint32_t i, jscr;

	for (i = 0; i < igrosoft_gamble_ROM_SIZE; i++)
	{
		jscr =  bitswap<24>(i,23,22,21,20,19,16,18,17,14,15,12,13,11,8,10,9,6,7,4,5,2,3,1,0) ^ xor_add;
		tmprom[i] = romptr[jscr] ^ xor_data;
	}
	memcpy(romptr,tmprom,igrosoft_gamble_ROM_SIZE);
}

static void ent_decode(running_machine &machine, uint8_t xor12, uint8_t xor34, uint8_t xor56, uint8_t xor78, uint32_t xor_addr)
{
	uint8_t *igrosoft_gamble_gfx = machine.root_device().memregion("gfx")->base();
	std::vector<uint8_t> temprom(igrosoft_gamble_ROM_SIZE);

	/* ROMs decode */
	roment_decodel(&igrosoft_gamble_gfx[0x000000], &temprom[0], xor12, xor_addr);
	roment_decodel(&igrosoft_gamble_gfx[0x100000], &temprom[0], xor12, xor_addr);
	roment_decodel(&igrosoft_gamble_gfx[0x200000], &temprom[0], xor34, xor_addr);
	roment_decodel(&igrosoft_gamble_gfx[0x300000], &temprom[0], xor34, xor_addr);
	roment_decodeh(&igrosoft_gamble_gfx[0x080000], &temprom[0], xor56, xor_addr);
	roment_decodeh(&igrosoft_gamble_gfx[0x180000], &temprom[0], xor56, xor_addr);
	roment_decodeh(&igrosoft_gamble_gfx[0x280000], &temprom[0], xor78, xor_addr);
	roment_decodeh(&igrosoft_gamble_gfx[0x380000], &temprom[0], xor78, xor_addr);
}

void igrosoft_gamble_state::init_island2l()
{
	m_xor_palette = 0x8bf7;
	m_xor_paltype = 1;
	lottery_decode(machine(), 0xff, 0x11, 0x77, 0xee, 0x44c40);
}
void igrosoft_gamble_state::init_keksl()
{
	m_xor_palette = 0x41f3;
	m_xor_paltype = 1;
	lottery_decode(machine(), 0xdd, 0xaa, 0x22, 0x55, 0x2cac0);
}
void igrosoft_gamble_state::init_pirate2l()
{
	m_xor_palette = 0x8bfb;
	m_xor_paltype = 1;
	lottery_decode(machine(), 0xaa, 0x11, 0x22, 0xee, 0x48480);
}
void igrosoft_gamble_state::init_fcockt2l()
{
	m_xor_palette = 0xedfb;
	m_xor_paltype = 1;
	lottery_decode(machine(), 0x55, 0x11, 0xff, 0xee, 0x78780);
}
void igrosoft_gamble_state::init_sweetl2l()
{
	m_xor_palette = 0x4bf7;
	m_xor_paltype = 1;
	lottery_decode(machine(), 0xdd, 0x33, 0x33, 0x77, 0x00800);
}
void igrosoft_gamble_state::init_gnomel()
{
	m_xor_palette = 0x49ff;
	m_xor_paltype = 1;
	lottery_decode(machine(), 0xcc, 0x22, 0x33, 0x66, 0x14940);
}
void igrosoft_gamble_state::init_crzmonent()
{
	m_xor_palette = 0x1cdb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0xaa, 0x44, 0x55, 0x55, 0x1c9c0);
}
void igrosoft_gamble_state::init_fcocktent()
{
	m_xor_palette = 0x2cdb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x77, 0x55, 0x22, 0x44, 0x18180);
}
void igrosoft_gamble_state::init_garageent()
{
	m_xor_palette = 0x7adb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x88, 0x66, 0x66, 0x99, 0x28280);
}
void igrosoft_gamble_state::init_rclimbent()
{
	m_xor_palette = 0x5edb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x55, 0xaa, 0x44, 0xff, 0x74740);
}
void igrosoft_gamble_state::init_sweetl2ent()
{
	m_xor_palette = 0xdcdb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0xee, 0x77, 0x88, 0x11, 0x5c5c0);
}
void igrosoft_gamble_state::init_resdntent()
{
	m_xor_palette = 0x6edb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0xaa, 0xcc, 0xaa, 0xaa, 0x78780);
}
void igrosoft_gamble_state::init_island2ent()
{
	m_xor_palette = 0xecdb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x88, 0x55, 0xff, 0x99, 0x58d80);
}
void igrosoft_gamble_state::init_pirate2ent()
{
	m_xor_palette = 0xbadb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x33, 0xbb, 0x77, 0x55, 0x68e80);
}
void igrosoft_gamble_state::init_keksent()
{
	m_xor_palette = 0xaedb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x55, 0xff, 0xaa, 0x22, 0x38b80);
}
void igrosoft_gamble_state::init_gnomeent()
{
	m_xor_palette = 0x9edb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x22, 0x77, 0x11, 0xbb, 0x34b40);
}
void igrosoft_gamble_state::init_lhauntent()
{
	m_xor_palette = 0x1adb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x22, 0x44, 0x44, 0xbb, 0x24240);
}
void igrosoft_gamble_state::init_fcockt2ent()
{
	m_xor_palette = 0x7cdb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x33, 0xcc, 0xaa, 0x88, 0x14140);
}
void igrosoft_gamble_state::init_sweetlent()
{
	m_xor_palette = 0xeadb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x44, 0xdd, 0xdd, 0x22, 0x6c6c0);
}
void igrosoft_gamble_state::init_islandent()
{
	m_xor_palette = 0xdadb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x66, 0x22, 0x33, 0xcc, 0x64e40);
}
void igrosoft_gamble_state::init_pirateent()
{
	m_xor_palette = 0xbcdb;
	m_xor_paltype = 2;
	ent_decode(machine(), 0x99, 0x22, 0xee, 0x66, 0x54d40);
}

void igrosoft_gamble_state::init_rollfruit()
{
	// m_xor_palette = 0xbff7;
	// needs gfx and palette descrambles
	// encryption looks similar to Crazy Monkey 2
}

void igrosoft_gamble_state::init_crzmon2()
{
	m_xor_paltype = 3;
	m_xor_palette = 0xaff7;
	// needs gfx (and palette) descrambles
}

void igrosoft_gamble_state::init_crzmon2lot()
{
	m_xor_paltype = 3;
	m_xor_palette = 0xddf7;
	// needs gfx (and palette) descrambles
}

void igrosoft_gamble_state::init_crzmon2ent()
{
	m_xor_paltype = 3;
	m_xor_palette = 0x4df7;
	// needs gfx (and palette) descrambles
}

void igrosoft_gamble_state::igrosoft_gamble_map(address_map &map)
{
	map(0x0000, 0x7fff).rom().w(FUNC(igrosoft_gamble_state::igrosoft_gamble_vid_w));
	map(0x8000, 0xbfff).bankr("bank1");
	map(0xc000, 0xdfff).rw(FUNC(igrosoft_gamble_state::igrosoft_gamble_timekeeper_r), FUNC(igrosoft_gamble_state::igrosoft_gamble_timekeeper_w));
	map(0xe000, 0xffff).rw(FUNC(igrosoft_gamble_state::bankedram_r), FUNC(igrosoft_gamble_state::bankedram_w));
}

// According to control panel the user buttons are arranged as
// Maxbet | Help | Payout |
// Bet/Cancel  |  1 Line  |  3 Lines  |  5 Lines  | 7 Lines  | 9 Lines  | Start |


INPUT_PORTS_START( igrosoft_gamble )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_KEYIN ) PORT_TOGGLE // Key In ( 35 A )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 ) PORT_IMPULSE(2) // COIN B (18 B)
	PORT_DIPNAME(     0x04, 0x04, "S Reserve (14 B)" ) // S Reserve ( 14 B )
	PORT_DIPSETTING(  0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x08, 0x08, "Hopper 3 ( 10 B )" ) // Hooper 3 ( 10 B )
	PORT_DIPSETTING(  0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SLOT_STOP4 ) PORT_NAME("7 Lines")
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("hopper", ticket_dispenser_device, line_r)
	PORT_DIPNAME(     0x02, 0x02, "BK Door (17 A)"  )
	PORT_DIPSETTING(  0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x04, 0x04, "P Reserve (13 A)" )
	PORT_DIPSETTING(  0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_START1 ) PORT_NAME("Start / Take")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN8 ) PORT_IMPULSE(2) // BILL 4 (07 A)
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN2")
	PORT_DIPNAME(     0x01, 0x01, "Unused??" ) // unused?
	PORT_DIPSETTING(  0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x02, 0x02, "Call Att (17 A)" )
	PORT_DIPSETTING(  0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x04, 0x04, "S Reserve (13 B)" )
	PORT_DIPSETTING(  0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x08, 0x08, "Hopper 2 (09 B)" )
	PORT_DIPSETTING(  0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SLOT_STOP3 ) PORT_NAME("5 Lines")
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_PAYOUT )
	PORT_DIPNAME(     0x02, 0x02, "S Reserve (16 B)" )
	PORT_DIPSETTING(  0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x04, 0x04, "Ticket (12 B)" )
	PORT_DIPSETTING(  0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x08, 0x08, "Hopper 1 (08 B)" )
	PORT_DIPSETTING(  0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN5 ) PORT_IMPULSE(2) // BILL 1 (04 B)
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN4")
	PORT_DIPNAME(     0x01, 0x01, "S Reserve (35 B)" )
	PORT_DIPSETTING(  0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN3 ) PORT_IMPULSE(2) // COIN C (19 A)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_GAMBLE_LOW ) PORT_NAME("Help")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_POKER_CANCEL ) PORT_NAME("9 Lines") // must be IPT_SLOT_STOP5
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SLOT_STOP1 ) PORT_NAME("1 Line")
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN5")
	PORT_SERVICE(     0x01, IP_ACTIVE_LOW )
	PORT_BIT(         0x02, IP_ACTIVE_LOW, IPT_COIN4 ) PORT_IMPULSE(2) // COIN D (19 B)
	PORT_DIPNAME(     0x04, 0x04, "S Reserve (16 B)" )
	PORT_DIPSETTING(  0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_SLOT_STOP_ALL ) PORT_NAME("Bet / Double / Cancel")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN6 ) PORT_IMPULSE(2) // BILL 2 (05 A)
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_BOOK ) PORT_NAME("Short Statistic") // Short St (20 A)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN1 ) PORT_IMPULSE(2) // COIN A (18 A)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_GAMBLE_HIGH ) PORT_NAME("Max Bet")
	PORT_DIPNAME(     0x08, 0x08, "Hopper 4 (11 A)" )
	PORT_DIPSETTING(  0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SLOT_STOP2 ) PORT_NAME("3 Lines")
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?

	PORT_START("IN7")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_KEYOUT ) // Key Out (21 B)
	PORT_DIPNAME(     0x02, 0x02, "Fr Door (16 A)" )
	PORT_DIPSETTING(  0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x04, 0x04, "P Reserve (12 A)" )
	PORT_DIPSETTING(  0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_DIPNAME(     0x08, 0x08, "P Reserve (11 A)" )
	PORT_DIPSETTING(  0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN7 ) PORT_IMPULSE(2) // BILL 3 (06 A)
	PORT_BIT( 0xe0, IP_ACTIVE_LOW, IPT_UNUSED ) // unused?
INPUT_PORTS_END

static INPUT_PORTS_START( rollfr )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN1 ) PORT_IMPULSE(2) // COIN B (18 B)
	PORT_BIT( 0xfc, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("hopper", ticket_dispenser_device, line_r)
	PORT_DIPNAME(     0x02, 0x02, "BK Door (17 A)"  )
	PORT_DIPSETTING(  0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0xfc, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN2")
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN3")
	PORT_DIPNAME(     0x08, 0x08, "Hopper Inhibit (08 B)" )
	PORT_DIPSETTING(  0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(  0x00, DEF_STR( On ) )
	PORT_BIT( 0xf7, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN4")
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_SERVICE ) //Service SW (20 B)
	PORT_BIT( 0xfe, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_BOOK ) //Statistic SW (20 A)
	PORT_BIT( 0xfe, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN7")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_SERVICE( 0x02, IP_ACTIVE_LOW ) // Fr Door (16 A)
	PORT_BIT( 0xfc, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END


void igrosoft_gamble_state::igrosoft_gamble_lamps1_w(uint8_t data)
{
/*  Port 0x30

    7654 3210
    ---- ---X Hold 1 Lamp 29A
    ---- --X- Hold 2 Lamp 30A
    ---- -X-- Hold 3 Lamp 31A
    ---- X--- Hold 4 Lamp 32A
    ---X ---- Hold 5 Lamp 33A
    --X- ---- Help Lamp 34B
    -X-- ---- Start Lamp 34A
    X--- ---- Bet/Double Lamp 29B
*/
	m_lamps[1] = BIT(data, 0); /* Hold 1 Lamp */
	m_lamps[2] = BIT(data, 1); /* Hold 2 Lamp */
	m_lamps[3] = BIT(data, 2); /* Hold 3 Lamp */
	m_lamps[4] = BIT(data, 3); /* Hold 4 Lamp */
	m_lamps[5] = BIT(data, 4); /* Hold 5 Lamp */
	m_lamps[8] = BIT(data, 5); /* Help Lamp */
	m_lamps[6] = BIT(data, 6); /* Start Lamp */
	m_lamps[0] = BIT(data, 7); /* Bet/Double Lamp */
}

void igrosoft_gamble_state::igrosoft_gamble_lamps2_w(uint8_t data)
{
/*  Port 0x34

    7654 3210
    ---- ---X Payout Lamp 31B
    ---- --X- Upper Lamp Yellow 27B (Hopper Error)
    ---- -X-- Maxbet Lamp 30B
    ---X ---- Upper Lamp Green 25B  (Demo Mode)
*/
	m_lamps[9] = BIT(data, 0);  /* Payout Lamp */
	m_lamps[12] = BIT(data, 1); /* Upper Lamp Yellow */
	m_lamps[7] = BIT(data, 2);  /* Maxbet Lamp */
	m_lamps[10] = BIT(data, 4); /* Upper Lamp Green */
}

void igrosoft_gamble_state::igrosoft_gamble_lamps3_w(uint8_t data)
{
/*  Port 0x35

    7654 3210
    ---- --X- Upper Lamp Red 26B (Service Mode)
*/
	m_lamps[11] = BIT(data, 1); /* Upper Lamp Red */
}

void igrosoft_gamble_state::igrosoft_gamble_counters_w(uint8_t data)
{
/*  Port 0x31

    7654 3210
    ---- ---X Total In Counter 25A
    ---- --X- Coin+Bill Counter 23A
    ---- -X-- Key In Counter 24A
    ---X ---- Total Out Counter 26A
    -X-- ---- Key Out Counter 27A
    X--- ---- Total Bet Counter 28B
*/
		machine().bookkeeping().coin_counter_w(0, data & 0x01);
		machine().bookkeeping().coin_counter_w(1, data & 0x02);
		machine().bookkeeping().coin_counter_w(2, data & 0x04);
		machine().bookkeeping().coin_counter_w(3, data & 0x10);
		machine().bookkeeping().coin_counter_w(4, data & 0x40);
		machine().bookkeeping().coin_counter_w(5, data & 0x80);
}

void igrosoft_gamble_state::igrosoft_gamble_f3_w(uint8_t data)
{
	//popmessage("igrosoft_gamble_f3_w %02x",data);
}

void igrosoft_gamble_state::igrosoft_gamble_dispenable_w(uint8_t data)
{
	//popmessage("igrosoft_gamble_f4_w %02x",data); // display enable?
	m_disp_enable = data;
}

void igrosoft_gamble_state::igrosoft_gamble_portmap(address_map &map)
{
	map.global_mask(0xff);
	map(0x10, 0x10).portr("IN0");
	map(0x11, 0x11).portr("IN1");
	map(0x12, 0x12).portr("IN2");
	map(0x13, 0x13).portr("IN3");
	map(0x14, 0x14).portr("IN4");
	map(0x15, 0x15).portr("IN5");
	map(0x16, 0x16).portr("IN6");
	map(0x17, 0x17).portr("IN7");

	/* Write ports not hooked up yet */
	map(0x30, 0x30).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_lamps1_w));
		map(0x31, 0x31).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_counters_w));
//  map(0x32, 0x32).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_port32_w));
	map(0x33, 0x33).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_hopper_w));
	map(0x34, 0x34).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_lamps2_w));
	map(0x35, 0x35).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_lamps3_w));
//  map(0x36, 0x36).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_port36_w));
	map(0x37, 0x37).w("watchdog", FUNC(watchdog_timer_device::reset_w));
	map(0x38, 0x38).w("aysnd", FUNC(ay8910_device::address_w));
	map(0x39, 0x39).w("aysnd", FUNC(ay8910_device::data_w));
	map(0x3a, 0x3a).r("aysnd", FUNC(ay8910_device::data_r));

	map(0x60, 0x60).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_dispenable_w)); // display enable mirror for lottery sets

	map(0x90, 0x90).r(FUNC(igrosoft_gamble_state::ray_r));

	map(0xa0, 0xa0).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Crazy Monkey 2 banking
	map(0xa5, 0xa5).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Crazy Monkey 2 Ent banking
	map(0xb0, 0xb0).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Fruit Cocktail 2 lottery banking
	map(0xb1, 0xb1).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Crazy Monkey Ent banking
	map(0xb2, 0xb2).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Lacky Haunter Ent banking
	map(0xb3, 0xb3).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Fruit Cocktail Ent banking
	map(0xb4, 0xb4).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Fruit Cocktail 2 Ent banking
	map(0xb5, 0xb5).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Garage Ent banking
	map(0xb6, 0xb6).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Resident Ent banking
	map(0xb7, 0xb7).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Rock Climber Ent banking
	map(0xb8, 0xb8).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Sweet Life Ent banking
	map(0xb9, 0xb9).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Sweet Life 2 Ent banking
	map(0xba, 0xba).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Island Ent banking
	map(0xbb, 0xbb).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Island 2 Ent banking
	map(0xbc, 0xbc).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Pirate Ent banking
	map(0xbd, 0xbd).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Pirate 2 Ent banking
	map(0xbe, 0xbe).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Keks Ent banking
	map(0xbf, 0xbf).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Gnome Ent banking
	map(0xc7, 0xc7).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Resident lottery banking
	map(0xca, 0xca).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Gnome lottery banking
	map(0xcb, 0xcb).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Keks lottery banking
	map(0xcc, 0xcc).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Sweet Life 2 lottery banking
	map(0xcd, 0xcd).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Island 2 lottery banking
	map(0xce, 0xce).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Pirate 2 lottery banking
	map(0xd0, 0xd0).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // rollfr_4 rollfr_5 banking
	map(0xe1, 0xe1).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Island 2 banking
	map(0xe5, 0xe5).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Gnome banking
	map(0xe8, 0xe8).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Sweet Life 2 banking
	map(0xea, 0xea).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Fruit Cocktail 2 banking
	map(0xec, 0xec).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Crazy Monkey lottery banking

	map(0xf0, 0xf0).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w)); // Gold Fish banking
	map(0xf1, 0xf1).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_rambank_w));
	map(0xf3, 0xf3).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_f3_w)); // from 00->01 at startup, irq enable maybe?
	map(0xf4, 0xf4).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_dispenable_w)); // display enable

	/* mirrors of the rom banking */
	map(0xf8, 0xfd).w(FUNC(igrosoft_gamble_state::igrosoft_gamble_bank_w));
}

void igrosoft_gamble_state::rollfr_portmap(address_map &map)
{
	igrosoft_gamble_portmap(map);
	map(0x33, 0x33).w(FUNC(igrosoft_gamble_state::rollfr_hopper_w));
}

static const gfx_layout tiles16x16_layout =
{
	16,16,
	RGN_FRAC(1,4),
	8,
	{ RGN_FRAC(2,4)+0, RGN_FRAC(2,4)+1,RGN_FRAC(2,4)+2, RGN_FRAC(2,4)+3,0,1,2,3 },
	{ 0,4,
		RGN_FRAC(1,4)+0, RGN_FRAC(1,4)+4,
		8, 12,
		RGN_FRAC(1,4)+8, RGN_FRAC(1,4)+12,
		16, 20,
		RGN_FRAC(1,4)+16, RGN_FRAC(1,4)+20,
		24, 28,
		RGN_FRAC(1,4)+24,RGN_FRAC(1,4)+28 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32,8*32,9*32,10*32,11*32,12*32,13*32,14*32,15*32 },
	8*64
};


static GFXDECODE_START( gfx_igrosoft_gamble )
	GFXDECODE_ENTRY( "gfx", 0, tiles16x16_layout, 0, 16 )
GFXDECODE_END

void igrosoft_gamble_state::machine_start()
{
	m_lamps.resolve();

	save_item(NAME(m_disp_enable));
	save_item(NAME(m_rambk));
}

void igrosoft_gamble_state::machine_reset()
{
	membank("bank1")->configure_entries(0, 16, memregion("maincpu")->base(), 0x4000);
	membank("bank1")->set_entry(0);

	m_disp_enable = 0;
	m_rambk = 0;
}

void igrosoft_gamble_state::igrosoft_gamble(machine_config &config)
{
	/* basic machine hardware */
	Z80(config, m_maincpu, XTAL(24'000'000)/4);
	m_maincpu->set_addrmap(AS_PROGRAM, &igrosoft_gamble_state::igrosoft_gamble_map);
	m_maincpu->set_addrmap(AS_IO, &igrosoft_gamble_state::igrosoft_gamble_portmap);
	m_maincpu->set_vblank_int("screen", FUNC(igrosoft_gamble_state::irq0_line_hold));

	WATCHDOG_TIMER(config, "watchdog");

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_refresh_hz(60);
	m_screen->set_vblank_time(ATTOSECONDS_IN_USEC(0));
	m_screen->set_size(64*16, 32*16);
	m_screen->set_visarea(17*16, 1024-16*7-1, 1*16, 32*16-1*16-1);
	m_screen->set_screen_update(FUNC(igrosoft_gamble_state::screen_update_igrosoft_gamble));
	m_screen->set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_igrosoft_gamble);
	PALETTE(config, m_palette).set_entries(0x1000);


	SPEAKER(config, "mono").front_center();
	AY8910(config, "aysnd", 6000000/4).add_route(ALL_OUTPUTS, "mono", 0.30);

	M48T35(config, m_m48t35, 0);
	HOPPER(config, m_hopper, attotime::from_msec(100), TICKET_MOTOR_ACTIVE_HIGH, TICKET_STATUS_ACTIVE_HIGH);
}

void igrosoft_gamble_state::rollfr(machine_config &config)
{
	igrosoft_gamble(config);
	m_maincpu->set_addrmap(AS_IO, &igrosoft_gamble_state::rollfr_portmap);
}



/* Rom Naming note:

The GFX ROMs do not have labels, for clarity we name
them as on Igrosoft web-site hash tables.

code roms:
xx_m_xxxxxx.rom     - world relase
xx_xxxxxx.rom       - release for Russia (or for all, if the game does not have different roms for different regions)
xx_l_xxxxxx.rom     - lottery game
xx_e_xxxxxx.rom     - entertainment game
x,n,a,b,c           - another entartainment revisions

graphics roms:
xxxxxxxx_m.00x      - roms for world relase
xxxxxxxx.00x        - release for Russia (or for all, if the game does not have special gfx-sets for different regions)
xxxxxxxx_loto.00x   - lottery sets
xxxxxxxx_ent.00x    - entertainment sets


*/

/*********************************************************
   Multifish
**********************************************************/

ROM_START( goldfish ) // Gold Fish 020903 prototype of Multi Fish
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gf_020903.rom", 0x00000, 0x40000, CRC(705304fc) SHA1(f02336066ba2ff394ac153107e308d5356e99eca) )

	ROM_REGION( 0x400000, "gfx", 0 )
	/* did it really use these graphic ROMs? they include the screens used by the games not included in 'Gold Fish' */
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(f497f017) SHA1(0fcf6511bcd2143a472387a20815c3ef037731b9) )
ROM_END



ROM_START( mfish ) // 021120
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf021120.rom", 0x00000, 0x40000, CRC(04a651c9) SHA1(eb7eb5aae00a77edcf328f460970eb180d86d058) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_2 ) // 021121
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf021121.rom", 0x00000, 0x40000, CRC(87090aff) SHA1(87a1fb81330cf4b66e17702c22fda694ebff58eb) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_3 ) // 021124
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf021124.rom", 0x00000, 0x40000, CRC(59fd16f5) SHA1(ea132f68e9c09c40369d4cc02c670ee6e26bdcbe) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1fd7ad5c) SHA1(85268e5396f88328abb42b9479f1127bf2208ac8) )
ROM_END

ROM_START( mfish_4 ) // 021219
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf021219.rom", 0x00000, 0x40000, CRC(1a38c67f) SHA1(887d456b2ba89560329457d9eaea26fb72223a38) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_5 ) // 021227
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf021227.rom", 0x00000, 0x40000, CRC(c3768da4) SHA1(58b74c41a88a781da01dba52744dc74e41deae70) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_6 ) // 030124
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf030124.rom", 0x00000, 0x40000, CRC(554c9cda) SHA1(b119b086bad3f6f8acc64a5809ce449800615406) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(0d985ca4) SHA1(6bd0690c0cc0e1f6f0e68f209bb9efee98e4f1e7) )
ROM_END

ROM_START( mfish_7 ) // 030511
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf030511.rom", 0x00000, 0x40000, CRC(410a34db) SHA1(06b3e3875f036782983e29e305f67a36f78a4f06) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_8 ) // 030522
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf030522.rom", 0x00000, 0x40000, CRC(bff97c25) SHA1(fa80e12275b960374c84518bcaa1e32d0a4ff437) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(47cb76d3) SHA1(108b2566afb064faacec9c41411a6bb2874c99b2) )
ROM_END

ROM_START( mfish_9 ) // 031026
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf031026.rom", 0x00000, 0x40000, CRC(45a23c9c) SHA1(451b390793f89188afe2b6e82fc02b474fb97a7c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_10 ) // 031117
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf031117.rom", 0x00000, 0x40000, CRC(a5283a9c) SHA1(1d244a332af0fb6aa593a246211ff2b6d2c48a59) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( mfish_11 ) // 031124
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf031124.rom", 0x00000, 0x40000, CRC(1d60d37a) SHA1(c0d1b541c4b076bbc810ad637acb4a2663a919ba) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(be1c3c7b) SHA1(9c8be6c1199677c72f52d3cb024253b6d6bb8470) )
ROM_END

ROM_START( mfish_12 ) // 040308
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf040308.rom", 0x00000, 0x40000, CRC(adb9c1d9) SHA1(88c69f48766dc7c98a6f03c1a0a4aa63b76560b6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(73955da7) SHA1(73debc4af32c277e8d5390cb6f11237477eb6d61) )
ROM_END

ROM_START( mfish_13 ) // 040316
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "mf040316.rom", 0x00000, 0x40000, CRC(1acf9f4f) SHA1(c1f4d1c51632a45b533d19c8b6f63d337d84d9cd) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "multi_fish.001", 0x000000, 0x80000, CRC(2f2a7367) SHA1(ce7ee9ca4f374ec61edc3b89d4752f0edb64a910) )
	ROM_LOAD( "multi_fish.002", 0x100000, 0x80000, CRC(606acd73) SHA1(ce5f7b1366dbb16d57fe4b7f395f08725e3cf756) )
	ROM_LOAD( "multi_fish.003", 0x200000, 0x80000, CRC(33759c2a) SHA1(6afcee2e00a27542fc9751702abcc84cd7d3a2a8) )
	ROM_LOAD( "multi_fish.004", 0x300000, 0x80000, CRC(d0053546) SHA1(01c69be0c594947d57648f491904a3b6938a5570) )
	ROM_LOAD( "multi_fish.005", 0x080000, 0x80000, CRC(6f632872) SHA1(949661cb234855a9c86403e9893c5d9f465ddd79) )
	ROM_LOAD( "multi_fish.006", 0x180000, 0x80000, CRC(023c1193) SHA1(98cf2732f9542b0bb3bee324611f6d3143ef1dc4) )
	ROM_LOAD( "multi_fish.007", 0x280000, 0x80000, CRC(9afdc2d3) SHA1(b112fd2005354c9f97d77030bdb6f99d7b5c8050) )
	ROM_LOAD( "multi_fish.008", 0x380000, 0x80000, CRC(29f1a326) SHA1(5e268411cab888c0727aaf8ae7d0b435d2efd189) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(73955da7) SHA1(73debc4af32c277e8d5390cb6f11237477eb6d61) )
ROM_END


/*********************************************************
   Windjammer
**********************************************************/

ROM_START( windjamr ) // 021216 possibly patched for use banking port F9
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "wj_021216_f9.rom", 0x00000, 0x40000, CRC(8ad9357d) SHA1(b6daf44a35075b771350145772d7c701049b0d28) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "windjammer.001", 0x000000, 0x80000, CRC(84067a47) SHA1(de98a2ff35034565c98f6493ccc945abd97cb2a8) )
	ROM_LOAD( "windjammer.002", 0x100000, 0x80000, CRC(c03939ce) SHA1(18e18987813eb996ad0a33089210653fe4c0c8ce) )
	ROM_LOAD( "windjammer.003", 0x200000, 0x80000, CRC(209b8a30) SHA1(f85b3af92f11b0beefe275a74279ea7978f85998) )
	ROM_LOAD( "windjammer.004", 0x300000, 0x80000, CRC(04f3a1f3) SHA1(6f3a267619c010395ff4c00d1dab3efb3202e591) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(f221c070) SHA1(d404496d76eb8127e38093537f31f816a172300b) )
ROM_END


/*********************************************************
   Crazy Monkey

    "Russia" sets use different gfx roms 1-4.
        The official list of hashes shows the both roms.

**********************************************************/

ROM_START( czmon ) // 030217
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_030217.rom", 0x00000, 0x40000, CRC(199ebd48) SHA1(75787f32aa4c8e8ff7bc11c57a37ad5a65f71c52) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_2 ) // 030225
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_030225.rom", 0x00000, 0x40000, CRC(f309aca5) SHA1(3627a3d6a4a50ed8544456d53ab5a489af389a19) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_3 ) // 030227
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_030227.rom", 0x00000, 0x40000, CRC(fa0f090c) SHA1(4f8cd68dd2b6abeaabc9b45da18469cc6e7ac74d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_4 ) // 030404
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_030404.rom", 0x00000, 0x40000, CRC(d92b96f2) SHA1(fd99caa2b6ef7218563db4f3b755e34dd551e05f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_5 ) // 030421
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_030421.rom", 0x00000, 0x40000, CRC(6826564e) SHA1(6559e45e3ec39c1d201ed54a10fdb5c6aeff6582) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(3a0e19fd) SHA1(27a56bd43264ccdd1d7db118dc218321338990ef) )
ROM_END

ROM_START( czmon_6 ) // 031016
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_031016.rom", 0x00000, 0x40000, CRC(cec8b85e) SHA1(2f2a5ecbb311ade75f8fdc322c6e63836d4119c3) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_7 ) // 031110
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_031110.rom", 0x00000, 0x40000, CRC(d3e67980) SHA1(f0daa91abdde211a2ff61414d84386b763c30949) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(3a0e19fd) SHA1(27a56bd43264ccdd1d7db118dc218321338990ef) )
ROM_END

ROM_START( czmon_8 ) // 050120
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_050120.rom", 0x00000, 0x40000, CRC(9af1e03f) SHA1(caadf48a36da48f4e126b286f6f5498005d8182a) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001", 0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002", 0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003", 0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004", 0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(3a0e19fd) SHA1(27a56bd43264ccdd1d7db118dc218321338990ef) )
ROM_END

ROM_START( czmon_9 ) // 070315
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_070315.rom", 0x00000, 0x40000, CRC(5b2310b0) SHA1(b9bcb45bd97cbf1546c938512709bae44501447d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey.001",   0x000000, 0x80000, CRC(665ae6a1) SHA1(2ef2d657918d66d303d45d2d82430d13108f3fad) )
	ROM_LOAD( "crazymonkey.002",   0x100000, 0x80000, CRC(1a8e235a) SHA1(6d562cc5250283fc0c8ca7e103231a2e5bab4c69) )
	ROM_LOAD( "crazymonkey.003",   0x200000, 0x80000, CRC(415133eb) SHA1(227f7c8858fd5b928fdde691017104d3bd69910a) )
	ROM_LOAD( "crazymonkey.004",   0x300000, 0x80000, CRC(ec45fe14) SHA1(4a0fc87e2f19ea05c9a5746bb4ca7cafe5592d33) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(8d494ff5) SHA1(9c5067bb86fca1f7fb023e7a7f9e1dc63f56c3f8) )
ROM_END

ROM_START( czmon_12 ) // 090711 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_e_090711.rom", 0x00000, 0x40000, CRC(f2330a20) SHA1(4e15b53bcd0df6ef8859fb198311f41fd3c34310) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_e.001", 0x000000, 0x80000, CRC(6cb4112a) SHA1(987da4347dc97ca618ade6275357924a8badb5a2) )
	ROM_LOAD( "crazymonkey_e.002", 0x100000, 0x80000, CRC(6f62a62e) SHA1(b71917ebb0f39bdf949a1f7746031663edb72186) )
	ROM_LOAD( "crazymonkey_e.003", 0x200000, 0x80000, CRC(2c9cbad7) SHA1(e42065218670a0b82d5dc91e92e81c7a89d6c6c1) )
	ROM_LOAD( "crazymonkey_e.004", 0x300000, 0x80000, CRC(ad06e7aa) SHA1(e1af54e8ad0bf960fdde5360dc2230326a19ceb9) )
	ROM_LOAD( "crazymonkey_e.005", 0x080000, 0x80000, CRC(6d148be7) SHA1(b9aa78ede6ace2e6fd24028851f0f750de7685de) )
	ROM_LOAD( "crazymonkey_e.006", 0x180000, 0x80000, CRC(dc12670d) SHA1(e39cb83b800c40884b2934206f498429d990553d) )
	ROM_LOAD( "crazymonkey_e.007", 0x280000, 0x80000, CRC(73d1a75b) SHA1(7cc230ee431288e0c8a05a1a7d77973ba500d503) )
	ROM_LOAD( "crazymonkey_e.008", 0x380000, 0x80000, CRC(0d3718ef) SHA1(3466f41b494439b6c24687fa75cb11bfe124a59f) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_13 ) // 100311
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_m_100311.rom", 0x00000, 0x40000, CRC(4eb76b34) SHA1(b2283fd8f6bc52007ae1fc3e63f37066adfd8351) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_m.001",  0x000000, 0x80000, CRC(683f2be3) SHA1(6fdba4ec07752bf049787a11638895352e9d5f10) )
	ROM_LOAD( "crazymonkey_m.002",  0x100000, 0x80000, CRC(e21ce6a4) SHA1(942ffe323ddbcaaad887cb5bc9f356550926083b) )
	ROM_LOAD( "crazymonkey_m.003",  0x200000, 0x80000, CRC(c3d0e3d5) SHA1(5b0cb436c6b0bac1213c1df56702fa7f16856106) )
	ROM_LOAD( "crazymonkey_m.004",  0x300000, 0x80000, CRC(f79df52c) SHA1(b99fa9f61849b62668bf9edff1c80212a9108b15) )
	ROM_LOAD( "crazymonkey_m.005",  0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006",  0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007",  0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008",  0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(c9e704d4) SHA1(df49cfcd9782d5a83e136e5de68b22cc1b2a6353) )
ROM_END


ROM_START( czmon_15 ) // 100311 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_e_100311.rom", 0x00000, 0x40000, CRC(8aa29af3) SHA1(0f9bcabf889d75f7dc8f16de4eb3166735536ec4) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_e.001", 0x000000, 0x80000, CRC(6cb4112a) SHA1(987da4347dc97ca618ade6275357924a8badb5a2) )
	ROM_LOAD( "crazymonkey_e.002", 0x100000, 0x80000, CRC(6f62a62e) SHA1(b71917ebb0f39bdf949a1f7746031663edb72186) )
	ROM_LOAD( "crazymonkey_e.003", 0x200000, 0x80000, CRC(2c9cbad7) SHA1(e42065218670a0b82d5dc91e92e81c7a89d6c6c1) )
	ROM_LOAD( "crazymonkey_e.004", 0x300000, 0x80000, CRC(ad06e7aa) SHA1(e1af54e8ad0bf960fdde5360dc2230326a19ceb9) )
	ROM_LOAD( "crazymonkey_e.005", 0x080000, 0x80000, CRC(6d148be7) SHA1(b9aa78ede6ace2e6fd24028851f0f750de7685de) )
	ROM_LOAD( "crazymonkey_e.006", 0x180000, 0x80000, CRC(dc12670d) SHA1(e39cb83b800c40884b2934206f498429d990553d) )
	ROM_LOAD( "crazymonkey_e.007", 0x280000, 0x80000, CRC(73d1a75b) SHA1(7cc230ee431288e0c8a05a1a7d77973ba500d503) )
	ROM_LOAD( "crazymonkey_e.008", 0x380000, 0x80000, CRC(0d3718ef) SHA1(3466f41b494439b6c24687fa75cb11bfe124a59f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(9ec64eca) SHA1(d3d389c7838e00a1d7b615ff6cb67709b8321991) )
ROM_END

ROM_START( czmon_16 ) // 100312
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_100312.rom", 0x00000, 0x40000, CRC(9465e680) SHA1(3fec33047944e9236ef4c8848256576b4bfd70d8) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey.001",   0x000000, 0x80000, CRC(665ae6a1) SHA1(2ef2d657918d66d303d45d2d82430d13108f3fad) )
	ROM_LOAD( "crazymonkey.002",   0x100000, 0x80000, CRC(1a8e235a) SHA1(6d562cc5250283fc0c8ca7e103231a2e5bab4c69) )
	ROM_LOAD( "crazymonkey.003",   0x200000, 0x80000, CRC(415133eb) SHA1(227f7c8858fd5b928fdde691017104d3bd69910a) )
	ROM_LOAD( "crazymonkey.004",   0x300000, 0x80000, CRC(ec45fe14) SHA1(4a0fc87e2f19ea05c9a5746bb4ca7cafe5592d33) )
	ROM_LOAD( "crazymonkey_m.005", 0x080000, 0x80000, CRC(9d4d2a94) SHA1(c714e110de628b343dfc7fff23befaa1276056a9) )
	ROM_LOAD( "crazymonkey_m.006", 0x180000, 0x80000, CRC(a15f0fee) SHA1(3f06d5a1a41e1335bcc7586a5ea95b9b734155c0) )
	ROM_LOAD( "crazymonkey_m.007", 0x280000, 0x80000, CRC(715a2528) SHA1(6c4c72592568ecbaa9518fb7271d2714dd22dbbb) )
	ROM_LOAD( "crazymonkey_m.008", 0x380000, 0x80000, CRC(6fdb6fd5) SHA1(f40916112365de258956ec033aff79aae1f58690) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(038c07ca) SHA1(4f47729ccf94848763722abd8d67cf4d3c2b777d) )
ROM_END

ROM_START( czmon_17 ) // 100324 entertainment x
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_x_100324.rom", 0x00000, 0x40000, CRC(6ebf9dc1) SHA1(2826eec19500f14393f7ac55bbe1da586b81400c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_x.001", 0x000000, 0x80000, CRC(57fd2287) SHA1(5dee6718ec4aecad094e98ccdb32d4e8c722382f) )
	ROM_LOAD( "crazymonkey_ent_x.002", 0x100000, 0x80000, CRC(e9133ea3) SHA1(e79ccbcb0a952a95ef95c246f14262b563afb9dc) )
	ROM_LOAD( "crazymonkey_ent_x.003", 0x200000, 0x80000, CRC(42eedf0d) SHA1(712fca16fb62cdb1b05f0c88f1749dcb064da61e) )
	ROM_LOAD( "crazymonkey_ent_x.004", 0x300000, 0x80000, CRC(b8e183d7) SHA1(6ef430d22522fe19994bab3fca0ce46ee0050d20) )
	ROM_LOAD( "crazymonkey_ent_x.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_x.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_x.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_x.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_18 ) // 100331 entertainment x
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_x_100331.rom", 0x00000, 0x40000, CRC(7cf8234a) SHA1(90c4b6da1ac7a3a53b6200c6cc31ddad5a90dd8c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_x.001", 0x000000, 0x80000, CRC(57fd2287) SHA1(5dee6718ec4aecad094e98ccdb32d4e8c722382f) )
	ROM_LOAD( "crazymonkey_ent_x.002", 0x100000, 0x80000, CRC(e9133ea3) SHA1(e79ccbcb0a952a95ef95c246f14262b563afb9dc) )
	ROM_LOAD( "crazymonkey_ent_x.003", 0x200000, 0x80000, CRC(42eedf0d) SHA1(712fca16fb62cdb1b05f0c88f1749dcb064da61e) )
	ROM_LOAD( "crazymonkey_ent_x.004", 0x300000, 0x80000, CRC(b8e183d7) SHA1(6ef430d22522fe19994bab3fca0ce46ee0050d20) )
	ROM_LOAD( "crazymonkey_ent_x.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_x.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_x.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_x.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_19 ) // 101208 entertainment n
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_n_101208.rom", 0x00000, 0x40000, CRC(c23beac7) SHA1(2fcdc0af4373f2b25830a2ec40449187951037b6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_n.001", 0x000000, 0x80000, CRC(566fbc6f) SHA1(509bbb93f5eacbe47773fc76a897d79bae923fb0) )
	ROM_LOAD( "crazymonkey_ent_n.002", 0x100000, 0x80000, CRC(d9418878) SHA1(270593b031058c1e404ac98592e6cbf07f5de908) )
	ROM_LOAD( "crazymonkey_ent_n.003", 0x200000, 0x80000, CRC(33c16c9e) SHA1(84bafc63afe91c313bee02479e54def6a54dfa47) )
	ROM_LOAD( "crazymonkey_ent_n.004", 0x300000, 0x80000, CRC(e4311273) SHA1(da4d5463126bde1065e486b852b02e694dbee74d) )
	ROM_LOAD( "crazymonkey_ent_n.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_n.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_n.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_n.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_20 ) // 110111 entertainment a
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_a_110111.rom", 0x00000, 0x40000, CRC(2863cc8f) SHA1(00133c029b7b6a3cd0a78947c9da033557d778c8) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_a.001", 0x000000, 0x80000, CRC(9bc37676) SHA1(6ba875c594a3d06de81bd5dbbb871767105ca5ea) )
	ROM_LOAD( "crazymonkey_ent_a.002", 0x100000, 0x80000, CRC(2a342ab8) SHA1(1e88c523089a6ba91f49c07a95e26337711fd0dc) )
	ROM_LOAD( "crazymonkey_ent_a.003", 0x200000, 0x80000, CRC(dc67d4d9) SHA1(e388528eebe1a2b6c1e93283bddbe747dae1d8c6) )
	ROM_LOAD( "crazymonkey_ent_a.004", 0x300000, 0x80000, CRC(cedd485e) SHA1(fecd8701efd0450e66be6fa62b1ba675c7533ae0) )
	ROM_LOAD( "crazymonkey_ent_a.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_a.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_a.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_a.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_21 ) // 110124 entertainment a
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_a_110124.rom", 0x00000, 0x40000, CRC(d5675b49) SHA1(d6831529e477e8853f1fa1f41287e5c9ab298ad6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_a.001", 0x000000, 0x80000, CRC(9bc37676) SHA1(6ba875c594a3d06de81bd5dbbb871767105ca5ea) )
	ROM_LOAD( "crazymonkey_ent_a.002", 0x100000, 0x80000, CRC(2a342ab8) SHA1(1e88c523089a6ba91f49c07a95e26337711fd0dc) )
	ROM_LOAD( "crazymonkey_ent_a.003", 0x200000, 0x80000, CRC(dc67d4d9) SHA1(e388528eebe1a2b6c1e93283bddbe747dae1d8c6) )
	ROM_LOAD( "crazymonkey_ent_a.004", 0x300000, 0x80000, CRC(cedd485e) SHA1(fecd8701efd0450e66be6fa62b1ba675c7533ae0) )
	ROM_LOAD( "crazymonkey_ent_a.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_a.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_a.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_a.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_22 ) // 110204 entertainment a
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_a_110204.rom", 0x00000, 0x40000, CRC(1958e2ba) SHA1(6a1ff8a9976cc8d211146d44bddbf1f295a0aa7c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_a.001", 0x000000, 0x80000, CRC(9bc37676) SHA1(6ba875c594a3d06de81bd5dbbb871767105ca5ea) )
	ROM_LOAD( "crazymonkey_ent_a.002", 0x100000, 0x80000, CRC(2a342ab8) SHA1(1e88c523089a6ba91f49c07a95e26337711fd0dc) )
	ROM_LOAD( "crazymonkey_ent_a.003", 0x200000, 0x80000, CRC(dc67d4d9) SHA1(e388528eebe1a2b6c1e93283bddbe747dae1d8c6) )
	ROM_LOAD( "crazymonkey_ent_a.004", 0x300000, 0x80000, CRC(cedd485e) SHA1(fecd8701efd0450e66be6fa62b1ba675c7533ae0) )
	ROM_LOAD( "crazymonkey_ent_a.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_a.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_a.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_a.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_23 ) // 110311 entertainment b
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_b_110311.rom", 0x00000, 0x40000, CRC(49991cec) SHA1(9be81d85ffb4aeb0c872e00f548f72150a9f0a23) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_b.001", 0x000000, 0x80000, CRC(39ce4dbf) SHA1(37c7a1c715142e9d62833c08e674a71ba79d3929) )
	ROM_LOAD( "crazymonkey_ent_b.002", 0x100000, 0x80000, CRC(62bda191) SHA1(14a9c1e0d8b83f2fc2a2fb6011fa90ec6f722473) )
	ROM_LOAD( "crazymonkey_ent_b.003", 0x200000, 0x80000, CRC(c270cc33) SHA1(65652450372cb7a5dff96029da0bbb27bbbb50f8) )
	ROM_LOAD( "crazymonkey_ent_b.004", 0x300000, 0x80000, CRC(4e4dea89) SHA1(a59efc77b125400c9af129017652e631a014247b) )
	ROM_LOAD( "crazymonkey_ent_b.005", 0x080000, 0x80000, CRC(e32e8701) SHA1(7beb285c4f649090108035a20219d09fa3cb9fdc) )
	ROM_LOAD( "crazymonkey_ent_b.006", 0x180000, 0x80000, CRC(f127ebea) SHA1(9385a011836f0634b12ff8fc16361ac3d854ffae) )
	ROM_LOAD( "crazymonkey_ent_b.007", 0x280000, 0x80000, CRC(c4c08bc7) SHA1(3e90e5a7d622e7ba38080872eb49252781fb16e2) )
	ROM_LOAD( "crazymonkey_ent_b.008", 0x380000, 0x80000, CRC(2b6d8096) SHA1(095aa6edeee97f5d7f901f60a3f493c35983798c) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_24 ) // 140526 entertainment b
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_b_140526.rom", 0x00000, 0x40000, CRC(60497175) SHA1(18756d9291d657d579eb78c3fba8517682080256) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey_ent_ba.001", 0x000000, 0x80000, CRC(ed608410) SHA1(0571d48e380db0c897e408383f906484c3df9e94) )
	ROM_LOAD( "crazymonkey_ent_ba.002", 0x100000, 0x80000, CRC(252563ef) SHA1(fd8a629b2be3ddd9a5bd5323f1bc01801d264f06) )
	ROM_LOAD( "crazymonkey_ent_ba.003", 0x200000, 0x80000, CRC(b96a207b) SHA1(1954e381ea0459e9223eba44908482a9c0d3aee7) )
	ROM_LOAD( "crazymonkey_ent_ba.004", 0x300000, 0x80000, CRC(ae57c9e3) SHA1(779a505ed728f115598fd0cb1127f582ad7f34ff) )
	ROM_LOAD( "crazymonkey_ent_b.005",  0x080000, 0x80000, CRC(e32e8701) SHA1(7beb285c4f649090108035a20219d09fa3cb9fdc) )
	ROM_LOAD( "crazymonkey_ent_b.006",  0x180000, 0x80000, CRC(f127ebea) SHA1(9385a011836f0634b12ff8fc16361ac3d854ffae) )
	ROM_LOAD( "crazymonkey_ent_b.007",  0x280000, 0x80000, CRC(c4c08bc7) SHA1(3e90e5a7d622e7ba38080872eb49252781fb16e2) )
	ROM_LOAD( "crazymonkey_ent_b.008",  0x380000, 0x80000, CRC(2b6d8096) SHA1(095aa6edeee97f5d7f901f60a3f493c35983798c) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( czmon_25 ) // 110411 entertainment c
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm_c_110411.rom", 0x00000, 0x40000, CRC(602f24e0) SHA1(a44a80f90ac30cc7c987372bb55f58bb5aa8f3a4) )

	ROM_REGION( 0x400000, "gfx", 0 ) // same as ent_a
	ROM_LOAD( "crazymonkey_ent_c.001", 0x000000, 0x80000, CRC(9bc37676) SHA1(6ba875c594a3d06de81bd5dbbb871767105ca5ea) )
	ROM_LOAD( "crazymonkey_ent_c.002", 0x100000, 0x80000, CRC(2a342ab8) SHA1(1e88c523089a6ba91f49c07a95e26337711fd0dc) )
	ROM_LOAD( "crazymonkey_ent_c.003", 0x200000, 0x80000, CRC(dc67d4d9) SHA1(e388528eebe1a2b6c1e93283bddbe747dae1d8c6) )
	ROM_LOAD( "crazymonkey_ent_c.004", 0x300000, 0x80000, CRC(cedd485e) SHA1(fecd8701efd0450e66be6fa62b1ba675c7533ae0) )
	ROM_LOAD( "crazymonkey_ent_c.005", 0x080000, 0x80000, CRC(e03cc814) SHA1(b1f4e4691c0f645e3988765c7b1fdfd476b293d8) )
	ROM_LOAD( "crazymonkey_ent_c.006", 0x180000, 0x80000, CRC(10e60a18) SHA1(3876ff0e6ea07b5f8147e3872438d1dfe3b3a373) )
	ROM_LOAD( "crazymonkey_ent_c.007", 0x280000, 0x80000, CRC(42be3677) SHA1(e12d6ea7216cda9f7cfb2838e5d7ce65283001d8) )
	ROM_LOAD( "crazymonkey_ent_c.008", 0x380000, 0x80000, CRC(59df80f2) SHA1(464728c1565482be11fd3e25ae3a537aa73d0e6d) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END


/*********************************************************
   Fruit Cocktail

    "Russia" sets use different gfx roms.
        The official list of hashes shows only updated roms.

**********************************************************/

ROM_START( fcockt ) // 030505
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_030505.rom", 0x00000, 0x40000, CRC(70f749a8) SHA1(cc65334e8dfae5ffef1d73bd5085e3555905e259) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( fcockt_2 ) // 030512
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_030512.rom", 0x00000, 0x40000, CRC(9563f1a1) SHA1(c23ebcf64609a56a029f05101185f3adf73cdadd) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( fcockt_3 ) // 030623
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_030623.rom", 0x00000, 0x40000, CRC(00a267b2) SHA1(b95c5e06cf41762802199e1b55a5eda2243c9af7) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(65ad8b49) SHA1(1f15a17c3a7f1dffc36b92b6d5e8e600c3a59eaf) )
ROM_END

ROM_START( fcockt_4 ) // 031028
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_031028.rom", 0x00000, 0x40000, CRC(8e2b7f02) SHA1(18a0ac6e3c6f1d6ae7aeae5322e6b6617923cfdf) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( fcockt_5 ) // 031111
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_031111.rom", 0x00000, 0x40000, CRC(feef74bf) SHA1(7cc9aeb88a2923f6c5c176abcd6c6b241b353eab) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(65ad8b49) SHA1(1f15a17c3a7f1dffc36b92b6d5e8e600c3a59eaf) )
ROM_END

ROM_START( fcockt_6 ) // 040216
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_040216.rom", 0x00000, 0x40000, CRC(d12b0201) SHA1(09f4b0b5239609ebf13e643782d1881920a1203d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(65ad8b49) SHA1(1f15a17c3a7f1dffc36b92b6d5e8e600c3a59eaf) )
ROM_END

ROM_START( fcockt_7 ) // 050118
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_050118.rom", 0x00000, 0x40000, CRC(356b140a) SHA1(d6e671b5c7fa6592f80b90b289cce0afe1a9cea3) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(65ad8b49) SHA1(1f15a17c3a7f1dffc36b92b6d5e8e600c3a59eaf) )
ROM_END

ROM_START( fcockt_8 ) // 060111
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_m_060111.rom", 0x00000, 0x40000, CRC(a4af79e3) SHA1(28f40573d6c61e1937b8d05da94e197da5236f57) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_m.001", 0x000000, 0x80000, CRC(da72c0bb) SHA1(41c9eebccce82520dfe41d6a3a574b2890945ffa) )
	ROM_LOAD( "fruitcocktail_m.002", 0x100000, 0x80000, CRC(6239ba9d) SHA1(22486ad30c28341784e7e490255247b82782b72e) )
	ROM_LOAD( "fruitcocktail_m.003", 0x200000, 0x80000, CRC(2c14a464) SHA1(5fce2f4ef95c5054b055db94399946257bc7321f) )
	ROM_LOAD( "fruitcocktail_m.004", 0x300000, 0x80000, CRC(115898f4) SHA1(55b93bddaeede1c2f6b18083a6a2e6329af087cc) )
	ROM_LOAD( "fruitcocktail_m.005", 0x080000, 0x80000, CRC(f0176b60) SHA1(f764aea00ed306a28cacc62f8d2db9cc42895db8) )
	ROM_LOAD( "fruitcocktail_m.006", 0x180000, 0x80000, CRC(ef24f255) SHA1(6ff924627c179868a25f180f79cd57182b72d9d4) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) )
	ROM_LOAD( "fruitcocktail_m.008", 0x380000, 0x80000, CRC(d282e42e) SHA1(eac9c3eaef39b1805f863ade5da47d6274d20a55) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1d1d98f5) SHA1(67dd05b5de28c30f7b35bb79794d303123e52154) )
ROM_END

ROM_START( fcockt_9 ) // 070305
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_070305.rom", 0x00000, 0x40000, CRC(4eb835d9) SHA1(406b2fcad0ca587eacee123ac4b040cb6f6db18c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail.001",   0x000000, 0x80000, CRC(735fbe79) SHA1(6ab590e00043dcb6648fd942e11747486d497df1) ) /* Only this set is listed as official hashes */
	ROM_LOAD( "fruitcocktail.002",   0x100000, 0x80000, CRC(28fc888e) SHA1(2b8c55675bf61203682d560c9b3f29568719113f) )
	ROM_LOAD( "fruitcocktail.003",   0x200000, 0x80000, CRC(01fc1a18) SHA1(4f73c6cde6ed741cc8c1bc32442f572ee7ba208a) )
	ROM_LOAD( "fruitcocktail.004",   0x300000, 0x80000, CRC(68daa864) SHA1(b05c455e23ace80e102699616b75f3a0946c04bc) )
	ROM_LOAD( "fruitcocktail.005",   0x080000, 0x80000, CRC(64b547e3) SHA1(285421fa3aa67a16cf6a9dadb20d74e6a8471dc0) )
	ROM_LOAD( "fruitcocktail.006",   0x180000, 0x80000, CRC(965d6363) SHA1(5c229238a09ec54147d492e9843595962ce79952) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) ) // this remains the same
	ROM_LOAD( "fruitcocktail.008",   0x380000, 0x80000, CRC(8384e4d4) SHA1(83d0bbbd7cca7328a66a69cf802632fd8d22d5b8) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(fba3b2e7) SHA1(ed7b6d3d9b4cb4903040456a56458585340ffebf) )
ROM_END

ROM_START( fcockt_10 ) // 070517
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_070517.rom", 0x00000, 0x40000, CRC(8b43f765) SHA1(86412c37252cf1f12a3acd9359bbf1cdcf52da9f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail.001",   0x000000, 0x80000, CRC(735fbe79) SHA1(6ab590e00043dcb6648fd942e11747486d497df1) ) /* Only this set is listed as official hashes */
	ROM_LOAD( "fruitcocktail.002",   0x100000, 0x80000, CRC(28fc888e) SHA1(2b8c55675bf61203682d560c9b3f29568719113f) )
	ROM_LOAD( "fruitcocktail.003",   0x200000, 0x80000, CRC(01fc1a18) SHA1(4f73c6cde6ed741cc8c1bc32442f572ee7ba208a) )
	ROM_LOAD( "fruitcocktail.004",   0x300000, 0x80000, CRC(68daa864) SHA1(b05c455e23ace80e102699616b75f3a0946c04bc) )
	ROM_LOAD( "fruitcocktail.005",   0x080000, 0x80000, CRC(64b547e3) SHA1(285421fa3aa67a16cf6a9dadb20d74e6a8471dc0) )
	ROM_LOAD( "fruitcocktail.006",   0x180000, 0x80000, CRC(965d6363) SHA1(5c229238a09ec54147d492e9843595962ce79952) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) ) // this remains the same
	ROM_LOAD( "fruitcocktail.008",   0x380000, 0x80000, CRC(8384e4d4) SHA1(83d0bbbd7cca7328a66a69cf802632fd8d22d5b8) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(4bde165d) SHA1(c004987022e78b32fda60de1526da4dd03c01d54) )
ROM_END

ROM_START( fcockt_11 ) // 070822
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_070822.rom", 0x00000, 0x40000, CRC(f156657d) SHA1(bd538e714a87461bdf84df18ae3f8caeee876747) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail.001",   0x000000, 0x80000, CRC(735fbe79) SHA1(6ab590e00043dcb6648fd942e11747486d497df1) ) /* Only this set is listed as official hashes */
	ROM_LOAD( "fruitcocktail.002",   0x100000, 0x80000, CRC(28fc888e) SHA1(2b8c55675bf61203682d560c9b3f29568719113f) )
	ROM_LOAD( "fruitcocktail.003",   0x200000, 0x80000, CRC(01fc1a18) SHA1(4f73c6cde6ed741cc8c1bc32442f572ee7ba208a) )
	ROM_LOAD( "fruitcocktail.004",   0x300000, 0x80000, CRC(68daa864) SHA1(b05c455e23ace80e102699616b75f3a0946c04bc) )
	ROM_LOAD( "fruitcocktail.005",   0x080000, 0x80000, CRC(64b547e3) SHA1(285421fa3aa67a16cf6a9dadb20d74e6a8471dc0) )
	ROM_LOAD( "fruitcocktail.006",   0x180000, 0x80000, CRC(965d6363) SHA1(5c229238a09ec54147d492e9843595962ce79952) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) ) // this remains the same
	ROM_LOAD( "fruitcocktail.008",   0x380000, 0x80000, CRC(8384e4d4) SHA1(83d0bbbd7cca7328a66a69cf802632fd8d22d5b8) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(249e1c14) SHA1(03ca203a1faf5133a72363f3fb1c845afc681ba3) )
ROM_END

ROM_START( fcockt_12 ) // 070911
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_070911.rom", 0x00000, 0x40000, CRC(17c015bb) SHA1(5369549853f1c463b999bb4ff9d06c5d8e467c5b) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail.001",   0x000000, 0x80000, CRC(735fbe79) SHA1(6ab590e00043dcb6648fd942e11747486d497df1) ) /* Only this set is listed as official hashes */
	ROM_LOAD( "fruitcocktail.002",   0x100000, 0x80000, CRC(28fc888e) SHA1(2b8c55675bf61203682d560c9b3f29568719113f) )
	ROM_LOAD( "fruitcocktail.003",   0x200000, 0x80000, CRC(01fc1a18) SHA1(4f73c6cde6ed741cc8c1bc32442f572ee7ba208a) )
	ROM_LOAD( "fruitcocktail.004",   0x300000, 0x80000, CRC(68daa864) SHA1(b05c455e23ace80e102699616b75f3a0946c04bc) )
	ROM_LOAD( "fruitcocktail.005",   0x080000, 0x80000, CRC(64b547e3) SHA1(285421fa3aa67a16cf6a9dadb20d74e6a8471dc0) )
	ROM_LOAD( "fruitcocktail.006",   0x180000, 0x80000, CRC(965d6363) SHA1(5c229238a09ec54147d492e9843595962ce79952) )
	ROM_LOAD( "fruitcocktail_m.007", 0x280000, 0x80000, CRC(20f87a15) SHA1(cb60866a3543668f3592c270b445dee881d78128) ) // this remains the same
	ROM_LOAD( "fruitcocktail.008",   0x380000, 0x80000, CRC(8384e4d4) SHA1(83d0bbbd7cca7328a66a69cf802632fd8d22d5b8) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(ec461dd8) SHA1(0d3d9909891fd85545b539d3efeec4bcf5f99ea8) )
ROM_END


ROM_START( fcockt_14 ) // 090708 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc_e_090708.rom", 0x00000, 0x40000, CRC(92a64b62) SHA1(6fb5a82fa41e131f01f097b739f808b5f1f8c11b) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail_ent.001", 0x000000, 0x80000, CRC(27c3f229) SHA1(06b19ff2134a4f419de71848f4764d7b3f9dfb1b) )
	ROM_LOAD( "fruitcocktail_ent.002", 0x100000, 0x80000, CRC(04247991) SHA1(0684c8b1cba824fb083be119b4c190205b50c9ee) )
	ROM_LOAD( "fruitcocktail_ent.003", 0x200000, 0x80000, CRC(be51802d) SHA1(95f3066c48c3018d3afdffcb2b109d25a00b2a64) )
	ROM_LOAD( "fruitcocktail_ent.004", 0x300000, 0x80000, CRC(bcc5c524) SHA1(ec860c359ffa3907296ed7524a56131debb5575e) )
	ROM_LOAD( "fruitcocktail_ent.005", 0x080000, 0x80000, CRC(b5c40862) SHA1(1a0890f9a3169ddf54fd702061c73bed84bc97b3) )
	ROM_LOAD( "fruitcocktail_ent.006", 0x180000, 0x80000, CRC(0e46b961) SHA1(a4b30aed5f0ed0dc6fb0d56c028cf501b4b4fd38) )
	ROM_LOAD( "fruitcocktail_ent.007", 0x280000, 0x80000, CRC(b189854b) SHA1(b1f53c6d48b7944bdbbbe06dc2ffd739635d2acb) )
	ROM_LOAD( "fruitcocktail_ent.008", 0x380000, 0x80000, CRC(f4395057) SHA1(c60b270b485fe9710155a875ed58dd0b3a6df056) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1786cfef) SHA1(4dd988d07951cd3152e122ca2ca151b5c8f771a0) )
ROM_END


/*********************************************************
   Lucky Haunter

    "Russia" sets use different gfx roms 1-4.
        The official list of hashes shows only updated roms.

**********************************************************/

ROM_START( lhaunt ) // 030707
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_m_030707.rom", 0x00000, 0x40000, CRC(079c7c1c) SHA1(c7b8e1b98cd0aa665d62c1652716993539c9f3ef) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_m.001", 0x000000, 0x80000, CRC(5f1000c6) SHA1(05154d786caf7f5fb9ed90c7d3391cec42e871f8) )
	ROM_LOAD( "luckyhaunter_m.002", 0x100000, 0x80000, CRC(b18abebc) SHA1(66c28fd3f338360b9236dcf414548bfb53655951) )
	ROM_LOAD( "luckyhaunter_m.003", 0x200000, 0x80000, CRC(2e67a1d9) SHA1(a3eff78f25e4e4878706d7c9e4ca71f6914006c6) )
	ROM_LOAD( "luckyhaunter_m.004", 0x300000, 0x80000, CRC(90c963f6) SHA1(2a9c689315cdfb67425f0710511dc0e0241741a7) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( lhaunt_2 ) // 030804
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_m_030804.rom", 0x00000, 0x40000, CRC(2a9a7267) SHA1(b75702a678d716cd0ccb1f2d1e58c1d3e9f7ca98) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_m.001", 0x000000, 0x80000, CRC(5f1000c6) SHA1(05154d786caf7f5fb9ed90c7d3391cec42e871f8) )
	ROM_LOAD( "luckyhaunter_m.002", 0x100000, 0x80000, CRC(b18abebc) SHA1(66c28fd3f338360b9236dcf414548bfb53655951) )
	ROM_LOAD( "luckyhaunter_m.003", 0x200000, 0x80000, CRC(2e67a1d9) SHA1(a3eff78f25e4e4878706d7c9e4ca71f6914006c6) )
	ROM_LOAD( "luckyhaunter_m.004", 0x300000, 0x80000, CRC(90c963f6) SHA1(2a9c689315cdfb67425f0710511dc0e0241741a7) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(70d4f018) SHA1(089c8cee33157d43c1946a23ff7e75af65b94ebd) )
ROM_END

ROM_START( lhaunt_3 ) // 031027
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_m_031027.rom", 0x00000, 0x40000, CRC(8784838d) SHA1(caec736dde2878588ab197ba37801cf7a9ed975b) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_m.001", 0x000000, 0x80000, CRC(5f1000c6) SHA1(05154d786caf7f5fb9ed90c7d3391cec42e871f8) )
	ROM_LOAD( "luckyhaunter_m.002", 0x100000, 0x80000, CRC(b18abebc) SHA1(66c28fd3f338360b9236dcf414548bfb53655951) )
	ROM_LOAD( "luckyhaunter_m.003", 0x200000, 0x80000, CRC(2e67a1d9) SHA1(a3eff78f25e4e4878706d7c9e4ca71f6914006c6) )
	ROM_LOAD( "luckyhaunter_m.004", 0x300000, 0x80000, CRC(90c963f6) SHA1(2a9c689315cdfb67425f0710511dc0e0241741a7) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( lhaunt_4 ) // 031111
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_m_031111.rom", 0x00000, 0x40000, CRC(fc357b75) SHA1(512e4f57612851284bb93ba97c276cbc7cb758d9) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_m.001", 0x000000, 0x80000, CRC(5f1000c6) SHA1(05154d786caf7f5fb9ed90c7d3391cec42e871f8) )
	ROM_LOAD( "luckyhaunter_m.002", 0x100000, 0x80000, CRC(b18abebc) SHA1(66c28fd3f338360b9236dcf414548bfb53655951) )
	ROM_LOAD( "luckyhaunter_m.003", 0x200000, 0x80000, CRC(2e67a1d9) SHA1(a3eff78f25e4e4878706d7c9e4ca71f6914006c6) )
	ROM_LOAD( "luckyhaunter_m.004", 0x300000, 0x80000, CRC(90c963f6) SHA1(2a9c689315cdfb67425f0710511dc0e0241741a7) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(70d4f018) SHA1(089c8cee33157d43c1946a23ff7e75af65b94ebd) )
ROM_END

ROM_START( lhaunt_5 ) // 040216
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_m_040216.rom", 0x00000, 0x40000, CRC(558d8345) SHA1(30a87902b291413b1e6eaad6bf4964c54e391e23) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_m.001", 0x000000, 0x80000, CRC(5f1000c6) SHA1(05154d786caf7f5fb9ed90c7d3391cec42e871f8) )
	ROM_LOAD( "luckyhaunter_m.002", 0x100000, 0x80000, CRC(b18abebc) SHA1(66c28fd3f338360b9236dcf414548bfb53655951) )
	ROM_LOAD( "luckyhaunter_m.003", 0x200000, 0x80000, CRC(2e67a1d9) SHA1(a3eff78f25e4e4878706d7c9e4ca71f6914006c6) )
	ROM_LOAD( "luckyhaunter_m.004", 0x300000, 0x80000, CRC(90c963f6) SHA1(2a9c689315cdfb67425f0710511dc0e0241741a7) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(70d4f018) SHA1(089c8cee33157d43c1946a23ff7e75af65b94ebd) )
ROM_END

ROM_START( lhaunt_6 ) // 040825
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_m_040825.rom", 0x00000, 0x40000, CRC(f9924fa1) SHA1(57a1730fef4963d30f3991f27021647a8c681952) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_m.001", 0x000000, 0x80000, CRC(5f1000c6) SHA1(05154d786caf7f5fb9ed90c7d3391cec42e871f8) )
	ROM_LOAD( "luckyhaunter_m.002", 0x100000, 0x80000, CRC(b18abebc) SHA1(66c28fd3f338360b9236dcf414548bfb53655951) )
	ROM_LOAD( "luckyhaunter_m.003", 0x200000, 0x80000, CRC(2e67a1d9) SHA1(a3eff78f25e4e4878706d7c9e4ca71f6914006c6) )
	ROM_LOAD( "luckyhaunter_m.004", 0x300000, 0x80000, CRC(90c963f6) SHA1(2a9c689315cdfb67425f0710511dc0e0241741a7) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(70d4f018) SHA1(089c8cee33157d43c1946a23ff7e75af65b94ebd) )
ROM_END

ROM_START( lhaunt_7 ) // 070402
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_070402.rom", 0x00000, 0x40000, CRC(16e921d9) SHA1(66849020a451d0c1dc4ac310e98fa7ef6a962548) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter.001",   0x000000, 0x80000, CRC(c541ce0f) SHA1(5b5ad171168f204e1a4892a4be06a4983dcfc5e5) )
	ROM_LOAD( "luckyhaunter.002",   0x100000, 0x80000, CRC(ea49e52f) SHA1(7c6b0da1c4ebca5439dda7d38882055b47d00169) )
	ROM_LOAD( "luckyhaunter.003",   0x200000, 0x80000, CRC(f601cd05) SHA1(b983d703ff5f5795ed60248cd4affc91f983072c) )
	ROM_LOAD( "luckyhaunter.004",   0x300000, 0x80000, CRC(10040104) SHA1(966abcccb67949b73f84ffc85ead70b10856fa5b) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(26ca8ed7) SHA1(6f444a2f1a2e04116ab0a4622d37409913bf6ff1) )
ROM_END

ROM_START( lhaunt_8 ) // 070604
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_070604.rom", 0x00000, 0x40000, CRC(a9e80888) SHA1(57fdfc1149db1555808040a7a902dec5f1389943) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter.001",   0x000000, 0x80000, CRC(c541ce0f) SHA1(5b5ad171168f204e1a4892a4be06a4983dcfc5e5) )
	ROM_LOAD( "luckyhaunter.002",   0x100000, 0x80000, CRC(ea49e52f) SHA1(7c6b0da1c4ebca5439dda7d38882055b47d00169) )
	ROM_LOAD( "luckyhaunter.003",   0x200000, 0x80000, CRC(f601cd05) SHA1(b983d703ff5f5795ed60248cd4affc91f983072c) )
	ROM_LOAD( "luckyhaunter.004",   0x300000, 0x80000, CRC(10040104) SHA1(966abcccb67949b73f84ffc85ead70b10856fa5b) )
	ROM_LOAD( "luckyhaunter_m.005", 0x080000, 0x80000, CRC(b50c90a3) SHA1(74749f4ffb5b0630631b511fc3230c6e7b50dc3b) )
	ROM_LOAD( "luckyhaunter_m.006", 0x180000, 0x80000, CRC(4eaaab64) SHA1(9fed16f8e0308200fd16c4b1e511e1bf6c22ae66) )
	ROM_LOAD( "luckyhaunter_m.007", 0x280000, 0x80000, CRC(64d16ba9) SHA1(3b897183d6e0f1256be7657441f234fc72077682) )
	ROM_LOAD( "luckyhaunter_m.008", 0x380000, 0x80000, CRC(1bdf6252) SHA1(7b5ae82a95a744b236e109024d47b526dccf9c14) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(e0079bf5) SHA1(29da8ccad2ded4b498d051125c9e136b55d84576) )
ROM_END


ROM_START( lhaunt_10 ) // 090712 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_e_090712.rom", 0x00000, 0x40000, CRC(16a3d1f3) SHA1(a1350d957d06a679db7f27eb949ddb3befe178d8) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "lh_ent.001", 0x000000, 0x80000, CRC(66de9088) SHA1(b8287b4d9eeeb6502b656dc9d104048364c55326) )
	ROM_LOAD( "lh_ent.002", 0x100000, 0x80000, CRC(04df45d7) SHA1(f74d72244fb3cefc3599b68af0fec01f9d1a17a1) )
	ROM_LOAD( "lh_ent.003", 0x200000, 0x80000, CRC(c4706ef2) SHA1(6a27af2e3e5893c0e8c0ce753fe223ab52451bd5) )
	ROM_LOAD( "lh_ent.004", 0x300000, 0x80000, CRC(4f91e005) SHA1(c65771ac4fd9affae7a9a4c5a5fb748fd0830b04) )
	ROM_LOAD( "lh_ent.005", 0x080000, 0x80000, CRC(63fc8d37) SHA1(c38a359c6cea75c7981a004cc32e80b5fda4fa6c) )
	ROM_LOAD( "lh_ent.006", 0x180000, 0x80000, CRC(3b1c0dcf) SHA1(2b31d145803a5500b070eabffccdc14e4e7540cf) )
	ROM_LOAD( "lh_ent.007", 0x280000, 0x80000, CRC(8d6549f3) SHA1(c890d37bb99e23550fc264562b5230edbca0afe8) )
	ROM_LOAD( "lh_ent.008", 0x380000, 0x80000, CRC(66ca6dac) SHA1(f8ffde6f1f0b5bb20cd8cbb51abaf92ff82b8217) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(55a909f7) SHA1(5375b656bc4490521f25005350fa6a97d753c2ff) )
ROM_END

ROM_START( lhaunt_11 ) // 100331 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "lh_x_100331.rom", 0x00000, 0x40000, CRC(992c4f25) SHA1(23618fb387e24fae26c7cd184bc61be61c11ad62) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "luckyhaunter_ent.001", 0x000000, 0x80000, CRC(f4d3eb09) SHA1(ac6796250a60cf926087671e9fac3980a136d86e) )
	ROM_LOAD( "luckyhaunter_ent.002", 0x100000, 0x80000, CRC(a2a88abc) SHA1(d417ab939faab373fc50abec5dc6edc160f0fdfb) )
	ROM_LOAD( "luckyhaunter_ent.003", 0x200000, 0x80000, CRC(7a567d2b) SHA1(61892fe1e93c6bae4c9cd2840dc3c2f71b3cb97d) )
	ROM_LOAD( "luckyhaunter_ent.004", 0x300000, 0x80000, CRC(bf8fbcdb) SHA1(6045d6f8331d405f9d43d51915253f426529b77b) )
	ROM_LOAD( "luckyhaunter_ent.005", 0x080000, 0x80000, CRC(eed42a52) SHA1(4a503c3ac7ffd0cd22efc24bb5a830147ce6cee4) )
	ROM_LOAD( "luckyhaunter_ent.006", 0x180000, 0x80000, CRC(afb10953) SHA1(33937879f5b2e80d178bff12866f70c7bc339088) )
	ROM_LOAD( "luckyhaunter_ent.007", 0x280000, 0x80000, CRC(132d82c3) SHA1(519f6ee58ba7469aebc69628e694c81f19558522) )
	ROM_LOAD( "luckyhaunter_ent.008", 0x380000, 0x80000, CRC(32de0c1b) SHA1(1f665bc9f198ddeb2fd16bfe66a111ce21107c59) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(894c960f) SHA1(44a0ee853475fdac5a4bd49ca4ab3e5ff7c24efc) )
ROM_END


/*********************************************************
   Garage

    "Russia" sets use different gfx roms 1-4.
        The official list of hashes shows only updated roms.

**********************************************************/

ROM_START( garage ) // 040122
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_m_040122.rom", 0x00000, 0x40000, CRC(6d844d9f) SHA1(327e55d1f4bdc0ad0556faa2fbdaa05b9a5f1c16) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage_m.001", 0x000000, 0x80000, CRC(57acd4bc) SHA1(8796f463787c359cce6ac09c1b6895f871bbb7c9) )
	ROM_LOAD( "garage_m.002", 0x100000, 0x80000, CRC(6d591fa3) SHA1(ddbdf87e0e88dc848b963fbfcb6e14d7b3b9efdc) )
	ROM_LOAD( "garage_m.003", 0x200000, 0x80000, CRC(6a15eeda) SHA1(ac35a20893b0518a159207401f6b7f58e3de45fa) )
	ROM_LOAD( "garage_m.004", 0x300000, 0x80000, CRC(38f2cd3c) SHA1(5a4463ac352e4e340c6aaa61102841541e9f4c48) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( garage_2 ) // 040123
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_m_040123.rom", 0x00000, 0x40000, CRC(afac9e74) SHA1(3051c99d22cfe46b532fcc59a0b98eec186f4a76) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage_m.001", 0x000000, 0x80000, CRC(57acd4bc) SHA1(8796f463787c359cce6ac09c1b6895f871bbb7c9) )
	ROM_LOAD( "garage_m.002", 0x100000, 0x80000, CRC(6d591fa3) SHA1(ddbdf87e0e88dc848b963fbfcb6e14d7b3b9efdc) )
	ROM_LOAD( "garage_m.003", 0x200000, 0x80000, CRC(6a15eeda) SHA1(ac35a20893b0518a159207401f6b7f58e3de45fa) )
	ROM_LOAD( "garage_m.004", 0x300000, 0x80000, CRC(38f2cd3c) SHA1(5a4463ac352e4e340c6aaa61102841541e9f4c48) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( garage_3 ) // 040216
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_m_040216.rom", 0x00000, 0x40000, CRC(76886e65) SHA1(321c4106ce07e195a05eacdef6387d61d5e58bb9) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage_m.001", 0x000000, 0x80000, CRC(57acd4bc) SHA1(8796f463787c359cce6ac09c1b6895f871bbb7c9) )
	ROM_LOAD( "garage_m.002", 0x100000, 0x80000, CRC(6d591fa3) SHA1(ddbdf87e0e88dc848b963fbfcb6e14d7b3b9efdc) )
	ROM_LOAD( "garage_m.003", 0x200000, 0x80000, CRC(6a15eeda) SHA1(ac35a20893b0518a159207401f6b7f58e3de45fa) )
	ROM_LOAD( "garage_m.004", 0x300000, 0x80000, CRC(38f2cd3c) SHA1(5a4463ac352e4e340c6aaa61102841541e9f4c48) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( garage_4 ) // 040219
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_m_040219.rom", 0x00000, 0x40000, CRC(49fe4a55) SHA1(df55df0065b4718d2b0c7ff3da85f5d66c2dd95f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage_m.001", 0x000000, 0x80000, CRC(57acd4bc) SHA1(8796f463787c359cce6ac09c1b6895f871bbb7c9) )
	ROM_LOAD( "garage_m.002", 0x100000, 0x80000, CRC(6d591fa3) SHA1(ddbdf87e0e88dc848b963fbfcb6e14d7b3b9efdc) )
	ROM_LOAD( "garage_m.003", 0x200000, 0x80000, CRC(6a15eeda) SHA1(ac35a20893b0518a159207401f6b7f58e3de45fa) )
	ROM_LOAD( "garage_m.004", 0x300000, 0x80000, CRC(38f2cd3c) SHA1(5a4463ac352e4e340c6aaa61102841541e9f4c48) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(2fc9133e) SHA1(b1e0df97316e8614aaa770f8766e3a10aa55d686) )
ROM_END

ROM_START( garage_5 ) // 050311
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_m_050311.rom", 0x00000, 0x40000, CRC(405aee88) SHA1(356a8c309434ae4ad6b6fab97aeaece8aa60a730) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage_m.001", 0x000000, 0x80000, CRC(57acd4bc) SHA1(8796f463787c359cce6ac09c1b6895f871bbb7c9) )
	ROM_LOAD( "garage_m.002", 0x100000, 0x80000, CRC(6d591fa3) SHA1(ddbdf87e0e88dc848b963fbfcb6e14d7b3b9efdc) )
	ROM_LOAD( "garage_m.003", 0x200000, 0x80000, CRC(6a15eeda) SHA1(ac35a20893b0518a159207401f6b7f58e3de45fa) )
	ROM_LOAD( "garage_m.004", 0x300000, 0x80000, CRC(38f2cd3c) SHA1(5a4463ac352e4e340c6aaa61102841541e9f4c48) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(d379dd5f) SHA1(c9302cfebed8c9120faf04b9f45671720cfebac0) )
ROM_END

ROM_START( garage_6 ) // 070213
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_070213.rom", 0x00000, 0x40000, CRC(2ae974c3) SHA1(61076d4154befffd3f90ae32b9bfa0e8d003ae72) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage.001",   0x000000, 0x80000, CRC(15349b8c) SHA1(4f59e67114d7440dab82655787a6df9404bcb4b2) )
	ROM_LOAD( "garage.002",   0x100000, 0x80000, CRC(634d1f94) SHA1(d979b0c22c7722eea8789296e9bd3f65b27a2b50) )
	ROM_LOAD( "garage.003",   0x200000, 0x80000, CRC(3fb65c28) SHA1(0b3cacc8907267be84051c562bce1fc82701a5d5) )
	ROM_LOAD( "garage.004",   0x300000, 0x80000, CRC(fc489452) SHA1(ac421f99c3ac3c9b7adf274881c00cbee65b6df5) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(c6776fac) SHA1(cdbc2d62e0cf891b052ca856c98885e79cf2dc62) )
ROM_END

ROM_START( garage_7 ) // 070329
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_070329.rom", 0x00000, 0x40000, CRC(2bd50a8f) SHA1(b0c66eb0ae40b87f81ee5bb66dc4ff7d7acd14bd) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage.001",   0x000000, 0x80000, CRC(15349b8c) SHA1(4f59e67114d7440dab82655787a6df9404bcb4b2) )
	ROM_LOAD( "garage.002",   0x100000, 0x80000, CRC(634d1f94) SHA1(d979b0c22c7722eea8789296e9bd3f65b27a2b50) )
	ROM_LOAD( "garage.003",   0x200000, 0x80000, CRC(3fb65c28) SHA1(0b3cacc8907267be84051c562bce1fc82701a5d5) )
	ROM_LOAD( "garage.004",   0x300000, 0x80000, CRC(fc489452) SHA1(ac421f99c3ac3c9b7adf274881c00cbee65b6df5) )
	ROM_LOAD( "garage_m.005", 0x080000, 0x80000, CRC(5bf85bc5) SHA1(ff9d2b9cbcd2af6f5fda972e387820d830c196a9) )
	ROM_LOAD( "garage_m.006", 0x180000, 0x80000, CRC(e5082b26) SHA1(6547409d39dd51498ce8e3f82ff813a8ac3c6522) )
	ROM_LOAD( "garage_m.007", 0x280000, 0x80000, CRC(dfa2ceb1) SHA1(ec7de8a8f6e7785a563df973841cc1f1603f79fc) )
	ROM_LOAD( "garage_m.008", 0x380000, 0x80000, CRC(90c5416b) SHA1(583bfb517bc2e30d7b7903aa19fc3b4b5188d7d2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(c6776fac) SHA1(cdbc2d62e0cf891b052ca856c98885e79cf2dc62) )
ROM_END


ROM_START( garage_9 ) // 090715 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gg_e_090715.rom", 0x00000, 0x40000, CRC(4e722a18) SHA1(cda2f605ffa321654d9179504558d66d081e53b4) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "garage_ent.001", 0x000000, 0x80000, CRC(9b8386af) SHA1(e0ae0af31799ce04c5a0cb868ef30e40f5ae23a4) )
	ROM_LOAD( "garage_ent.002", 0x100000, 0x80000, CRC(803cc291) SHA1(5448b963bfbe3a20d118d8d4bf474a75a486e325) )
	ROM_LOAD( "garage_ent.003", 0x200000, 0x80000, CRC(714c8051) SHA1(dbde0b70f032620a03a946032145a039d5faa4bd) )
	ROM_LOAD( "garage_ent.004", 0x300000, 0x80000, CRC(7c4e515c) SHA1(bd6075bfab0a2df5305059c2a9d6fc26a58f6705) )
	ROM_LOAD( "garage_ent.005", 0x080000, 0x80000, CRC(21bf0f76) SHA1(a20e3f9c055e6a68d1c58b1b54b25e532a6c5a97) )
	ROM_LOAD( "garage_ent.006", 0x180000, 0x80000, CRC(f0f9d1ab) SHA1(8ef8f69e730137372279d6024ee59eda68463be9) )
	ROM_LOAD( "garage_ent.007", 0x280000, 0x80000, CRC(a3b88049) SHA1(06b49bb333f3d303885bd41b052059de847cf1d8) )
	ROM_LOAD( "garage_ent.008", 0x380000, 0x80000, CRC(4331a5e9) SHA1(1593dfd998c14a03aa3e66dc7a102b6a94d159d9) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(ea8cd62a) SHA1(20214f080d85131102af9b5dae49fadcc8953b98) )
ROM_END


/*********************************************************
   Rock Climber

    "Russia" sets use different gfx roms 1-4.
        The official list of hashes shows only updated roms.

**********************************************************/

ROM_START( rclimb ) // 040815
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rc_m_040815.rom", 0x00000, 0x40000, CRC(5efd5c86) SHA1(593e64bfe57ba271c04bdd2a35c9484c4efaaa00))

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rockclimber_m.001", 0x000000, 0x80000, CRC(934f18c7) SHA1(da3a7cddc68e104d415d947e89c0e7f0d067c056) )
	ROM_LOAD( "rockclimber_m.002", 0x100000, 0x80000, CRC(7364bd2b) SHA1(c0edfd3b8de813c95fe5d6072662fa0e39fec89e) )
	ROM_LOAD( "rockclimber_m.003", 0x200000, 0x80000, CRC(e7befb17) SHA1(8a214680142cd657784a667ab3f6422165fea224) )
	ROM_LOAD( "rockclimber_m.004", 0x300000, 0x80000, CRC(dc6d43a0) SHA1(62fc47136775f3fa9369857ec91fe897a1f1ebd6) )
	ROM_LOAD( "rockclimber_m.005", 0x080000, 0x80000, CRC(ea127c3d) SHA1(a6391eed69a4723b68d727f59b6baebe51633e66) )
	ROM_LOAD( "rockclimber_m.006", 0x180000, 0x80000, CRC(277fa273) SHA1(6320e6c5b5e48dc451cc48189054c42d85e8ccc1) )
	ROM_LOAD( "rockclimber_m.007", 0x280000, 0x80000, CRC(3ca7f69a) SHA1(878cca181d915dc3548d5285a4bbb51aef31a64e) )
	ROM_LOAD( "rockclimber_m.008", 0x380000, 0x80000, CRC(8cf6b4c2) SHA1(4c36c217b83c82acfdd615f5547bf597af7b8833) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(5b7ff0e2) SHA1(bd1920d975281256dbd36adf31985931c4f1e63b) )
ROM_END

ROM_START( rclimb_2 ) // 040823
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rc_m_040823.rom", 0x00000, 0x40000, CRC(619239c7) SHA1(31cf4d7f50102d35556817273893182e30c9a70c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rockclimber_m.001", 0x000000, 0x80000, CRC(934f18c7) SHA1(da3a7cddc68e104d415d947e89c0e7f0d067c056) )
	ROM_LOAD( "rockclimber_m.002", 0x100000, 0x80000, CRC(7364bd2b) SHA1(c0edfd3b8de813c95fe5d6072662fa0e39fec89e) )
	ROM_LOAD( "rockclimber_m.003", 0x200000, 0x80000, CRC(e7befb17) SHA1(8a214680142cd657784a667ab3f6422165fea224) )
	ROM_LOAD( "rockclimber_m.004", 0x300000, 0x80000, CRC(dc6d43a0) SHA1(62fc47136775f3fa9369857ec91fe897a1f1ebd6) )
	ROM_LOAD( "rockclimber_m.005", 0x080000, 0x80000, CRC(ea127c3d) SHA1(a6391eed69a4723b68d727f59b6baebe51633e66) )
	ROM_LOAD( "rockclimber_m.006", 0x180000, 0x80000, CRC(277fa273) SHA1(6320e6c5b5e48dc451cc48189054c42d85e8ccc1) )
	ROM_LOAD( "rockclimber_m.007", 0x280000, 0x80000, CRC(3ca7f69a) SHA1(878cca181d915dc3548d5285a4bbb51aef31a64e) )
	ROM_LOAD( "rockclimber_m.008", 0x380000, 0x80000, CRC(8cf6b4c2) SHA1(4c36c217b83c82acfdd615f5547bf597af7b8833) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( rclimb_3 ) // 040827
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rc_m_040827.rom", 0x00000, 0x40000, CRC(3ba55647) SHA1(56e96be0d9782da4b3d5d911ea67962257626ae0) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rockclimber_m.001", 0x000000, 0x80000, CRC(934f18c7) SHA1(da3a7cddc68e104d415d947e89c0e7f0d067c056) )
	ROM_LOAD( "rockclimber_m.002", 0x100000, 0x80000, CRC(7364bd2b) SHA1(c0edfd3b8de813c95fe5d6072662fa0e39fec89e) )
	ROM_LOAD( "rockclimber_m.003", 0x200000, 0x80000, CRC(e7befb17) SHA1(8a214680142cd657784a667ab3f6422165fea224) )
	ROM_LOAD( "rockclimber_m.004", 0x300000, 0x80000, CRC(dc6d43a0) SHA1(62fc47136775f3fa9369857ec91fe897a1f1ebd6) )
	ROM_LOAD( "rockclimber_m.005", 0x080000, 0x80000, CRC(ea127c3d) SHA1(a6391eed69a4723b68d727f59b6baebe51633e66) )
	ROM_LOAD( "rockclimber_m.006", 0x180000, 0x80000, CRC(277fa273) SHA1(6320e6c5b5e48dc451cc48189054c42d85e8ccc1) )
	ROM_LOAD( "rockclimber_m.007", 0x280000, 0x80000, CRC(3ca7f69a) SHA1(878cca181d915dc3548d5285a4bbb51aef31a64e) )
	ROM_LOAD( "rockclimber_m.008", 0x380000, 0x80000, CRC(8cf6b4c2) SHA1(4c36c217b83c82acfdd615f5547bf597af7b8833) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(4c709bc6) SHA1(3d9a2a85818533ac014bac47821dbfab306eef31) )
ROM_END

ROM_START( rclimb_4 ) // 070322
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rc_070322.rom", 0x00000, 0x40000, CRC(200fcc35) SHA1(a9f71c5a3cb328f6522436693364d85586cca796) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rockclimber.001",   0x000000, 0x80000, CRC(2c6c10b6) SHA1(b2f96fb90a5f2150d84fe5012414c74fcbde81d1) )
	ROM_LOAD( "rockclimber.002",   0x100000, 0x80000, CRC(7def062a) SHA1(d418f5e8b80680ecdc10159322c83d298c96cf4d) )
	ROM_LOAD( "rockclimber.003",   0x200000, 0x80000, CRC(313a6490) SHA1(96430117f45cc0a34a7dbc51c0df032595451390) )
	ROM_LOAD( "rockclimber.004",   0x300000, 0x80000, CRC(0cfaa33f) SHA1(df289b816fdb094228d518e823a3e660f5b8a0b6) )
	ROM_LOAD( "rockclimber_m.005", 0x080000, 0x80000, CRC(ea127c3d) SHA1(a6391eed69a4723b68d727f59b6baebe51633e66) )
	ROM_LOAD( "rockclimber_m.006", 0x180000, 0x80000, CRC(277fa273) SHA1(6320e6c5b5e48dc451cc48189054c42d85e8ccc1) )
	ROM_LOAD( "rockclimber_m.007", 0x280000, 0x80000, CRC(3ca7f69a) SHA1(878cca181d915dc3548d5285a4bbb51aef31a64e) )
	ROM_LOAD( "rockclimber_m.008", 0x380000, 0x80000, CRC(8cf6b4c2) SHA1(4c36c217b83c82acfdd615f5547bf597af7b8833) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(70144314) SHA1(f30a4d655082df7245899b2faaea5ce8af26f2b9) )
ROM_END

ROM_START( rclimb_5 ) // 070621
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rc_070621.rom", 0x00000, 0x40000, CRC(7f5223a6) SHA1(9aaed66ce6cd12faad09e2b74f8ae764a9707be7) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rockclimber.001",   0x000000, 0x80000, CRC(2c6c10b6) SHA1(b2f96fb90a5f2150d84fe5012414c74fcbde81d1) )
	ROM_LOAD( "rockclimber.002",   0x100000, 0x80000, CRC(7def062a) SHA1(d418f5e8b80680ecdc10159322c83d298c96cf4d) )
	ROM_LOAD( "rockclimber.003",   0x200000, 0x80000, CRC(313a6490) SHA1(96430117f45cc0a34a7dbc51c0df032595451390) )
	ROM_LOAD( "rockclimber.004",   0x300000, 0x80000, CRC(0cfaa33f) SHA1(df289b816fdb094228d518e823a3e660f5b8a0b6) )
	ROM_LOAD( "rockclimber_m.005", 0x080000, 0x80000, CRC(ea127c3d) SHA1(a6391eed69a4723b68d727f59b6baebe51633e66) )
	ROM_LOAD( "rockclimber_m.006", 0x180000, 0x80000, CRC(277fa273) SHA1(6320e6c5b5e48dc451cc48189054c42d85e8ccc1) )
	ROM_LOAD( "rockclimber_m.007", 0x280000, 0x80000, CRC(3ca7f69a) SHA1(878cca181d915dc3548d5285a4bbb51aef31a64e) )
	ROM_LOAD( "rockclimber_m.008", 0x380000, 0x80000, CRC(8cf6b4c2) SHA1(4c36c217b83c82acfdd615f5547bf597af7b8833) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(2030f37d) SHA1(56e0df6310989da67667b37d2734046eabb87cda) )
ROM_END


ROM_START( rclimb_7 ) // 090716 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rc_e_090716.rom", 0x00000, 0x40000, CRC(35bd6e28) SHA1(ee99956131ecfe3c4f05acc11bcf20a8a10403bd) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rc_ent.001", 0x000000, 0x80000, CRC(246db785) SHA1(348624c5534de72422be23da289ab63b3e4ba3f5) )
	ROM_LOAD( "rc_ent.002", 0x100000, 0x80000, CRC(4ddf95c1) SHA1(b83d0239f9e877fc5ae1557fd01668c126334501) )
	ROM_LOAD( "rc_ent.003", 0x200000, 0x80000, CRC(7230bb2e) SHA1(dbcd82961ff9ba7bc05f749f07ce1ef7e2107690) )
	ROM_LOAD( "rc_ent.004", 0x300000, 0x80000, CRC(ad2850c8) SHA1(9ced6e35656b81c27bfb300383e2f61bd359a143) )
	ROM_LOAD( "rc_ent.005", 0x080000, 0x80000, CRC(95f5fbb1) SHA1(ea91c72594c9cb6c42a005a11ad0ce899724c509) )
	ROM_LOAD( "rc_ent.006", 0x180000, 0x80000, CRC(35c50330) SHA1(b9c6e6f84f6efe62b0c2eef5e366d2423612e01e) )
	ROM_LOAD( "rc_ent.007", 0x280000, 0x80000, CRC(61bea923) SHA1(16b54a310de4a8af158c1381be464f601e22c825) )
	ROM_LOAD( "rc_ent.008", 0x380000, 0x80000, CRC(f4601d40) SHA1(64bc63db23e934104a9d68b77a56322a5b0540b8) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(9e3ca059) SHA1(ccf7cdd29dbbd50dd84a49bc8d01299b87c3a410) )
ROM_END


/*********************************************************
   Sweet Life
**********************************************************/

ROM_START( sweetl ) // 041220
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl_m_041220.rom", 0x00000, 0x40000, CRC(851b85c6) SHA1(a5db94d94fe82d06f3fac1c16aed5358fcb92f29) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife.001", 0x000000, 0x80000, CRC(a096c786) SHA1(81f6b083cb089e9412a8506889196354c670d945) )
	ROM_LOAD( "sweetlife.002", 0x100000, 0x80000, CRC(c5e1e22c) SHA1(973ad27681a0f3beee7084b1b85fc9deb79d638e) )
	ROM_LOAD( "sweetlife.003", 0x200000, 0x80000, CRC(af335323) SHA1(b8afdce231a8ec0f313cc47e00a27f05461bbbc4) )
	ROM_LOAD( "sweetlife.004", 0x300000, 0x80000, CRC(a35c7503) SHA1(78f7a868660bbaa066e8e9e341db52018aaf3af1) )
	ROM_LOAD( "sweetlife.005", 0x080000, 0x80000, CRC(e2d6b632) SHA1(65d05e55671b8c335cae2dfbf6a6f5bd8cc90e2c) )
	ROM_LOAD( "sweetlife.006", 0x180000, 0x80000, CRC(d34e0905) SHA1(cc4afe64fb9052a31f759be41ff07a727e0a9093) )
	ROM_LOAD( "sweetlife.007", 0x280000, 0x80000, CRC(978b67bb) SHA1(87357d5832588f00272bd76df736c06c599f3853) )
	ROM_LOAD( "sweetlife.008", 0x380000, 0x80000, CRC(75954355) SHA1(e6ef2b70d859b61e8e3d1751de8558b8778e502d) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1547f59e) SHA1(7f19c8a4ea8d09270d04b1185d5e0957fc3a102d) )
ROM_END

ROM_START( sweetl_2 ) // 070412
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl_070412.rom", 0x00000, 0x40000, CRC(2d2cd8ec) SHA1(40f7778c6eb4681452635249a708e9dbc15c9045) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife.001", 0x000000, 0x80000, CRC(a096c786) SHA1(81f6b083cb089e9412a8506889196354c670d945) )
	ROM_LOAD( "sweetlife.002", 0x100000, 0x80000, CRC(c5e1e22c) SHA1(973ad27681a0f3beee7084b1b85fc9deb79d638e) )
	ROM_LOAD( "sweetlife.003", 0x200000, 0x80000, CRC(af335323) SHA1(b8afdce231a8ec0f313cc47e00a27f05461bbbc4) )
	ROM_LOAD( "sweetlife.004", 0x300000, 0x80000, CRC(a35c7503) SHA1(78f7a868660bbaa066e8e9e341db52018aaf3af1) )
	ROM_LOAD( "sweetlife.005", 0x080000, 0x80000, CRC(e2d6b632) SHA1(65d05e55671b8c335cae2dfbf6a6f5bd8cc90e2c) )
	ROM_LOAD( "sweetlife.006", 0x180000, 0x80000, CRC(d34e0905) SHA1(cc4afe64fb9052a31f759be41ff07a727e0a9093) )
	ROM_LOAD( "sweetlife.007", 0x280000, 0x80000, CRC(978b67bb) SHA1(87357d5832588f00272bd76df736c06c599f3853) )
	ROM_LOAD( "sweetlife.008", 0x380000, 0x80000, CRC(75954355) SHA1(e6ef2b70d859b61e8e3d1751de8558b8778e502d) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(b5deda35) SHA1(adc4dbbd73f9c4305abc594617218c32c31e0b8c) )
ROM_END

ROM_START( sweetl_3 ) // 090720 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl_e_090720.rom", 0x00000, 0x40000, CRC(97b2f1dd) SHA1(1c14dbeb35fe58f65c2a438e7443d3c37a00d48f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife_ent.001", 0x000000, 0x80000, CRC(954cec46) SHA1(1068cfe2d68f68db7523c4cd9aad67f9eba5f370) )
	ROM_LOAD( "sweetlife_ent.002", 0x100000, 0x80000, CRC(39a4dfef) SHA1(2344aa37b4943777fa8276fe7c9829e4e6f5fe21) )
	ROM_LOAD( "sweetlife_ent.003", 0x200000, 0x80000, CRC(5448a09a) SHA1(6c72e42094eeb9cbbc9e67d029376a31c01762dd) )
	ROM_LOAD( "sweetlife_ent.004", 0x300000, 0x80000, CRC(b7a5f2cb) SHA1(75f9c9f031fc5c2f249020bafbed23caa233efcc) )
	ROM_LOAD( "sweetlife_ent.005", 0x080000, 0x80000, CRC(b2b45395) SHA1(65b4516da1698ac6e02accd7b07b4e3ad55bcd17) )
	ROM_LOAD( "sweetlife_ent.006", 0x180000, 0x80000, CRC(b9b5a8fe) SHA1(a985c59813cfba99abd5817c3bd9d3fe2c0ae38e) )
	ROM_LOAD( "sweetlife_ent.007", 0x280000, 0x80000, CRC(28b33238) SHA1(3668fb0eed46c9cccc155246e4832fc902be597d) )
	ROM_LOAD( "sweetlife_ent.008", 0x380000, 0x80000, CRC(7aa675a7) SHA1(9027b889d5370d9daf1f880d43b1252a381aade8) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(fd66d54b) SHA1(769cc41e1f3c25518aaf4c4d9ab3fe0a3e525bd7) )
ROM_END


/*********************************************************
   Sweet Life 2
**********************************************************/

ROM_START( sweetl2 ) // 071217
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl2_071217.rom", 0x00000, 0x40000, CRC(b6299b02) SHA1(7051f9034bf3181d6f3dc66c0048c4a57685d2a8) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife2.001", 0x000000, 0x80000, CRC(b1e6157c) SHA1(e042aaaf85b13865d56d1709be280f5a3d5c95e3) )
	ROM_LOAD( "sweetlife2.002", 0x100000, 0x80000, CRC(8bc54047) SHA1(9179cf02376fec77ae50b5603de66754237698f8) )
	ROM_LOAD( "sweetlife2.003", 0x200000, 0x80000, CRC(709c5d34) SHA1(d702576d70ebef667c77376effecc02abbaaa8cd) )
	ROM_LOAD( "sweetlife2.004", 0x300000, 0x80000, CRC(7bcfa2ce) SHA1(8df894e9b7afa52e47e4dabd1802e878bbbeaa80) )
	ROM_LOAD( "sweetlife2.005", 0x080000, 0x80000, CRC(5c385d43) SHA1(ddb362e5894d146ce90acbaafe9dd7aad2a7c242) )
	ROM_LOAD( "sweetlife2.006", 0x180000, 0x80000, CRC(868fe1cb) SHA1(692679f8242950e009c30cc084c4ddc5d1963502) )
	ROM_LOAD( "sweetlife2.007", 0x280000, 0x80000, CRC(6ce87282) SHA1(586e08994db4ca2b967d47b16ba5b458e240d30f) )
	ROM_LOAD( "sweetlife2.008", 0x380000, 0x80000, CRC(c2ad2b74) SHA1(c78e3ca5d15acb17ee671d2205405f287ad9c464) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(8dbc9a6e) SHA1(f14b3608a7c1a318fbc20629e292b02f97cb1187) )
ROM_END

ROM_START( sweetl2_2 ) // 080320
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl2_m_080320.rom", 0x00000, 0x40000, CRC(78669405) SHA1(149990bee56d0f94ce1bea17883cbf5545925eb7) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife2.001", 0x000000, 0x80000, CRC(b1e6157c) SHA1(e042aaaf85b13865d56d1709be280f5a3d5c95e3) )
	ROM_LOAD( "sweetlife2.002", 0x100000, 0x80000, CRC(8bc54047) SHA1(9179cf02376fec77ae50b5603de66754237698f8) )
	ROM_LOAD( "sweetlife2.003", 0x200000, 0x80000, CRC(709c5d34) SHA1(d702576d70ebef667c77376effecc02abbaaa8cd) )
	ROM_LOAD( "sweetlife2.004", 0x300000, 0x80000, CRC(7bcfa2ce) SHA1(8df894e9b7afa52e47e4dabd1802e878bbbeaa80) )
	ROM_LOAD( "sweetlife2.005", 0x080000, 0x80000, CRC(5c385d43) SHA1(ddb362e5894d146ce90acbaafe9dd7aad2a7c242) )
	ROM_LOAD( "sweetlife2.006", 0x180000, 0x80000, CRC(868fe1cb) SHA1(692679f8242950e009c30cc084c4ddc5d1963502) )
	ROM_LOAD( "sweetlife2.007", 0x280000, 0x80000, CRC(6ce87282) SHA1(586e08994db4ca2b967d47b16ba5b458e240d30f) )
	ROM_LOAD( "sweetlife2.008", 0x380000, 0x80000, CRC(c2ad2b74) SHA1(c78e3ca5d15acb17ee671d2205405f287ad9c464) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(81ec556f) SHA1(f772e37d9df36e11f97ec2d5fa02f67ec42f277c) )
ROM_END

ROM_START( sweetl2_3 ) // 090525 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl2_l_090525.rom", 0x00000, 0x40000, CRC(79f878b4) SHA1(a21dc4a7986dab7ec9236b2c612c43c1604b5588) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife2_loto.001", 0x000000, 0x80000, CRC(01b8bc85) SHA1(9c6475e8e6e01717d61b5f15f95748a66ea958b7) )
	ROM_LOAD( "sweetlife2_loto.002", 0x100000, 0x80000, CRC(e95f1ecb) SHA1(a538c6df004129ccf54a09ab6dae79d301ee6966) )
	ROM_LOAD( "sweetlife2_loto.003", 0x200000, 0x80000, CRC(506c149a) SHA1(59f680cc5deabca32bee3bb5b46a4cda3e583dc7) )
	ROM_LOAD( "sweetlife2_loto.004", 0x300000, 0x80000, CRC(512012b4) SHA1(ec9f4bb6f97cb0001335a75923dd640e239fdce6) )
	ROM_LOAD( "sweetlife2_loto.005", 0x080000, 0x80000, CRC(4b600255) SHA1(e997e694bdc4b59e9e05ac87c6241b80f9745f43) )
	ROM_LOAD( "sweetlife2_loto.006", 0x180000, 0x80000, CRC(679cd95b) SHA1(e3fa14d87fc25c863cfce313f7f76e0bcaabf070) )
	ROM_LOAD( "sweetlife2_loto.007", 0x280000, 0x80000, CRC(4c325bdd) SHA1(b0a383787ff9211df2e9cc2e48f70e76a7ec9976) )
	ROM_LOAD( "sweetlife2_loto.008", 0x380000, 0x80000, CRC(26d3cff2) SHA1(e8896e03c0bd8bf71dffcfc93785a144a5161e04) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(89adb28a) SHA1(4e019697312f3e74c5597b7ce3af6d2ca32a90d6) )
ROM_END

ROM_START( sweetl2_4 ) // 090812 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "sl2_e_090812.rom", 0x00000, 0x40000, CRC(1ee6b0c9) SHA1(481ec72b87e0419aa435e3876404c1b802aed7dd) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "sweetlife2_ent.001", 0x000000, 0x80000, CRC(378c185d) SHA1(3486bc353c2947375e0f60b6e0f188e95fb7ebcb) )
	ROM_LOAD( "sweetlife2_ent.002", 0x100000, 0x80000, CRC(3f92e579) SHA1(0b1ddd294c6a8d7bb353965a7117ea336c577ff5) )
	ROM_LOAD( "sweetlife2_ent.003", 0x200000, 0x80000, CRC(fc831019) SHA1(46c1a68aa205fd5df9cca37b3b04ecfb6bf4f671) )
	ROM_LOAD( "sweetlife2_ent.004", 0x300000, 0x80000, CRC(074dc8ca) SHA1(1ae8833c490bb8626cf5445fe380b09e2f91f6c0) )
	ROM_LOAD( "sweetlife2_ent.005", 0x080000, 0x80000, CRC(8ead745f) SHA1(9b7e5ea90e7fdb400981fba83794ae85f85ba023) )
	ROM_LOAD( "sweetlife2_ent.006", 0x180000, 0x80000, CRC(64b89085) SHA1(8b12c1679ec460ea4614000eea60fe958c4538ff) )
	ROM_LOAD( "sweetlife2_ent.007", 0x280000, 0x80000, CRC(405df6a2) SHA1(fcdfcd34cfbcaaa4dd12ed795bea3fc257ba2435) )
	ROM_LOAD( "sweetlife2_ent.008", 0x380000, 0x80000, CRC(6cfb55e9) SHA1(f08943cb6989d004ec80e8c7e032d1f471380a7f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(a86d48d2) SHA1(8e9d3d73bade758fef9b1d2539542f4e2c1d7f91) )
ROM_END


/*********************************************************
   Resident

    "Russia" sets use different gfx roms 1-4.
        The official list of hashes shows both roms.

**********************************************************/

ROM_START( resdnt ) // 040415
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_m_040415.rom", 0x00000, 0x40000, CRC(2e06f70c) SHA1(b9f07bc2765d4f366e548007e51b9f605c884ba1) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident_m.001", 0x000000, 0x80000, CRC(e0645da6) SHA1(dd72f4830d8011f603aa6d430f34ac2598005281) )
	ROM_LOAD( "resident_m.002", 0x100000, 0x80000, CRC(dd8de247) SHA1(498c5b931ce65e289f52d8864b603166f81e3dc4) )
	ROM_LOAD( "resident_m.003", 0x200000, 0x80000, CRC(0d346ec2) SHA1(e2456b28825c54c5e16829525627c40611c0083d) )
	ROM_LOAD( "resident_m.004", 0x300000, 0x80000, CRC(1f95aad9) SHA1(51d003288d5ff23b3c981fbaa99d29b66dd2c101) )
	ROM_LOAD( "resident_m.005", 0x080000, 0x80000, CRC(0cfe7d44) SHA1(9f0e4925e815ff9f79188f18e78c0a7b377daa3f) )
	ROM_LOAD( "resident_m.006", 0x180000, 0x80000, CRC(7437904f) SHA1(630c79cd6a990ce7658a1ffabba5a27efba985a1) )
	ROM_LOAD( "resident_m.007", 0x280000, 0x80000, CRC(6e94728a) SHA1(ab414879cb957d9bc8d653b5e3bb2bbf91139ec0) )
	ROM_LOAD( "resident_m.008", 0x380000, 0x80000, CRC(a9f55043) SHA1(4771df3d45bdc0a21b1c479f45e09ac5bab6c94f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(7a76dd58) SHA1(70c240dcbfb30b9119ca8de1fb769891a4c5d0f2) )
ROM_END

ROM_START( resdnt_2 ) // 040513
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_m_040513.rom", 0x00000, 0x40000, CRC(95f74cb3) SHA1(2e4862ac0ad86899b8ce12580ebd217dfb74f6a2) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident_m.001", 0x000000, 0x80000, CRC(e0645da6) SHA1(dd72f4830d8011f603aa6d430f34ac2598005281) )
	ROM_LOAD( "resident_m.002", 0x100000, 0x80000, CRC(dd8de247) SHA1(498c5b931ce65e289f52d8864b603166f81e3dc4) )
	ROM_LOAD( "resident_m.003", 0x200000, 0x80000, CRC(0d346ec2) SHA1(e2456b28825c54c5e16829525627c40611c0083d) )
	ROM_LOAD( "resident_m.004", 0x300000, 0x80000, CRC(1f95aad9) SHA1(51d003288d5ff23b3c981fbaa99d29b66dd2c101) )
	ROM_LOAD( "resident_m.005", 0x080000, 0x80000, CRC(0cfe7d44) SHA1(9f0e4925e815ff9f79188f18e78c0a7b377daa3f) )
	ROM_LOAD( "resident_m.006", 0x180000, 0x80000, CRC(7437904f) SHA1(630c79cd6a990ce7658a1ffabba5a27efba985a1) )
	ROM_LOAD( "resident_m.007", 0x280000, 0x80000, CRC(6e94728a) SHA1(ab414879cb957d9bc8d653b5e3bb2bbf91139ec0) )
	ROM_LOAD( "resident_m.008", 0x380000, 0x80000, CRC(a9f55043) SHA1(4771df3d45bdc0a21b1c479f45e09ac5bab6c94f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(7a76dd58) SHA1(70c240dcbfb30b9119ca8de1fb769891a4c5d0f2) )
ROM_END

ROM_START( resdnt_3 ) // 070222
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_070222.rom", 0x00000, 0x40000, CRC(8aec434e) SHA1(e83c57bbde3379b4d09aa615f2f3fc82d5c5a3cb) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident.001",   0x000000, 0x80000, CRC(5a1a24f1) SHA1(da601ed84603b880d2ce4b98784067531d5294b2) )
	ROM_LOAD( "resident.002",   0x100000, 0x80000, CRC(9a8bfa95) SHA1(392690b7031ce2c8ff87843befeefa0e68ae8fce) )
	ROM_LOAD( "resident.003",   0x200000, 0x80000, CRC(0d0db860) SHA1(93bc2b0bb51d754e19f58d8388c837ba2c7ba5ee) )
	ROM_LOAD( "resident.004",   0x300000, 0x80000, CRC(1d3a5b92) SHA1(c8bebca6beb225911c26e30e090d66b724733d59) )
	ROM_LOAD( "resident_m.005", 0x080000, 0x80000, CRC(0cfe7d44) SHA1(9f0e4925e815ff9f79188f18e78c0a7b377daa3f) )
	ROM_LOAD( "resident_m.006", 0x180000, 0x80000, CRC(7437904f) SHA1(630c79cd6a990ce7658a1ffabba5a27efba985a1) )
	ROM_LOAD( "resident_m.007", 0x280000, 0x80000, CRC(6e94728a) SHA1(ab414879cb957d9bc8d653b5e3bb2bbf91139ec0) )
	ROM_LOAD( "resident_m.008", 0x380000, 0x80000, CRC(a9f55043) SHA1(4771df3d45bdc0a21b1c479f45e09ac5bab6c94f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(0abfdafa) SHA1(582231bf79f6c97e62fcfc355de3c0386d3a72d4) )
ROM_END

ROM_START( resdnt_5 ) // 090722 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_e_090722.rom", 0x00000, 0x40000, CRC(f3bf53e8) SHA1(c321f81cb3389daa3309c0849b50d0ba4e6b9fa1) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident_ent.001", 0x000000, 0x80000, CRC(c14a6d1d) SHA1(6bd077db1faf148a7bd480db7a8b23f0deea0e90) )
	ROM_LOAD( "resident_ent.002", 0x100000, 0x80000, CRC(4b76311c) SHA1(6d945787a37ddff9a2157e1ecebffdf254c67f83) )
	ROM_LOAD( "resident_ent.003", 0x200000, 0x80000, CRC(0f920bc6) SHA1(4058396a8ea2413c8e3f430130b5452a7466af45) )
	ROM_LOAD( "resident_ent.004", 0x300000, 0x80000, CRC(2e175050) SHA1(49392578a0a6f472ce41dbf0a08c35b67dd9ca5a) )
	ROM_LOAD( "resident_ent.005", 0x080000, 0x80000, CRC(c9360af3) SHA1(7c9a4ac4137b225e2cbebae7afdc4513c67ff9ec) )
	ROM_LOAD( "resident_ent.006", 0x180000, 0x80000, CRC(69224185) SHA1(2016c976570727658a8dbdb7e8844df384143ca8) )
	ROM_LOAD( "resident_ent.007", 0x280000, 0x80000, CRC(7e2eef27) SHA1(f2acc7fd8e5917523efa7028d60f737cc2330c71) )
	ROM_LOAD( "resident_ent.008", 0x380000, 0x80000, CRC(d4924f74) SHA1(62f13413a8d7bbcfe833a6d7283e1c726ed06a52) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( resdnt_6 ) // 100311
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_m_100311.rom", 0x00000, 0x40000, CRC(ed51b591) SHA1(baca7320ea67138ea02a3b6d78ad68ad40986f88) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident_m.001", 0x000000, 0x80000, CRC(e0645da6) SHA1(dd72f4830d8011f603aa6d430f34ac2598005281) )
	ROM_LOAD( "resident_m.002", 0x100000, 0x80000, CRC(dd8de247) SHA1(498c5b931ce65e289f52d8864b603166f81e3dc4) )
	ROM_LOAD( "resident_m.003", 0x200000, 0x80000, CRC(0d346ec2) SHA1(e2456b28825c54c5e16829525627c40611c0083d) )
	ROM_LOAD( "resident_m.004", 0x300000, 0x80000, CRC(1f95aad9) SHA1(51d003288d5ff23b3c981fbaa99d29b66dd2c101) )
	ROM_LOAD( "resident_m.005", 0x080000, 0x80000, CRC(0cfe7d44) SHA1(9f0e4925e815ff9f79188f18e78c0a7b377daa3f) )
	ROM_LOAD( "resident_m.006", 0x180000, 0x80000, CRC(7437904f) SHA1(630c79cd6a990ce7658a1ffabba5a27efba985a1) )
	ROM_LOAD( "resident_m.007", 0x280000, 0x80000, CRC(6e94728a) SHA1(ab414879cb957d9bc8d653b5e3bb2bbf91139ec0) )
	ROM_LOAD( "resident_m.008", 0x380000, 0x80000, CRC(a9f55043) SHA1(4771df3d45bdc0a21b1c479f45e09ac5bab6c94f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(74ef5037) SHA1(344775d470e80f4d5580ce22b291c7fde883a716) )
ROM_END


ROM_START( resdnt_8 ) // 100311 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_e_100311.rom", 0x00000, 0x40000, CRC(b55bbd59) SHA1(1c61290b5b0d1174e3f999ea924d3f2ec85bd231) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident_ent.001", 0x000000, 0x80000, CRC(c14a6d1d) SHA1(6bd077db1faf148a7bd480db7a8b23f0deea0e90) )
	ROM_LOAD( "resident_ent.002", 0x100000, 0x80000, CRC(4b76311c) SHA1(6d945787a37ddff9a2157e1ecebffdf254c67f83) )
	ROM_LOAD( "resident_ent.003", 0x200000, 0x80000, CRC(0f920bc6) SHA1(4058396a8ea2413c8e3f430130b5452a7466af45) )
	ROM_LOAD( "resident_ent.004", 0x300000, 0x80000, CRC(2e175050) SHA1(49392578a0a6f472ce41dbf0a08c35b67dd9ca5a) )
	ROM_LOAD( "resident_ent.005", 0x080000, 0x80000, CRC(c9360af3) SHA1(7c9a4ac4137b225e2cbebae7afdc4513c67ff9ec) )
	ROM_LOAD( "resident_ent.006", 0x180000, 0x80000, CRC(69224185) SHA1(2016c976570727658a8dbdb7e8844df384143ca8) )
	ROM_LOAD( "resident_ent.007", 0x280000, 0x80000, CRC(7e2eef27) SHA1(f2acc7fd8e5917523efa7028d60f737cc2330c71) )
	ROM_LOAD( "resident_ent.008", 0x380000, 0x80000, CRC(d4924f74) SHA1(62f13413a8d7bbcfe833a6d7283e1c726ed06a52) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(78cdda87) SHA1(569e8f3c635f4f28ad3402fe390b46e008d118c3) )
ROM_END

ROM_START( resdnt_9 ) // 100316
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rs_100316.rom", 0x00000, 0x40000, CRC(39aaa536) SHA1(b8b7a8f1bf7ad4cbe310448ac64dc4c6be6ac699) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "resident.001",   0x000000, 0x80000, CRC(5a1a24f1) SHA1(da601ed84603b880d2ce4b98784067531d5294b2) )
	ROM_LOAD( "resident.002",   0x100000, 0x80000, CRC(9a8bfa95) SHA1(392690b7031ce2c8ff87843befeefa0e68ae8fce) )
	ROM_LOAD( "resident.003",   0x200000, 0x80000, CRC(0d0db860) SHA1(93bc2b0bb51d754e19f58d8388c837ba2c7ba5ee) )
	ROM_LOAD( "resident.004",   0x300000, 0x80000, CRC(1d3a5b92) SHA1(c8bebca6beb225911c26e30e090d66b724733d59) )
	ROM_LOAD( "resident_m.005", 0x080000, 0x80000, CRC(0cfe7d44) SHA1(9f0e4925e815ff9f79188f18e78c0a7b377daa3f) )
	ROM_LOAD( "resident_m.006", 0x180000, 0x80000, CRC(7437904f) SHA1(630c79cd6a990ce7658a1ffabba5a27efba985a1) )
	ROM_LOAD( "resident_m.007", 0x280000, 0x80000, CRC(6e94728a) SHA1(ab414879cb957d9bc8d653b5e3bb2bbf91139ec0) )
	ROM_LOAD( "resident_m.008", 0x380000, 0x80000, CRC(a9f55043) SHA1(4771df3d45bdc0a21b1c479f45e09ac5bab6c94f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(d2ec780c) SHA1(15f1044821589e2971f4fdd78e281955f4f72b7a) )
ROM_END


/*********************************************************
   Roll Fruit

    Roms 4-8 were changed after the 080327 update.
        The official list of hashes shows both set of roms.

**********************************************************/

ROM_START( rollfr ) // 030821
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rf5-030821.rom", 0x00000, 0x40000, CRC(3afda856) SHA1(5e9c2235ea4207086db23870993d8e28356c9eb8) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "roll_fruit5.001",  0x000000, 0x80000, CRC(caeb1fc3) SHA1(14b9f99f892849faecb3327e572dc134e1065463) )
	ROM_LOAD( "roll_fruit5.002",  0x100000, 0x80000, CRC(f017c200) SHA1(a247bbbd1c4ca99978dcc705bd62590815a891f2) )
	ROM_LOAD( "roll_fruit5.003",  0x200000, 0x80000, CRC(a2d6df11) SHA1(c2553136252aebe3b3ce0b5c33e740d0e27fb7b2) )
	ROM_LOAD( "roll_fruit5.004",  0x300000, 0x80000, CRC(cd3c928a) SHA1(4c50ce17bd5714149eae91279a0133059397b776) )
	ROM_LOAD( "roll_fruit5.005",  0x080000, 0x80000, CRC(24c7362a) SHA1(684b7b370fcad07bf74bddffaf432bd52e5d29e2) )
	ROM_LOAD( "roll_fruit5.006",  0x180000, 0x80000, CRC(d6a61904) SHA1(73700e88358ed9bccbb63643b7daaff416737e43) )
	ROM_LOAD( "roll_fruit5.007",  0x280000, 0x80000, CRC(81e3480b) SHA1(c0f006cf2a4747359cb79f14976ac3411951af1c) )
	ROM_LOAD( "roll_fruit5.008",  0x380000, 0x80000, CRC(ed3558b8) SHA1(8ec808069053f0c07d81c45090b2ba22ef8e9c32) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( rollfr_2 ) // 040318
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rf5-040318.rom", 0x00000, 0x40000, CRC(d8efd395) SHA1(71edd1541df400fef97abacabb10d882ace4c8b0) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "roll_fruit5.001",  0x000000, 0x80000, CRC(caeb1fc3) SHA1(14b9f99f892849faecb3327e572dc134e1065463) )
	ROM_LOAD( "roll_fruit5.002",  0x100000, 0x80000, CRC(f017c200) SHA1(a247bbbd1c4ca99978dcc705bd62590815a891f2) )
	ROM_LOAD( "roll_fruit5.003",  0x200000, 0x80000, CRC(a2d6df11) SHA1(c2553136252aebe3b3ce0b5c33e740d0e27fb7b2) )
	ROM_LOAD( "roll_fruit5.004",  0x300000, 0x80000, CRC(cd3c928a) SHA1(4c50ce17bd5714149eae91279a0133059397b776) )
	ROM_LOAD( "roll_fruit5.005",  0x080000, 0x80000, CRC(24c7362a) SHA1(684b7b370fcad07bf74bddffaf432bd52e5d29e2) )
	ROM_LOAD( "roll_fruit5.006",  0x180000, 0x80000, CRC(d6a61904) SHA1(73700e88358ed9bccbb63643b7daaff416737e43) )
	ROM_LOAD( "roll_fruit5.007",  0x280000, 0x80000, CRC(81e3480b) SHA1(c0f006cf2a4747359cb79f14976ac3411951af1c) )
	ROM_LOAD( "roll_fruit5.008",  0x380000, 0x80000, CRC(ed3558b8) SHA1(8ec808069053f0c07d81c45090b2ba22ef8e9c32) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(2c6058f7) SHA1(4e42000366f2e951a96d1c9f15bafe432fcb6abc) )
ROM_END

ROM_START( rollfr_3 ) // 080327
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rf1-080327.rom", 0x00000, 0x40000, CRC(ea0b402d) SHA1(cf1ae74c90ca1609cf911a2127130a9d56625db0) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "roll_fruit1.001", 0x000000, 0x80000, CRC(caeb1fc3) SHA1(14b9f99f892849faecb3327e572dc134e1065463) )
	ROM_LOAD( "roll_fruit1.002", 0x100000, 0x80000, CRC(f017c200) SHA1(a247bbbd1c4ca99978dcc705bd62590815a891f2) )
	ROM_LOAD( "roll_fruit1.003", 0x200000, 0x80000, CRC(a2d6df11) SHA1(c2553136252aebe3b3ce0b5c33e740d0e27fb7b2) )
	ROM_LOAD( "roll_fruit1.004", 0x300000, 0x80000, CRC(cd3c928a) SHA1(4c50ce17bd5714149eae91279a0133059397b776) )
	ROM_LOAD( "roll_fruit1.005", 0x080000, 0x80000, CRC(1711ab4d) SHA1(902854770bf5ae3de6c9e3100a31399c7ab40cbb) )
	ROM_LOAD( "roll_fruit1.006", 0x180000, 0x80000, CRC(ef2639fb) SHA1(90c67916380f9c49e284aac42f899cae86982829) )
	ROM_LOAD( "roll_fruit1.007", 0x280000, 0x80000, CRC(0305c231) SHA1(f5b25e8716f163eee1f40c737240a8db7059268f) )
	ROM_LOAD( "roll_fruit1.008", 0x380000, 0x80000, CRC(437fe141) SHA1(96783757a0b1af560954f2658b940bec1a4610b2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(8d69b6ab) SHA1(e4080e181acf1e84115efbd1de641256faf71c4d) )
ROM_END

ROM_START( rollfr_4 ) // 080331, bank D0
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rf1-080331.rom", 0x00000, 0x40000, CRC(19b84292) SHA1(f01d907cc8b1716eecb5c28b93ee9712073e1e8d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "roll_fruit1.001", 0x000000, 0x80000, CRC(caeb1fc3) SHA1(14b9f99f892849faecb3327e572dc134e1065463) )
	ROM_LOAD( "roll_fruit1.002", 0x100000, 0x80000, CRC(f017c200) SHA1(a247bbbd1c4ca99978dcc705bd62590815a891f2) )
	ROM_LOAD( "roll_fruit1.003", 0x200000, 0x80000, CRC(a2d6df11) SHA1(c2553136252aebe3b3ce0b5c33e740d0e27fb7b2) )
	ROM_LOAD( "roll_fruit1.004", 0x300000, 0x80000, CRC(cd3c928a) SHA1(4c50ce17bd5714149eae91279a0133059397b776) )
	ROM_LOAD( "roll_fruit1.005", 0x080000, 0x80000, CRC(1711ab4d) SHA1(902854770bf5ae3de6c9e3100a31399c7ab40cbb) )
	ROM_LOAD( "roll_fruit1.006", 0x180000, 0x80000, CRC(ef2639fb) SHA1(90c67916380f9c49e284aac42f899cae86982829) )
	ROM_LOAD( "roll_fruit1.007", 0x280000, 0x80000, CRC(0305c231) SHA1(f5b25e8716f163eee1f40c737240a8db7059268f) )
	ROM_LOAD( "roll_fruit1.008", 0x380000, 0x80000, CRC(437fe141) SHA1(96783757a0b1af560954f2658b940bec1a4610b2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(8d69b6ab) SHA1(e4080e181acf1e84115efbd1de641256faf71c4d) )
ROM_END

ROM_START( rollfr_5 ) // 100924, bank D0
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "rf1-100924.rom",  0x00000, 0x40000, CRC(27b68b41) SHA1(a98ddd9da2ec1d3dd235e8a14efa96fda3d13c05) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "rollfruit1.001", 0x000000, 0x80000, CRC(1e4e6cc7) SHA1(3c301094a0879c42dc93678a5504d6a1a3ca6f37) )
	ROM_LOAD( "rollfruit1.002", 0x100000, 0x80000, CRC(5b744953) SHA1(2d140e3f4022c635c2ca3926b203e66ea618229e) )
	ROM_LOAD( "rollfruit1.003", 0x200000, 0x80000, CRC(f169bbd2) SHA1(35115ae187206d81d121e8fa57ec3edc8581ec2b) )
	ROM_LOAD( "rollfruit1.004", 0x300000, 0x80000, CRC(d877bd31) SHA1(254e6360ec56a99b103289850e585bd5f21621be) )
	ROM_LOAD( "rollfruit1.005", 0x080000, 0x80000, CRC(a9c750ca) SHA1(630db2a329a520eb66e29f4520f6af7e3ac4ae53) )
	ROM_LOAD( "rollfruit1.006", 0x180000, 0x80000, CRC(adfffc87) SHA1(5dc23e8af750639c1615d766dcc7d0133d683118) )
	ROM_LOAD( "rollfruit1.007", 0x280000, 0x80000, CRC(04489de4) SHA1(b73e83475b10651db294788cac8cd5ff636c27f3) )
	ROM_LOAD( "rollfruit1.008", 0x380000, 0x80000, CRC(8462b4ae) SHA1(6abac36f9466ecdf6761600b46bee33ea6e36270) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(8d69b6ab) SHA1(e4080e181acf1e84115efbd1de641256faf71c4d) )
ROM_END


/*********************************************************
   Island
**********************************************************/

ROM_START( island ) // 050713
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is_m_050713.rom", 0x00000, 0x40000, CRC(26c7013e) SHA1(5d604f5b4859e9e82830424a1e21f32a9e49bf34) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island.001", 0x000000, 0x80000, CRC(dbe8cdda) SHA1(4747cf0d85afdef22d3ba9fa5e75b39548725745) )
	ROM_LOAD( "island.002", 0x100000, 0x80000, CRC(64064745) SHA1(91a7bc7204a8f7a7512eeaf4906da20a9f587565) )
	ROM_LOAD( "island.003", 0x200000, 0x80000, CRC(1d993f68) SHA1(b0459d3941d50668f7533909e3f3da91453d3efd) )
	ROM_LOAD( "island.004", 0x300000, 0x80000, CRC(a4739404) SHA1(8f7ffcc13dcb35adfa8060ab1930d07195b6110c) )
	ROM_LOAD( "island.005", 0x080000, 0x80000, CRC(d016eb31) SHA1(a84f18af470f72730b241b9031cd6131c8a03db2) )
	ROM_LOAD( "island.006", 0x180000, 0x80000, CRC(0faaa968) SHA1(0f05546e6e0559e24c6afdde65b3feeb66b6adff) )
	ROM_LOAD( "island.007", 0x280000, 0x80000, CRC(d7277a6c) SHA1(d96a0befc965ad22087381982305d68208978a7e) )
	ROM_LOAD( "island.008", 0x380000, 0x80000, CRC(ac6fba48) SHA1(64dd03d624f16da52bc7fa0702246e91ae39a806) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(30c24324) SHA1(cd531f23ddae833a3a652d8c97d619ba123e8cb7) )
ROM_END

ROM_START( island_2 ) // 070409
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is_070409.rom", 0x00000, 0x40000, CRC(b6790aeb) SHA1(b80d0e4ae003473ac623183439737d6159f59274) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island.001", 0x000000, 0x80000, CRC(dbe8cdda) SHA1(4747cf0d85afdef22d3ba9fa5e75b39548725745) )
	ROM_LOAD( "island.002", 0x100000, 0x80000, CRC(64064745) SHA1(91a7bc7204a8f7a7512eeaf4906da20a9f587565) )
	ROM_LOAD( "island.003", 0x200000, 0x80000, CRC(1d993f68) SHA1(b0459d3941d50668f7533909e3f3da91453d3efd) )
	ROM_LOAD( "island.004", 0x300000, 0x80000, CRC(a4739404) SHA1(8f7ffcc13dcb35adfa8060ab1930d07195b6110c) )
	ROM_LOAD( "island.005", 0x080000, 0x80000, CRC(d016eb31) SHA1(a84f18af470f72730b241b9031cd6131c8a03db2) )
	ROM_LOAD( "island.006", 0x180000, 0x80000, CRC(0faaa968) SHA1(0f05546e6e0559e24c6afdde65b3feeb66b6adff) )
	ROM_LOAD( "island.007", 0x280000, 0x80000, CRC(d7277a6c) SHA1(d96a0befc965ad22087381982305d68208978a7e) )
	ROM_LOAD( "island.008", 0x380000, 0x80000, CRC(ac6fba48) SHA1(64dd03d624f16da52bc7fa0702246e91ae39a806) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(e606d0df) SHA1(25a31534a660abfc344b824a7c45595f4f9cf430) )
ROM_END

ROM_START( island_3 ) // 090806 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is_e_090806.rom", 0x00000, 0x40000, CRC(144565f9) SHA1(8f5d335516a7ec8b7b3f64610258d589e7006d0a) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island_ent.001", 0x000000, 0x80000, CRC(a5ba250e) SHA1(a7f61f770a1edd8802b1eaa1659a3dd857335c63) )
	ROM_LOAD( "island_ent.002", 0x100000, 0x80000, CRC(453e9414) SHA1(fcdaf3895b5482445e9c73f2f27042276cd402ca) )
	ROM_LOAD( "island_ent.003", 0x200000, 0x80000, CRC(29466f14) SHA1(a52d1189ec893bac81824af8e08050ef43a8e8e6) )
	ROM_LOAD( "island_ent.004", 0x300000, 0x80000, CRC(a483d9b0) SHA1(6e1420ceca4975db3609ae8c3947a10a9869364b) )
	ROM_LOAD( "island_ent.005", 0x080000, 0x80000, CRC(3f7ce2c0) SHA1(fc65c5d5f1f765684b118523c0b935b69e63c344) )
	ROM_LOAD( "island_ent.006", 0x180000, 0x80000, CRC(17517ff8) SHA1(0b4945fb4c3c6e168d130629509eb26077a900d0) )
	ROM_LOAD( "island_ent.007", 0x280000, 0x80000, CRC(99d3baac) SHA1(d45c54fa2ef54e0d472b16850fda012a0cdaf586) )
	ROM_LOAD( "island_ent.008", 0x380000, 0x80000, CRC(ceecd2ef) SHA1(a2d810f690258519497303ac9bd37291661706a9) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1be1d844) SHA1(7d02dd6414f08595178941440150c0d2b6ab45ad) )
ROM_END


/*********************************************************
   Island 2
**********************************************************/

ROM_START( island2 ) // 060529
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is2_m_060529.rom", 0x00000, 0x40000, CRC(4ccddabd) SHA1(ae5902734488b7ddfa0f7bbf9b800d25b2b657b5) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island2.001", 0x000000, 0x80000, CRC(f8dd9fe9) SHA1(0cf67fbca107b255011fded6390507d12cbac514) )
	ROM_LOAD( "island2.002", 0x100000, 0x80000, CRC(4f9607c0) SHA1(cd3e7b4a88f46231a115c9d18a26b5e30fea74e4) )
	ROM_LOAD( "island2.003", 0x200000, 0x80000, CRC(bceccdba) SHA1(5cf3b51ccfe317ca57d770bff0204b0ee83d1173) )
	ROM_LOAD( "island2.004", 0x300000, 0x80000, CRC(15fdecc7) SHA1(6882ea10f117c85544df51d9abd67ef52db91d95) )
	ROM_LOAD( "island2.005", 0x080000, 0x80000, CRC(2d4905aa) SHA1(c4a1e4db61e8af6cf0fb70aabe5e3896ab5227ca) )
	ROM_LOAD( "island2.006", 0x180000, 0x80000, CRC(55e285d9) SHA1(ba58963441c65220700cd8057e6afe3f5f8faa4f) )
	ROM_LOAD( "island2.007", 0x280000, 0x80000, CRC(edd72be6) SHA1(fb1e63f59e8565c23ae43630fa572fbc022c878f) )
	ROM_LOAD( "island2.008", 0x380000, 0x80000, CRC(c336d608) SHA1(55391183c6d95ecea81354efa70641350860d1f5) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(79c849de) SHA1(a304dab045021ea7a1b3d6dbd003559e2c8de5d0) )
ROM_END

ROM_START( island2_2 ) // 061214
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is2_m_061214.rom", 0x00000, 0x40000, CRC(b4b6cf53) SHA1(dd416d831c3773f044b355efcb1121f7eb81932b) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island2.001", 0x000000, 0x80000, CRC(f8dd9fe9) SHA1(0cf67fbca107b255011fded6390507d12cbac514) )
	ROM_LOAD( "island2.002", 0x100000, 0x80000, CRC(4f9607c0) SHA1(cd3e7b4a88f46231a115c9d18a26b5e30fea74e4) )
	ROM_LOAD( "island2.003", 0x200000, 0x80000, CRC(bceccdba) SHA1(5cf3b51ccfe317ca57d770bff0204b0ee83d1173) )
	ROM_LOAD( "island2.004", 0x300000, 0x80000, CRC(15fdecc7) SHA1(6882ea10f117c85544df51d9abd67ef52db91d95) )
	ROM_LOAD( "island2.005", 0x080000, 0x80000, CRC(2d4905aa) SHA1(c4a1e4db61e8af6cf0fb70aabe5e3896ab5227ca) )
	ROM_LOAD( "island2.006", 0x180000, 0x80000, CRC(55e285d9) SHA1(ba58963441c65220700cd8057e6afe3f5f8faa4f) )
	ROM_LOAD( "island2.007", 0x280000, 0x80000, CRC(edd72be6) SHA1(fb1e63f59e8565c23ae43630fa572fbc022c878f) )
	ROM_LOAD( "island2.008", 0x380000, 0x80000, CRC(c336d608) SHA1(55391183c6d95ecea81354efa70641350860d1f5) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( island2_3 ) // 061218
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is2_m_061218.rom", 0x00000, 0x40000, CRC(f16cecc5) SHA1(c9caa265295837deb83d1454daf586f3d0be0ee6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island2.001", 0x000000, 0x80000, CRC(f8dd9fe9) SHA1(0cf67fbca107b255011fded6390507d12cbac514) )
	ROM_LOAD( "island2.002", 0x100000, 0x80000, CRC(4f9607c0) SHA1(cd3e7b4a88f46231a115c9d18a26b5e30fea74e4) )
	ROM_LOAD( "island2.003", 0x200000, 0x80000, CRC(bceccdba) SHA1(5cf3b51ccfe317ca57d770bff0204b0ee83d1173) )
	ROM_LOAD( "island2.004", 0x300000, 0x80000, CRC(15fdecc7) SHA1(6882ea10f117c85544df51d9abd67ef52db91d95) )
	ROM_LOAD( "island2.005", 0x080000, 0x80000, CRC(2d4905aa) SHA1(c4a1e4db61e8af6cf0fb70aabe5e3896ab5227ca) )
	ROM_LOAD( "island2.006", 0x180000, 0x80000, CRC(55e285d9) SHA1(ba58963441c65220700cd8057e6afe3f5f8faa4f) )
	ROM_LOAD( "island2.007", 0x280000, 0x80000, CRC(edd72be6) SHA1(fb1e63f59e8565c23ae43630fa572fbc022c878f) )
	ROM_LOAD( "island2.008", 0x380000, 0x80000, CRC(c336d608) SHA1(55391183c6d95ecea81354efa70641350860d1f5) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(35779a11) SHA1(71dbb98eff18c82432b2a606fc3e0bab43e523bf) )
ROM_END

ROM_START( island2_4 ) // 070205
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is2_070205.rom", 0x00000, 0x40000, CRC(bc3e619b) SHA1(4a8e01d60f73e58b2c6cfa739a0a8fce60e565cc) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island2.001", 0x000000, 0x80000, CRC(f8dd9fe9) SHA1(0cf67fbca107b255011fded6390507d12cbac514) )
	ROM_LOAD( "island2.002", 0x100000, 0x80000, CRC(4f9607c0) SHA1(cd3e7b4a88f46231a115c9d18a26b5e30fea74e4) )
	ROM_LOAD( "island2.003", 0x200000, 0x80000, CRC(bceccdba) SHA1(5cf3b51ccfe317ca57d770bff0204b0ee83d1173) )
	ROM_LOAD( "island2.004", 0x300000, 0x80000, CRC(15fdecc7) SHA1(6882ea10f117c85544df51d9abd67ef52db91d95) )
	ROM_LOAD( "island2.005", 0x080000, 0x80000, CRC(2d4905aa) SHA1(c4a1e4db61e8af6cf0fb70aabe5e3896ab5227ca) )
	ROM_LOAD( "island2.006", 0x180000, 0x80000, CRC(55e285d9) SHA1(ba58963441c65220700cd8057e6afe3f5f8faa4f) )
	ROM_LOAD( "island2.007", 0x280000, 0x80000, CRC(edd72be6) SHA1(fb1e63f59e8565c23ae43630fa572fbc022c878f) )
	ROM_LOAD( "island2.008", 0x380000, 0x80000, CRC(c336d608) SHA1(55391183c6d95ecea81354efa70641350860d1f5) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(6408870a) SHA1(a97a5374f8848c46264d76f62de9695c96559a5d) )
ROM_END

ROM_START( island2_5 ) // 090528 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is2_l_090528.rom", 0x00000, 0x40000, CRC(47490834) SHA1(82299ad59e5df3d681c46286681112b890745579) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island2_loto.001", 0x000000, 0x80000, CRC(413e9a9d) SHA1(4ee525ff21dd7d87c37dcce9c226d1d0a749559d) )
	ROM_LOAD( "island2_loto.002", 0x100000, 0x80000, CRC(af825eb6) SHA1(69772255593852b29e6b9e3065eb455578daf125) )
	ROM_LOAD( "island2_loto.003", 0x200000, 0x80000, CRC(dede5375) SHA1(7c793b528a5b70def168a669ce838cc34a8d27e0) )
	ROM_LOAD( "island2_loto.004", 0x300000, 0x80000, CRC(f12bef1a) SHA1(b87e3611258014c1ee83f0735c5242f431cdaca0) )
	ROM_LOAD( "island2_loto.005", 0x080000, 0x80000, CRC(f458c588) SHA1(e7f4be7ddbb925bb8015eaab86733a33a13996f4) )
	ROM_LOAD( "island2_loto.006", 0x180000, 0x80000, CRC(6275c382) SHA1(3e09f597edc7c2789dd8235f1b6c17bb92e8b0d3) )
	ROM_LOAD( "island2_loto.007", 0x280000, 0x80000, CRC(1e20305d) SHA1(4f2386251dbae869a2862f884c509f854c4fb283) )
	ROM_LOAD( "island2_loto.008", 0x380000, 0x80000, CRC(e4af286f) SHA1(439a151a26b8a1e5abe08f3bf6601107e516ee68) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(4a41b64c) SHA1(dc3b22268c0385ce8b4e7dda578dd25920c23e02) )
ROM_END

ROM_START( island2_6 ) // 090724 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "is2e_090724.rom", 0x00000, 0x40000, CRC(4fabced1) SHA1(7617337238f6bf114189aae13a2b248634c7446a) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "island2_ent.001", 0x000000, 0x80000, CRC(e0b9a33b) SHA1(e4ee665a3e6bcce868605a2485817b502ce041ce) )
	ROM_LOAD( "island2_ent.002", 0x100000, 0x80000, CRC(f349bbe6) SHA1(b15782a438f34b06a2e785948cfa39d87ae07112) )
	ROM_LOAD( "island2_ent.003", 0x200000, 0x80000, CRC(68ba6e71) SHA1(d394f7043b8f92d61a6c17ae3ace641729a146df) )
	ROM_LOAD( "island2_ent.004", 0x300000, 0x80000, CRC(98b446fb) SHA1(728e8eed785bb7c08bc62589993c26becc79b460) )
	ROM_LOAD( "island2_ent.005", 0x080000, 0x80000, CRC(e8166a42) SHA1(baca76902a48a221ed342d25e430a6223d9e64fe) )
	ROM_LOAD( "island2_ent.006", 0x180000, 0x80000, CRC(0e398ed6) SHA1(386bad1ba0cf33e150088dc99fad55ca3cf45cdb) )
	ROM_LOAD( "island2_ent.007", 0x280000, 0x80000, CRC(b8af96a0) SHA1(5323d696609a4496e74083b73bdd132c13ab73b8) )
	ROM_LOAD( "island2_ent.008", 0x380000, 0x80000, CRC(5e8cbbd8) SHA1(e50ef2e12ac52c007088e1e07ad41efec6c5d0ac) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(e28280b6) SHA1(96ccd48e167d02b73b7fd8ed9602521d73b14059) )
ROM_END


/*********************************************************
   Pirate
**********************************************************/

ROM_START( pirate ) // 051229
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr_m_051229.rom", 0x00000, 0x40000, CRC(c4e90e7b) SHA1(d1286cba474ccbbff8358ba2fd6917d43d101674) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate.001", 0x000000, 0x80000, CRC(d2199619) SHA1(8c67ef7d0e305ae0783302ba9a1cb56cdcf4bc09) )
	ROM_LOAD( "pirate.002", 0x100000, 0x80000, CRC(ce5c6548) SHA1(ef1cd6ae36cc1abcf010762dc89a255cd817d016) )
	ROM_LOAD( "pirate.003", 0x200000, 0x80000, CRC(d6a8338d) SHA1(6a0e41309dc909decf8bd49cf13cbeca95f0314a) )
	ROM_LOAD( "pirate.004", 0x300000, 0x80000, CRC(590b8cf6) SHA1(b2778f6e1b7bcf7f33ced43f999eff983e5a6af4) )
	ROM_LOAD( "pirate.005", 0x080000, 0x80000, CRC(bf9f1267) SHA1(b0947bd7d31301ffbe80cbaf1e96c3476f6f9ca3) )
	ROM_LOAD( "pirate.006", 0x180000, 0x80000, CRC(b0cdf7eb) SHA1(cf6bd20fb40cf0d87eeb6f1502fb73d9760c9140) )
	ROM_LOAD( "pirate.007", 0x280000, 0x80000, CRC(6c4a9510) SHA1(e10bf8475ff7c73ba90b904b9214b285a5b2669f) )
	ROM_LOAD( "pirate.008", 0x380000, 0x80000, CRC(cc2edac2) SHA1(24bacd9e092a83945a8def3a254ec66758d71ff5) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( pirate_2 ) // 060210
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr_m_060210.rom", 0x00000, 0x40000, CRC(5684d67d) SHA1(4cbd103bcd071df26830d56760ef477b9a652857) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate.001", 0x000000, 0x80000, CRC(d2199619) SHA1(8c67ef7d0e305ae0783302ba9a1cb56cdcf4bc09) )
	ROM_LOAD( "pirate.002", 0x100000, 0x80000, CRC(ce5c6548) SHA1(ef1cd6ae36cc1abcf010762dc89a255cd817d016) )
	ROM_LOAD( "pirate.003", 0x200000, 0x80000, CRC(d6a8338d) SHA1(6a0e41309dc909decf8bd49cf13cbeca95f0314a) )
	ROM_LOAD( "pirate.004", 0x300000, 0x80000, CRC(590b8cf6) SHA1(b2778f6e1b7bcf7f33ced43f999eff983e5a6af4) )
	ROM_LOAD( "pirate.005", 0x080000, 0x80000, CRC(bf9f1267) SHA1(b0947bd7d31301ffbe80cbaf1e96c3476f6f9ca3) )
	ROM_LOAD( "pirate.006", 0x180000, 0x80000, CRC(b0cdf7eb) SHA1(cf6bd20fb40cf0d87eeb6f1502fb73d9760c9140) )
	ROM_LOAD( "pirate.007", 0x280000, 0x80000, CRC(6c4a9510) SHA1(e10bf8475ff7c73ba90b904b9214b285a5b2669f) )
	ROM_LOAD( "pirate.008", 0x380000, 0x80000, CRC(cc2edac2) SHA1(24bacd9e092a83945a8def3a254ec66758d71ff5) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(0f61c9eb) SHA1(c50833d8d0284fb3c909cfedf731dcb14a085ada) )
ROM_END

ROM_START( pirate_3 ) // 060803
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr_m_060803.rom", 0x00000, 0x40000, CRC(1de68707) SHA1(99dd3c5186ed8ba6c17e9f5479df93173da527e0) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate.001", 0x000000, 0x80000, CRC(d2199619) SHA1(8c67ef7d0e305ae0783302ba9a1cb56cdcf4bc09) )
	ROM_LOAD( "pirate.002", 0x100000, 0x80000, CRC(ce5c6548) SHA1(ef1cd6ae36cc1abcf010762dc89a255cd817d016) )
	ROM_LOAD( "pirate.003", 0x200000, 0x80000, CRC(d6a8338d) SHA1(6a0e41309dc909decf8bd49cf13cbeca95f0314a) )
	ROM_LOAD( "pirate.004", 0x300000, 0x80000, CRC(590b8cf6) SHA1(b2778f6e1b7bcf7f33ced43f999eff983e5a6af4) )
	ROM_LOAD( "pirate.005", 0x080000, 0x80000, CRC(bf9f1267) SHA1(b0947bd7d31301ffbe80cbaf1e96c3476f6f9ca3) )
	ROM_LOAD( "pirate.006", 0x180000, 0x80000, CRC(b0cdf7eb) SHA1(cf6bd20fb40cf0d87eeb6f1502fb73d9760c9140) )
	ROM_LOAD( "pirate.007", 0x280000, 0x80000, CRC(6c4a9510) SHA1(e10bf8475ff7c73ba90b904b9214b285a5b2669f) )
	ROM_LOAD( "pirate.008", 0x380000, 0x80000, CRC(cc2edac2) SHA1(24bacd9e092a83945a8def3a254ec66758d71ff5) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(0f61c9eb) SHA1(c50833d8d0284fb3c909cfedf731dcb14a085ada) )
ROM_END

ROM_START( pirate_4 ) // 070412
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr_070412.rom", 0x00000, 0x40000, CRC(31909fb2) SHA1(f9ebad0de53645f7ed854147d8227a3b5b9224f4) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate.001", 0x000000, 0x80000, CRC(d2199619) SHA1(8c67ef7d0e305ae0783302ba9a1cb56cdcf4bc09) )
	ROM_LOAD( "pirate.002", 0x100000, 0x80000, CRC(ce5c6548) SHA1(ef1cd6ae36cc1abcf010762dc89a255cd817d016) )
	ROM_LOAD( "pirate.003", 0x200000, 0x80000, CRC(d6a8338d) SHA1(6a0e41309dc909decf8bd49cf13cbeca95f0314a) )
	ROM_LOAD( "pirate.004", 0x300000, 0x80000, CRC(590b8cf6) SHA1(b2778f6e1b7bcf7f33ced43f999eff983e5a6af4) )
	ROM_LOAD( "pirate.005", 0x080000, 0x80000, CRC(bf9f1267) SHA1(b0947bd7d31301ffbe80cbaf1e96c3476f6f9ca3) )
	ROM_LOAD( "pirate.006", 0x180000, 0x80000, CRC(b0cdf7eb) SHA1(cf6bd20fb40cf0d87eeb6f1502fb73d9760c9140) )
	ROM_LOAD( "pirate.007", 0x280000, 0x80000, CRC(6c4a9510) SHA1(e10bf8475ff7c73ba90b904b9214b285a5b2669f) )
	ROM_LOAD( "pirate.008", 0x380000, 0x80000, CRC(cc2edac2) SHA1(24bacd9e092a83945a8def3a254ec66758d71ff5) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(85054f45) SHA1(4d9e86a23a5a521083b6faa3b1b3fd8e7b8c85b0) )
ROM_END

ROM_START( pirate_5 ) // 090803 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr_e_090803.rom", 0x00000, 0x40000, CRC(72a1260a) SHA1(84678a9a90ca795e4938d82945ce3c4d15c49ed1) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate_ent.001", 0x000000, 0x80000, CRC(9b93cd27) SHA1(81cce41463322aa90866f5ac56b82fb73e191319) )
	ROM_LOAD( "pirate_ent.002", 0x100000, 0x80000, CRC(5776a0dc) SHA1(2d81b7333ee418329ec0f0bb96b30dfaaf32cbf9) )
	ROM_LOAD( "pirate_ent.003", 0x200000, 0x80000, CRC(bf91b15f) SHA1(933cde175e343c6e5511b023096aec43d49b8963) )
	ROM_LOAD( "pirate_ent.004", 0x300000, 0x80000, CRC(5ad7299d) SHA1(d1246cd2b87a0b0b9e81083f1724d17fb69f64e5) )
	ROM_LOAD( "pirate_ent.005", 0x080000, 0x80000, CRC(7af72c13) SHA1(c264c8a6eb8e4a6b22e9be7bc8bde05901d91c56) )
	ROM_LOAD( "pirate_ent.006", 0x180000, 0x80000, CRC(9ea25f75) SHA1(0c8715b3f3fc5b875f3fbd80c79ae55ca9a32cb1) )
	ROM_LOAD( "pirate_ent.007", 0x280000, 0x80000, CRC(dffa307e) SHA1(da231a501fe2cc2149fff9cc3370aa1766292091) )
	ROM_LOAD( "pirate_ent.008", 0x380000, 0x80000, CRC(c9504ac4) SHA1(c8dc072b75348da02f5d811a580ba103728c2dd4) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(348f7ba9) SHA1(0f48481bf1c790760c7ee27f0caed45a833d3365) )
ROM_END


/*********************************************************
   Pirate 2
**********************************************************/

ROM_START( pirate2 ) // 061005
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr2_m_061005.rom", 0x00000, 0x40000, CRC(4ad0a29a) SHA1(72950da5f201a393a4761a8696cfc210725df23f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate2.001", 0x000000, 0x80000, CRC(106e7cba) SHA1(289a3ae38b895c83600c920bee0c2dd46e941eac) )
	ROM_LOAD( "pirate2.002", 0x100000, 0x80000, CRC(076a290f) SHA1(2f9bb74e081262e535c8ed9a31589d6a919f5d15) )
	ROM_LOAD( "pirate2.003", 0x200000, 0x80000, CRC(13a91fe7) SHA1(6e127b3827a9271ad19986714747be9367125f62) )
	ROM_LOAD( "pirate2.004", 0x300000, 0x80000, CRC(5ac8c531) SHA1(1da91b9a71a9a8681577342660bfa85e5bbc99bc) )
	ROM_LOAD( "pirate2.005", 0x080000, 0x80000, CRC(98012c74) SHA1(2a5b466353eef3a5cfc9f98eceb7523b00d0204a) )
	ROM_LOAD( "pirate2.006", 0x180000, 0x80000, CRC(366e1465) SHA1(440230d5306c4b424f27839b7fb9c8a5bb922dcc) )
	ROM_LOAD( "pirate2.007", 0x280000, 0x80000, CRC(21fb963e) SHA1(e3f7fb13f326699e34aebcc3ee07016f7cfe6e46) )
	ROM_LOAD( "pirate2.008", 0x380000, 0x80000, CRC(40c59448) SHA1(774af0f376864ec5948904df338bc7493eaed392) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1a0a98a5) SHA1(2d7d2d6bb02317de1f5225189a156d2ec5ade23a) )
ROM_END

ROM_START( pirate2_2 ) // 070126
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr2_070126.rom", 0x00000, 0x40000, CRC(5abf1580) SHA1(0f5c2ed4f52070dcbeb3adf0d209088e2822696d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate2.001", 0x000000, 0x80000, CRC(106e7cba) SHA1(289a3ae38b895c83600c920bee0c2dd46e941eac) )
	ROM_LOAD( "pirate2.002", 0x100000, 0x80000, CRC(076a290f) SHA1(2f9bb74e081262e535c8ed9a31589d6a919f5d15) )
	ROM_LOAD( "pirate2.003", 0x200000, 0x80000, CRC(13a91fe7) SHA1(6e127b3827a9271ad19986714747be9367125f62) )
	ROM_LOAD( "pirate2.004", 0x300000, 0x80000, CRC(5ac8c531) SHA1(1da91b9a71a9a8681577342660bfa85e5bbc99bc) )
	ROM_LOAD( "pirate2.005", 0x080000, 0x80000, CRC(98012c74) SHA1(2a5b466353eef3a5cfc9f98eceb7523b00d0204a) )
	ROM_LOAD( "pirate2.006", 0x180000, 0x80000, CRC(366e1465) SHA1(440230d5306c4b424f27839b7fb9c8a5bb922dcc) )
	ROM_LOAD( "pirate2.007", 0x280000, 0x80000, CRC(21fb963e) SHA1(e3f7fb13f326699e34aebcc3ee07016f7cfe6e46) )
	ROM_LOAD( "pirate2.008", 0x380000, 0x80000, CRC(40c59448) SHA1(774af0f376864ec5948904df338bc7493eaed392) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(ee15ee8a) SHA1(aa7d4e302e14abc8a7bd1b4dc2cfb7b21c5a2061) )
ROM_END

ROM_START( pirate2_3 ) // 090528 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr2_l_090528.rom", 0x00000, 0x40000, CRC(8c7195e9) SHA1(5a1210d66dcdeaddee5292b0ecdb37f00d56acb0) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate2_loto.001", 0x000000, 0x80000, CRC(d59b5dc4) SHA1(3a9473be9b867b66960e9002f7f0834570954c17) )
	ROM_LOAD( "pirate2_loto.002", 0x100000, 0x80000, CRC(8f14b97d) SHA1(f92a777b908537e347bbe17c8dbfd1e44dbee11b) )
	ROM_LOAD( "pirate2_loto.003", 0x200000, 0x80000, CRC(a55f59f7) SHA1(c270f9d28d6a18bfccd0e550220707b5b2a6c2a6) )
	ROM_LOAD( "pirate2_loto.004", 0x300000, 0x80000, CRC(b51ab6e7) SHA1(c0c3aec8ae2b5d8b6734158bc717994d86806a80) )
	ROM_LOAD( "pirate2_loto.005", 0x080000, 0x80000, CRC(b9343b01) SHA1(d84852f0181f3a521df05ed2a79a8f78f8021023) )
	ROM_LOAD( "pirate2_loto.006", 0x180000, 0x80000, CRC(8032a162) SHA1(31f87cb5d24e1776cb70a3163612427ef783fc78) )
	ROM_LOAD( "pirate2_loto.007", 0x280000, 0x80000, CRC(e238a3cc) SHA1(9c7695d16e2b3a1ab721d0a6bd78203494a47a67) )
	ROM_LOAD( "pirate2_loto.008", 0x380000, 0x80000, CRC(3ae18117) SHA1(2b71e09a39f00e8a665e1ac2205b79b0e476f0b2) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(bd42032b) SHA1(ec7d11f7954e5a69c691bd6d2f53bf63c4eabfb5) )
ROM_END

ROM_START( pirate2_4 ) // 090730 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "pr2e_090730.rom", 0x00000, 0x40000, CRC(debc96ea) SHA1(defd83d2bd44c7a1de893180f4dccad12f7410e2) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "pirate2_ent.001", 0x000000, 0x80000, CRC(e4e634c2) SHA1(710fdf76d186dfd9bb6c87ff56322245a50d00a3) )
	ROM_LOAD( "pirate2_ent.002", 0x100000, 0x80000, CRC(22b2939d) SHA1(dfec07586cfc3a7fe2eb65bb46064d9ca590c2ec) )
	ROM_LOAD( "pirate2_ent.003", 0x200000, 0x80000, CRC(eb8bdb4b) SHA1(3b4a4d95e141c9846465fc934432541e750c0f69) )
	ROM_LOAD( "pirate2_ent.004", 0x300000, 0x80000, CRC(43a4fe47) SHA1(08d397ea06706a1be0dd1b87430638a67356b77a) )
	ROM_LOAD( "pirate2_ent.005", 0x080000, 0x80000, CRC(62001d8e) SHA1(f986aa76ef92edc33b1ce6e4fa0644759d68cb96) )
	ROM_LOAD( "pirate2_ent.006", 0x180000, 0x80000, CRC(56f4fad5) SHA1(f9d13d799637041bd1d29ed0828e5ef232af4f1c) )
	ROM_LOAD( "pirate2_ent.007", 0x280000, 0x80000, CRC(87440119) SHA1(d6abbe144938acbefbabbf1b501dec784e9f98db) )
	ROM_LOAD( "pirate2_ent.008", 0x380000, 0x80000, CRC(eddcbdbd) SHA1(5b4eccf7a821d98a27f195705c3a71d59cee008d) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(84fe498b) SHA1(4d1ff3c130a0d6105a0b64c6a115c45188c1c155) )
ROM_END


/*********************************************************
   Keks

    "Russia" sets use different gfx roms 1-4.
        The official list of hashes shows only updated roms.

**********************************************************/

ROM_START( keks ) // 060328
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_m_060328.rom", 0x00000, 0x40000, CRC(bcf77f77) SHA1(26b09994907c41be957a0b7442cfb1807b27d7be) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks_m.001", 0x000000, 0x80000, CRC(f4c20f66) SHA1(bed42ef01dfaa9d5d6ebb703e44ce7c11b8a373c) )
	ROM_LOAD( "keks_m.002", 0x100000, 0x80000, CRC(b7ec3fac) SHA1(c3c62690487a6056415c46888bde8254efca836f) )
	ROM_LOAD( "keks_m.003", 0x200000, 0x80000, CRC(5b6e8568) SHA1(003297e9cd080d91fe6751286eabd3a2f37ceb76) )
	ROM_LOAD( "keks_m.004", 0x300000, 0x80000, CRC(9dc32736) SHA1(7b2091ae802431d1c959b859a58e0076d32abef0) )
	ROM_LOAD( "keks_m.005", 0x080000, 0x80000, CRC(c5b09267) SHA1(7fd0988e63752fdbb31fde60b4726cfd63149622) )
	ROM_LOAD( "keks_m.006", 0x180000, 0x80000, CRC(583da5fd) SHA1(645228db20cdaacb53bfc68731fd1a66a6a8cf56) )
	ROM_LOAD( "keks_m.007", 0x280000, 0x80000, CRC(311c166a) SHA1(5f0ad8d755a6141964d818b98b3f156cbda8fb0d) )
	ROM_LOAD( "keks_m.008", 0x380000, 0x80000, CRC(f69b0831) SHA1(75392349ef02a39cf883206938e2c615445065fc) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(99307739) SHA1(cfcf9cd9426a1a977c29f8825288064a561df28d) )
ROM_END


ROM_START( keks_2 ) // 060403
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_m_060403.rom", 0x00000, 0x40000, CRC(7abb9392) SHA1(f7a0ba5bcc7566f706e911486fa9cf3e62b86b8b) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks_m.001", 0x000000, 0x80000, CRC(f4c20f66) SHA1(bed42ef01dfaa9d5d6ebb703e44ce7c11b8a373c) )
	ROM_LOAD( "keks_m.002", 0x100000, 0x80000, CRC(b7ec3fac) SHA1(c3c62690487a6056415c46888bde8254efca836f) )
	ROM_LOAD( "keks_m.003", 0x200000, 0x80000, CRC(5b6e8568) SHA1(003297e9cd080d91fe6751286eabd3a2f37ceb76) )
	ROM_LOAD( "keks_m.004", 0x300000, 0x80000, CRC(9dc32736) SHA1(7b2091ae802431d1c959b859a58e0076d32abef0) )
	ROM_LOAD( "keks_m.005", 0x080000, 0x80000, CRC(c5b09267) SHA1(7fd0988e63752fdbb31fde60b4726cfd63149622) )
	ROM_LOAD( "keks_m.006", 0x180000, 0x80000, CRC(583da5fd) SHA1(645228db20cdaacb53bfc68731fd1a66a6a8cf56) )
	ROM_LOAD( "keks_m.007", 0x280000, 0x80000, CRC(311c166a) SHA1(5f0ad8d755a6141964d818b98b3f156cbda8fb0d) )
	ROM_LOAD( "keks_m.008", 0x380000, 0x80000, CRC(f69b0831) SHA1(75392349ef02a39cf883206938e2c615445065fc) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(99307739) SHA1(cfcf9cd9426a1a977c29f8825288064a561df28d) )
ROM_END

ROM_START( keks_3 ) // 070119
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_070119.rom", 0x00000, 0x40000, CRC(1cf8fdaa) SHA1(a7f1242d19c5fb5a6fccacc06dd9d27f3352fe24) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks.001",   0x000000, 0x80000, CRC(fc399595) SHA1(037afd4a613cd58d4a28627b9e395d48c3fa866a) )
	ROM_LOAD( "keks.002",   0x100000, 0x80000, CRC(474b36e7) SHA1(e1e62acd4a706b2654fc1249850806b612fc1419) )
	ROM_LOAD( "keks.003",   0x200000, 0x80000, CRC(7f885e3d) SHA1(09bb4690e86ed4a29eef75ee4e5753ce40a164dd) )
	ROM_LOAD( "keks.004",   0x300000, 0x80000, CRC(a0fc654b) SHA1(3354bdb7aa372816a766b0d36408543de7d3482f) )
	ROM_LOAD( "keks_m.005", 0x080000, 0x80000, CRC(c5b09267) SHA1(7fd0988e63752fdbb31fde60b4726cfd63149622) )
	ROM_LOAD( "keks_m.006", 0x180000, 0x80000, CRC(583da5fd) SHA1(645228db20cdaacb53bfc68731fd1a66a6a8cf56) )
	ROM_LOAD( "keks_m.007", 0x280000, 0x80000, CRC(311c166a) SHA1(5f0ad8d755a6141964d818b98b3f156cbda8fb0d) )
	ROM_LOAD( "keks_m.008", 0x380000, 0x80000, CRC(f69b0831) SHA1(75392349ef02a39cf883206938e2c615445065fc) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1464bf6c) SHA1(5c1a984825dbd129e8295c3c6de6f82bfe5dc99e) )
ROM_END

ROM_START( keks_4 ) // 090604 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_l_090604.rom", 0x00000, 0x40000, CRC(5ab26391) SHA1(7a33707542368fc26df421c2dcdd874009d333da) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks_loto.001", 0x000000, 0x80000, CRC(cf43d372) SHA1(bb17fd16aa0afe2f1477c242d945cefd58d78dc6) )
	ROM_LOAD( "keks_loto.002", 0x100000, 0x80000, CRC(5696bb7c) SHA1(c35cf77f7f58a62801e5a08a8034e93bfc207c01) )
	ROM_LOAD( "keks_loto.003", 0x200000, 0x80000, CRC(2769b92b) SHA1(9bc5c062f3f1336d4419a9133535f7b0599be7b8) )
	ROM_LOAD( "keks_loto.004", 0x300000, 0x80000, CRC(c82ce6a4) SHA1(76edc1582e474df8746e91e53e0575f696b3842f) )
	ROM_LOAD( "keks_loto.005", 0x080000, 0x80000, CRC(666ebc1a) SHA1(4263da25b7394fe5557e3b1ca008896161c2ea2d) )
	ROM_LOAD( "keks_loto.006", 0x180000, 0x80000, CRC(a559c07a) SHA1(37ea1bd2b4e2097a6f13b70aa94bf993fd629a94) )
	ROM_LOAD( "keks_loto.007", 0x280000, 0x80000, CRC(99f3d881) SHA1(e355ebe4a1c61cb18d3794766b2a0198310a14be) )
	ROM_LOAD( "keks_loto.008", 0x380000, 0x80000, CRC(7eaf1418) SHA1(420a39ba1f592a91b962101a1ac8fdaf8b3a81ab) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(7a8c3e05) SHA1(87d8fcb67ca0ca72f78a50f9a32ba4119fe80aca) )
ROM_END

ROM_START( keks_5 ) // 090727 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_e_090727.rom", 0x00000, 0x40000, CRC(378d94b0) SHA1(17865dbdaf31005b5c582af019ae508392a31eee) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks_ent.001", 0x000000, 0x80000, CRC(69a4dd8f) SHA1(9e5078bdf5c5c8188ddb11ede7b567eaf04fa6c4) )
	ROM_LOAD( "keks_ent.002", 0x100000, 0x80000, CRC(fecb6769) SHA1(215799c496ebe63702b00c26deb58f0a4f020ebe) )
	ROM_LOAD( "keks_ent.003", 0x200000, 0x80000, CRC(3608c175) SHA1(922e7209a6aab75c1a96fe4dc200358bd128a263) )
	ROM_LOAD( "keks_ent.004", 0x300000, 0x80000, CRC(3f091d42) SHA1(f8d21e29ffa7048e4fa5fb01f4aed6d29df5181e) )
	ROM_LOAD( "keks_ent.005", 0x080000, 0x80000, CRC(e027f675) SHA1(a92c6eb2563c136d055f1d92d2edbfa242aa75c2) )
	ROM_LOAD( "keks_ent.006", 0x180000, 0x80000, CRC(946dc35b) SHA1(6ed88ac7142c1a0b962ba0b56756f4d68bbcf27a) )
	ROM_LOAD( "keks_ent.007", 0x280000, 0x80000, CRC(9ed6bf1a) SHA1(e712a38b0e3718972e65d7dd73f505205ef4bd45) )
	ROM_LOAD( "keks_ent.008", 0x380000, 0x80000, CRC(f5363166) SHA1(e743e8f7e32202c998dfcb20fef6003d61cae64a) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(480de536) SHA1(bddc8269222ff3837c2924caf8ffac803e4aed18) )
ROM_END

ROM_START( keks_6 ) // 110816
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_m_110816.rom", 0x00000, 0x40000, CRC(9e244f5d) SHA1(49f8a2cdbef309b26bcc95bfc78e5b4a83c30583) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks_m.001", 0x000000, 0x80000, CRC(f4c20f66) SHA1(bed42ef01dfaa9d5d6ebb703e44ce7c11b8a373c) )
	ROM_LOAD( "keks_m.002", 0x100000, 0x80000, CRC(b7ec3fac) SHA1(c3c62690487a6056415c46888bde8254efca836f) )
	ROM_LOAD( "keks_m.003", 0x200000, 0x80000, CRC(5b6e8568) SHA1(003297e9cd080d91fe6751286eabd3a2f37ceb76) )
	ROM_LOAD( "keks_m.004", 0x300000, 0x80000, CRC(9dc32736) SHA1(7b2091ae802431d1c959b859a58e0076d32abef0) )
	ROM_LOAD( "keks_m.005", 0x080000, 0x80000, CRC(c5b09267) SHA1(7fd0988e63752fdbb31fde60b4726cfd63149622) )
	ROM_LOAD( "keks_m.006", 0x180000, 0x80000, CRC(583da5fd) SHA1(645228db20cdaacb53bfc68731fd1a66a6a8cf56) )
	ROM_LOAD( "keks_m.007", 0x280000, 0x80000, CRC(311c166a) SHA1(5f0ad8d755a6141964d818b98b3f156cbda8fb0d) )
	ROM_LOAD( "keks_m.008", 0x380000, 0x80000, CRC(f69b0831) SHA1(75392349ef02a39cf883206938e2c615445065fc) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(99307739) SHA1(cfcf9cd9426a1a977c29f8825288064a561df28d) )
ROM_END

ROM_START( keks_7 ) // 110816
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "ks_110816.rom", 0x00000, 0x40000, CRC(be6b2642) SHA1(47ea80529da40d3260a84fa322dbaf5fb581dfbe) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "keks.001",   0x000000, 0x80000, CRC(fc399595) SHA1(037afd4a613cd58d4a28627b9e395d48c3fa866a) )
	ROM_LOAD( "keks.002",   0x100000, 0x80000, CRC(474b36e7) SHA1(e1e62acd4a706b2654fc1249850806b612fc1419) )
	ROM_LOAD( "keks.003",   0x200000, 0x80000, CRC(7f885e3d) SHA1(09bb4690e86ed4a29eef75ee4e5753ce40a164dd) )
	ROM_LOAD( "keks.004",   0x300000, 0x80000, CRC(a0fc654b) SHA1(3354bdb7aa372816a766b0d36408543de7d3482f) )
	ROM_LOAD( "keks_m.005", 0x080000, 0x80000, CRC(c5b09267) SHA1(7fd0988e63752fdbb31fde60b4726cfd63149622) )
	ROM_LOAD( "keks_m.006", 0x180000, 0x80000, CRC(583da5fd) SHA1(645228db20cdaacb53bfc68731fd1a66a6a8cf56) )
	ROM_LOAD( "keks_m.007", 0x280000, 0x80000, CRC(311c166a) SHA1(5f0ad8d755a6141964d818b98b3f156cbda8fb0d) )
	ROM_LOAD( "keks_m.008", 0x380000, 0x80000, CRC(f69b0831) SHA1(75392349ef02a39cf883206938e2c615445065fc) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(1464bf6c) SHA1(5c1a984825dbd129e8295c3c6de6f82bfe5dc99e) )
ROM_END


/*********************************************************
   Gnome
**********************************************************/

ROM_START( gnome ) // 070906
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_070906.rom", 0x00000, 0x40000, CRC(8fe48f72) SHA1(cc3c74be120359ecfc42a5e96eff95c2da1e8f4c) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(52b7a804) SHA1(d620185adb2160f455f94edc08fd2b9f4a3ea9c2) )
ROM_END

ROM_START( gnome_2 ) // 071115
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_071115.rom", 0x00000, 0x40000, CRC(2fb768b8) SHA1(b0aadf057bd8a5b6cdcef5baed190c3d222256ca) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(e58acd1d) SHA1(3b5c19f06e28cde38cf25627ffd488db327eb293) )
ROM_END

ROM_START( gnome_3 ) // 080303
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_m_080303.rom", 0x00000, 0x40000, CRC(d0aec288) SHA1(5870fd0fe27b049b7945369b23b0dd00e0ccf3e1) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(9218c36a) SHA1(ff96c6b258a5aba9563903b8791d11c50da7261e) )
ROM_END

ROM_START( gnome_4 ) // 090402
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_090402.rom", 0x00000, 0x40000, CRC(645f4643) SHA1(6d9cdcb98bcb9a664c7c3e4197a093edfda6a9b8) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(77bf0ebd) SHA1(b3af0e8d2839de95afcaf01fe78a3b2a2516ca14) )
ROM_END

ROM_START( gnome_5 ) // 090406
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_m_090406.rom", 0x00000, 0x40000, CRC(e8a03650) SHA1(e4564e59c6c6836cd4013073549a019e8028ea0d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(155dadcd) SHA1(f6447584d113e32a9b5332ded2b4581fae87b9d2) )
ROM_END


ROM_START( gnome_7 ) // 090708 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_l_090708.rom", 0x00000, 0x40000, CRC(ac212d25) SHA1(c45397204467f5cf8a56ffe0c84f30f388a51193) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome_loto.001", 0x000000, 0x80000, CRC(15f75190) SHA1(85587a008889b5e34f5f79ceb1abfcd9a6c53cec) )
	ROM_LOAD( "gnome_loto.002", 0x100000, 0x80000, CRC(26f9af6a) SHA1(131b26e035b4cfd9d36ab8a7f2957e77170a529d) )
	ROM_LOAD( "gnome_loto.003", 0x200000, 0x80000, CRC(7d388bd5) SHA1(2f2eadc44f35033d61dbab390a4dbfec23f31c85) )
	ROM_LOAD( "gnome_loto.004", 0x300000, 0x80000, CRC(7bad4ac5) SHA1(2cfac6462b666b4bb0d546932b6784a80cf8d0d4) )
	ROM_LOAD( "gnome_loto.005", 0x080000, 0x80000, CRC(f86a7d02) SHA1(1e7da8ac89eb8b1d2c293d2cfead7a52524fc674) )
	ROM_LOAD( "gnome_loto.006", 0x180000, 0x80000, CRC(d66f1ab8) SHA1(27b612ab42008f8673a0508a1b813c63a0e2ba4c) )
	ROM_LOAD( "gnome_loto.007", 0x280000, 0x80000, CRC(99ae985c) SHA1(f0fe5a0dbc289a93246a825f32a726cf62ccb9aa) )
	ROM_LOAD( "gnome_loto.008", 0x380000, 0x80000, CRC(4dc3f777) SHA1(3352170877c59daff63c056dfca00915f87b5795) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(6bc033cd) SHA1(6e676a7e8695708567f03759fe55f24ffb749619) )
ROM_END

ROM_START( gnome_8 ) // 090810 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_e_090810.rom", 0x00000, 0x40000, CRC(614102f1) SHA1(99ae99e2d56f016e7376f75c6eddcf6150015205) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome_ent.001", 0x000000, 0x80000, CRC(84c84c44) SHA1(09173c35667f1911fdc942aa82f62d7792d5da09) )
	ROM_LOAD( "gnome_ent.002", 0x100000, 0x80000, CRC(5d92e36c) SHA1(d0ead5702ce9b6a9e28f2dab3b5fd6fe23789988) )
	ROM_LOAD( "gnome_ent.003", 0x200000, 0x80000, CRC(1a2d3c3c) SHA1(9d519238891e95a0b25d7885d239dbcce422d042) )
	ROM_LOAD( "gnome_ent.004", 0x300000, 0x80000, CRC(885e1885) SHA1(9c4b1e220602fc192cda62254d31cfa16419cdbd) )
	ROM_LOAD( "gnome_ent.005", 0x080000, 0x80000, CRC(9a5ec2e1) SHA1(f0eca8d7912f0cd8fceb873bf37fc038584eff65) )
	ROM_LOAD( "gnome_ent.006", 0x180000, 0x80000, CRC(6809fe49) SHA1(bce6d182552c2e590da4b5a56292be533cb69bc7) )
	ROM_LOAD( "gnome_ent.007", 0x280000, 0x80000, CRC(09d6a157) SHA1(95a25c0ffb5d6d42323140bb66695cfed9c0daca) )
	ROM_LOAD( "gnome_ent.008", 0x380000, 0x80000, CRC(cba3676e) SHA1(306a7d9c3d229e86d735a2b0a9a71d2f33929038) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( gnome_9 ) // 100326 World
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_m_100326.rom", 0x00000, 0x40000, CRC(f35f2602) SHA1(ef86ca59e4f342f3b949c43e3204560d16271eaa) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(bf13bc68) SHA1(8740849128bba3ec3c9e9f14d50da982f74a433e) )
ROM_END

ROM_START( gnome_10 ) // 100326 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_l_100326.rom", 0x00000, 0x40000, CRC(cbedcefc) SHA1(54258cc653a1f3b4cb044f9f393138168794831e) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome_loto.001", 0x000000, 0x80000, CRC(15f75190) SHA1(85587a008889b5e34f5f79ceb1abfcd9a6c53cec) )
	ROM_LOAD( "gnome_loto.002", 0x100000, 0x80000, CRC(26f9af6a) SHA1(131b26e035b4cfd9d36ab8a7f2957e77170a529d) )
	ROM_LOAD( "gnome_loto.003", 0x200000, 0x80000, CRC(7d388bd5) SHA1(2f2eadc44f35033d61dbab390a4dbfec23f31c85) )
	ROM_LOAD( "gnome_loto.004", 0x300000, 0x80000, CRC(7bad4ac5) SHA1(2cfac6462b666b4bb0d546932b6784a80cf8d0d4) )
	ROM_LOAD( "gnome_loto.005", 0x080000, 0x80000, CRC(f86a7d02) SHA1(1e7da8ac89eb8b1d2c293d2cfead7a52524fc674) )
	ROM_LOAD( "gnome_loto.006", 0x180000, 0x80000, CRC(d66f1ab8) SHA1(27b612ab42008f8673a0508a1b813c63a0e2ba4c) )
	ROM_LOAD( "gnome_loto.007", 0x280000, 0x80000, CRC(99ae985c) SHA1(f0fe5a0dbc289a93246a825f32a726cf62ccb9aa) )
	ROM_LOAD( "gnome_loto.008", 0x380000, 0x80000, CRC(4dc3f777) SHA1(3352170877c59daff63c056dfca00915f87b5795) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(6bc033cd) SHA1(6e676a7e8695708567f03759fe55f24ffb749619) )
ROM_END

ROM_START( gnome_11 ) // 100326 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_e_100326.rom", 0x00000, 0x40000, CRC(9e11dd7c) SHA1(77e6f27670de01b4d0f0cb5475979e236ea25224) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome_ent.001", 0x000000, 0x80000, CRC(84c84c44) SHA1(09173c35667f1911fdc942aa82f62d7792d5da09) )
	ROM_LOAD( "gnome_ent.002", 0x100000, 0x80000, CRC(5d92e36c) SHA1(d0ead5702ce9b6a9e28f2dab3b5fd6fe23789988) )
	ROM_LOAD( "gnome_ent.003", 0x200000, 0x80000, CRC(1a2d3c3c) SHA1(9d519238891e95a0b25d7885d239dbcce422d042) )
	ROM_LOAD( "gnome_ent.004", 0x300000, 0x80000, CRC(885e1885) SHA1(9c4b1e220602fc192cda62254d31cfa16419cdbd) )
	ROM_LOAD( "gnome_ent.005", 0x080000, 0x80000, CRC(9a5ec2e1) SHA1(f0eca8d7912f0cd8fceb873bf37fc038584eff65) )
	ROM_LOAD( "gnome_ent.006", 0x180000, 0x80000, CRC(6809fe49) SHA1(bce6d182552c2e590da4b5a56292be533cb69bc7) )
	ROM_LOAD( "gnome_ent.007", 0x280000, 0x80000, CRC(09d6a157) SHA1(95a25c0ffb5d6d42323140bb66695cfed9c0daca) )
	ROM_LOAD( "gnome_ent.008", 0x380000, 0x80000, CRC(cba3676e) SHA1(306a7d9c3d229e86d735a2b0a9a71d2f33929038) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(e79d5075) SHA1(76abb7deae73bf594f66c53927cc1bfa9155a902) )
ROM_END

ROM_START( gnome_12 ) // 100326 Russia
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "gn_100326.rom", 0x00000, 0x40000, CRC(78585923) SHA1(c3fadbb9e1de317ec5be78c102dd90a64b85e816) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "gnome.001", 0x000000, 0x80000, CRC(6ed866d7) SHA1(68d75d24d98e6d533cb26ceac0a680203cb26069) )
	ROM_LOAD( "gnome.002", 0x100000, 0x80000, CRC(f6e5e6f0) SHA1(9751e8df87f14a252595547d24b8dd865ee4f08d) )
	ROM_LOAD( "gnome.003", 0x200000, 0x80000, CRC(f8beb972) SHA1(3afbca8ce7e69d2dadae05f69205a6fd9036cf6a) )
	ROM_LOAD( "gnome.004", 0x300000, 0x80000, CRC(83357c38) SHA1(45cd31c4f02f9d7b1888701c2146d1e7229b6cb5) )
	ROM_LOAD( "gnome.005", 0x080000, 0x80000, CRC(687ad3e3) SHA1(23941a4f40c45029b9a43451f78b04c03c3cd7da) )
	ROM_LOAD( "gnome.006", 0x180000, 0x80000, CRC(7ef2b88a) SHA1(7e7de60fc6791731d7cfd6a50e2bc5af1bf5e4b2) )
	ROM_LOAD( "gnome.007", 0x280000, 0x80000, CRC(71976bdf) SHA1(c44dbfa75a0f12893b3177907fc93b3d5e8ad390) )
	ROM_LOAD( "gnome.008", 0x380000, 0x80000, CRC(c86a1586) SHA1(e622bca8dc618ca8edc1a7daa9c8286383caebef) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(77bf0ebd) SHA1(b3af0e8d2839de95afcaf01fe78a3b2a2516ca14) )
ROM_END


/*********************************************************
   Fruit Cocktail 2

    Roms 4-8 were changed 11/19/2008 (listed below as non "old" roms)

        The Igrosoft web site explains:
         The wrong representation of the number of free games
         on the page 6 of help was corrected

**********************************************************/

ROM_START( fcockt2 ) // 080707
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2_080707.rom", 0x00000, 0x40000, CRC(3a42f27d) SHA1(7ba91f52b1b0ac4513caebc2989f3b9d6f9dfde4) ) /* Not officially listed on Igrosoft's web site hash page */

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2.001",  0x000000, 0x80000, CRC(d1b9416d) SHA1(4d3cb0a6dbcf02bbd20d5c43df358882b2ad794d) )
	ROM_LOAD( "fruitcocktail2.002",  0x100000, 0x80000, CRC(69236be1) SHA1(9a2e6c8f279714f79a606c0b118b6bf1d8442cda) )
	ROM_LOAD( "fruitcocktail2.003",  0x200000, 0x80000, CRC(29aade8c) SHA1(bef42f8a25c90e3a1cccd13872a10eb8b2b2e276) )
	ROM_LOAD( "fruitcocktail2.004",  0x300000, 0x80000, CRC(4b9646e7) SHA1(26548a018401f4e07383eb145f8f0847677f3272) )
	ROM_LOAD( "fruitcocktail2_old.005", 0x080000, 0x80000, CRC(6b9e6b43) SHA1(c7fb17e91ec62b22da42f110d68b4f37e39de3ce) )
	ROM_LOAD( "fruitcocktail2_old.006", 0x180000, 0x80000, CRC(2c9f712e) SHA1(c3118154eafca74b66b3325a2e07c85f86f3544d) )
	ROM_LOAD( "fruitcocktail2_old.007", 0x280000, 0x80000, CRC(85ba9a86) SHA1(aa9b6170135e9e420509e8f7c1702c9896bc5d8e) )
	ROM_LOAD( "fruitcocktail2_old.008", 0x380000, 0x80000, CRC(a27c49a2) SHA1(7c9ee0e01f76ca3ab6716579f5dde7036050970b) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(73f70613) SHA1(853354ee41a13fa2ae013101608faf852f9a3feb) )
ROM_END

ROM_START( fcockt2_2 ) // 080904
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2_080904.rom", 0x00000, 0x40000, CRC(b4b38770) SHA1(d8f1034753274aed874ee552ab78e660ddaba939) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2.001",  0x000000, 0x80000, CRC(d1b9416d) SHA1(4d3cb0a6dbcf02bbd20d5c43df358882b2ad794d) )
	ROM_LOAD( "fruitcocktail2.002",  0x100000, 0x80000, CRC(69236be1) SHA1(9a2e6c8f279714f79a606c0b118b6bf1d8442cda) )
	ROM_LOAD( "fruitcocktail2.003",  0x200000, 0x80000, CRC(29aade8c) SHA1(bef42f8a25c90e3a1cccd13872a10eb8b2b2e276) )
	ROM_LOAD( "fruitcocktail2.004",  0x300000, 0x80000, CRC(4b9646e7) SHA1(26548a018401f4e07383eb145f8f0847677f3272) )
	ROM_LOAD( "fruitcocktail2_old.005", 0x080000, 0x80000, CRC(6b9e6b43) SHA1(c7fb17e91ec62b22da42f110d68b4f37e39de3ce) )
	ROM_LOAD( "fruitcocktail2_old.006", 0x180000, 0x80000, CRC(2c9f712e) SHA1(c3118154eafca74b66b3325a2e07c85f86f3544d) )
	ROM_LOAD( "fruitcocktail2_old.007", 0x280000, 0x80000, CRC(85ba9a86) SHA1(aa9b6170135e9e420509e8f7c1702c9896bc5d8e) )
	ROM_LOAD( "fruitcocktail2_old.008", 0x380000, 0x80000, CRC(a27c49a2) SHA1(7c9ee0e01f76ca3ab6716579f5dde7036050970b) )

	ROM_REGION( 0x8000, "m48t35", ROMREGION_ERASE00	)
ROM_END

ROM_START( fcockt2_3 ) // 080909
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2_m_080909.rom", 0x00000, 0x40000, CRC(6de0353c) SHA1(7a827a172cdd593f8b37a7737304a5a2e145d52d) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2.001",  0x000000, 0x80000, CRC(d1b9416d) SHA1(4d3cb0a6dbcf02bbd20d5c43df358882b2ad794d) )
	ROM_LOAD( "fruitcocktail2.002",  0x100000, 0x80000, CRC(69236be1) SHA1(9a2e6c8f279714f79a606c0b118b6bf1d8442cda) )
	ROM_LOAD( "fruitcocktail2.003",  0x200000, 0x80000, CRC(29aade8c) SHA1(bef42f8a25c90e3a1cccd13872a10eb8b2b2e276) )
	ROM_LOAD( "fruitcocktail2.004",  0x300000, 0x80000, CRC(4b9646e7) SHA1(26548a018401f4e07383eb145f8f0847677f3272) )
	ROM_LOAD( "fruitcocktail2_old.005", 0x080000, 0x80000, CRC(6b9e6b43) SHA1(c7fb17e91ec62b22da42f110d68b4f37e39de3ce) )
	ROM_LOAD( "fruitcocktail2_old.006", 0x180000, 0x80000, CRC(2c9f712e) SHA1(c3118154eafca74b66b3325a2e07c85f86f3544d) )
	ROM_LOAD( "fruitcocktail2_old.007", 0x280000, 0x80000, CRC(85ba9a86) SHA1(aa9b6170135e9e420509e8f7c1702c9896bc5d8e) )
	ROM_LOAD( "fruitcocktail2_old.008", 0x380000, 0x80000, CRC(a27c49a2) SHA1(7c9ee0e01f76ca3ab6716579f5dde7036050970b) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(ddf45c08) SHA1(1da0f411ffacb39a6207d15f285a163706245dd8) )
ROM_END

ROM_START( fcockt2_4 ) // 081105
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2_m_081105.rom", 0x00000, 0x40000,  CRC(2cc5313d) SHA1(b5e55acbb5936f49130758947ae22d1847800333) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2.001", 0x000000, 0x80000, CRC(d1b9416d) SHA1(4d3cb0a6dbcf02bbd20d5c43df358882b2ad794d) )
	ROM_LOAD( "fruitcocktail2.002", 0x100000, 0x80000, CRC(69236be1) SHA1(9a2e6c8f279714f79a606c0b118b6bf1d8442cda) )
	ROM_LOAD( "fruitcocktail2.003", 0x200000, 0x80000, CRC(29aade8c) SHA1(bef42f8a25c90e3a1cccd13872a10eb8b2b2e276) )
	ROM_LOAD( "fruitcocktail2.004", 0x300000, 0x80000, CRC(4b9646e7) SHA1(26548a018401f4e07383eb145f8f0847677f3272) )
	ROM_LOAD( "fruitcocktail2.005", 0x080000, 0x80000, CRC(3fc13d72) SHA1(ba0727138ef03d576d190cbce04b9eb0bba88a9a) )
	ROM_LOAD( "fruitcocktail2.006", 0x180000, 0x80000, CRC(8fc75fd7) SHA1(3b7cd8a3e04ca9d4494b37c801e21d1293f094e8) )
	ROM_LOAD( "fruitcocktail2.007", 0x280000, 0x80000, CRC(d37fcc0f) SHA1(57c2ea5dc747f16e2233305f2c73cb4b632aae2c) )
	ROM_LOAD( "fruitcocktail2.008", 0x380000, 0x80000, CRC(e3a9442c) SHA1(cbaba182e858b0f158756118e5da873e3ddfc0b9) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(c13d3d9c) SHA1(911dd037410e75c7211a0961cce784cb605945ae) )
ROM_END

ROM_START( fcockt2_5 ) // 081106
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2_081106.rom", 0x00000, 0x40000, CRC(d23347b3) SHA1(2d7d00af182c61fa166a8f3fd6fd830cf5eb78c6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2.001", 0x000000, 0x80000, CRC(d1b9416d) SHA1(4d3cb0a6dbcf02bbd20d5c43df358882b2ad794d) )
	ROM_LOAD( "fruitcocktail2.002", 0x100000, 0x80000, CRC(69236be1) SHA1(9a2e6c8f279714f79a606c0b118b6bf1d8442cda) )
	ROM_LOAD( "fruitcocktail2.003", 0x200000, 0x80000, CRC(29aade8c) SHA1(bef42f8a25c90e3a1cccd13872a10eb8b2b2e276) )
	ROM_LOAD( "fruitcocktail2.004", 0x300000, 0x80000, CRC(4b9646e7) SHA1(26548a018401f4e07383eb145f8f0847677f3272) )
	ROM_LOAD( "fruitcocktail2.005", 0x080000, 0x80000, CRC(3fc13d72) SHA1(ba0727138ef03d576d190cbce04b9eb0bba88a9a) )
	ROM_LOAD( "fruitcocktail2.006", 0x180000, 0x80000, CRC(8fc75fd7) SHA1(3b7cd8a3e04ca9d4494b37c801e21d1293f094e8) )
	ROM_LOAD( "fruitcocktail2.007", 0x280000, 0x80000, CRC(d37fcc0f) SHA1(57c2ea5dc747f16e2233305f2c73cb4b632aae2c) )
	ROM_LOAD( "fruitcocktail2.008", 0x380000, 0x80000, CRC(e3a9442c) SHA1(cbaba182e858b0f158756118e5da873e3ddfc0b9) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(bc471961) SHA1(b27de4e862d21377f0326875d7ce3c2d73fbd23b) )
ROM_END

ROM_START( fcockt2_6 ) // 090525 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2_l_090525.rom", 0x00000, 0x40000, CRC(2fa86f1d) SHA1(f365f96750cdea56b024e87606303051b1bc725f) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2_loto.001", 0x000000, 0x80000, CRC(e01a356f) SHA1(838add7aed1f044a57770ff40611906f3aa13997) )
	ROM_LOAD( "fruitcocktail2_loto.002", 0x100000, 0x80000, CRC(f2726212) SHA1(f3682b58776bca2858cfa51dd628c3bcd8b7d71d) )
	ROM_LOAD( "fruitcocktail2_loto.003", 0x200000, 0x80000, CRC(a164b307) SHA1(8c6431aad5971b5a8a151ea289401cff81c7687f) )
	ROM_LOAD( "fruitcocktail2_loto.004", 0x300000, 0x80000, CRC(42db8990) SHA1(7c7c4abd551eca2e9db916ab1b780adf131a0d46) )
	ROM_LOAD( "fruitcocktail2_loto.005", 0x080000, 0x80000, CRC(800d29aa) SHA1(5ec4f342acdf113b5c3967909cdb2cfef4ef72a7) )
	ROM_LOAD( "fruitcocktail2_loto.006", 0x180000, 0x80000, CRC(b9f21925) SHA1(f72c9654e89587f2ca050d7767a1db7c70024602) )
	ROM_LOAD( "fruitcocktail2_loto.007", 0x280000, 0x80000, CRC(62514e5f) SHA1(d96a9d0ef4f2d8978757e6d71e3bed7a973efa80) )
	ROM_LOAD( "fruitcocktail2_loto.008", 0x380000, 0x80000, CRC(3ba806fb) SHA1(dbc70c442061298bdb4ac8651429bdea678aebbf) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(dcde7ea8) SHA1(5973833eca7edde2d65c0d5977460aec1f380148) )
ROM_END

ROM_START( fcockt2_7 ) // 090813 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "fc2e_090813.rom", 0x00000, 0x40000, CRC(f81ae7f0) SHA1(40a0a15d887906667b245fc4a68421008f478d27) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "fruitcocktail2_ent.001", 0x000000, 0x80000, CRC(843d6a33) SHA1(dba5739288a0fddac928ee99c23b4186b17f01ee) )
	ROM_LOAD( "fruitcocktail2_ent.002", 0x100000, 0x80000, CRC(4249ed51) SHA1(88884bc8685c8ef75cdb265a0fd7f7ebb416c2c7) )
	ROM_LOAD( "fruitcocktail2_ent.003", 0x200000, 0x80000, CRC(3d8edb5e) SHA1(1615acd711ff5b28ea61d0c5fb33fb140114a091) )
	ROM_LOAD( "fruitcocktail2_ent.004", 0x300000, 0x80000, CRC(caf02101) SHA1(02344f59a5b44c4ec5ca21bb9e14262f8503154c) )
	ROM_LOAD( "fruitcocktail2_ent.005", 0x080000, 0x80000, CRC(8b7fa4ad) SHA1(d38c64cc27fedbd9213f51b4c9889dbe0a84dde6) )
	ROM_LOAD( "fruitcocktail2_ent.006", 0x180000, 0x80000, CRC(e9d90f96) SHA1(b63dada78836d05166c6f2e81db23b4d91917151) )
	ROM_LOAD( "fruitcocktail2_ent.007", 0x280000, 0x80000, CRC(e478766b) SHA1(ae951202d4cb52cc4a53c1bb5eafc5bbcf7c8088) )
	ROM_LOAD( "fruitcocktail2_ent.008", 0x380000, 0x80000, CRC(9bbf362e) SHA1(156d2c90d8bde74f8938bdaddf3ccd31c67e05bb) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(31784c1e) SHA1(80f9b7d8d938138679495867b354a24b678bc6c1) )
ROM_END


/*********************************************************
   Crazy Monkey 2
**********************************************************/

ROM_START( crzmon2 ) // 100310
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm2_m_100310.rom", 0x00000, 0x40000, CRC(c7fa01c8) SHA1(180b5ce0e456abe9178255b8ea09afb953986310) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey2.001", 0x000000, 0x80000, CRC(8c8edfa7) SHA1(a68d4ebc370a09d588b906e5c254bccac4f53001) )
	ROM_LOAD( "crazymonkey2.002", 0x100000, 0x80000, CRC(6379769f) SHA1(3b3ffc3ce4436168db41f7e643b58dca94f250da) )
	ROM_LOAD( "crazymonkey2.003", 0x200000, 0x80000, CRC(b12f8080) SHA1(218fd38c3c2b2886084a1c694ce181f497c54085) )
	ROM_LOAD( "crazymonkey2.004", 0x300000, 0x80000, CRC(1da8ed1a) SHA1(9681e631eaeb26d242daba4d1c362302de6ca1bd) )
	ROM_LOAD( "crazymonkey2.005", 0x080000, 0x80000, CRC(82bfb2d3) SHA1(ca4c4a8b105fbcb3e120fc2d89866f3e66624e96) )
	ROM_LOAD( "crazymonkey2.006", 0x180000, 0x80000, CRC(e3dfdf6a) SHA1(67608c09d8c92f3278ddc11c27ddba8f3146c2c6) )
	ROM_LOAD( "crazymonkey2.007", 0x280000, 0x80000, CRC(329f4c3d) SHA1(27e1ed25b7e11c604abcf435f87b165201918def) )
	ROM_LOAD( "crazymonkey2.008", 0x380000, 0x80000, CRC(f7c8e613) SHA1(6dec3f6f27773cd0af83ca914d8481c8f17f1d3f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(9849f5ec) SHA1(9d9bd0a85fe0214e254cb2f5817696b39cffe4a4) )
ROM_END

ROM_START( crzmon2_2 ) // 100311 lottery
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm2_l_100311.rom", 0x00000, 0x40000, CRC(e7277872) SHA1(52000a5139056f98b1c991a80383646f349304af) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey2_loto.001", 0x000000, 0x80000, CRC(91792059) SHA1(23da523033e5344fc4ced4d0df5355f6d939467f) )
	ROM_LOAD( "crazymonkey2_loto.002", 0x100000, 0x80000, CRC(eccda383) SHA1(2f1518dd653ca890a742caad0dcc98f2da9e3ece) )
	ROM_LOAD( "crazymonkey2_loto.003", 0x200000, 0x80000, CRC(d3452ad0) SHA1(07ca63e377371ac1e06cd4daea7e1056a8fd8577) )
	ROM_LOAD( "crazymonkey2_loto.004", 0x300000, 0x80000, CRC(3e0ef510) SHA1(1e6c7e268694d1aa95a5e99a3ac9ea87eb6535c3) )
	ROM_LOAD( "crazymonkey2_loto.005", 0x080000, 0x80000, CRC(766e8066) SHA1(25f3e42b508ba105df15b5de639b7e2720793be8) )
	ROM_LOAD( "crazymonkey2_loto.006", 0x180000, 0x80000, CRC(4cf6ddc0) SHA1(b63ef431a4cba22d2e52fa511e92883edbcac3ac) )
	ROM_LOAD( "crazymonkey2_loto.007", 0x280000, 0x80000, CRC(97fe75c9) SHA1(c19162efb732b9eb90bdf00edede9c17d737718b) )
	ROM_LOAD( "crazymonkey2_loto.008", 0x380000, 0x80000, CRC(c2355fe7) SHA1(6ace750fdb2e6d0c35d42b2c7972782f58788337) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(62b593d2) SHA1(60ea8856198449023e8affafd26cb2ac6ce7a27b) )
ROM_END

ROM_START( crzmon2_3 ) // 100315 entertainment
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm2_e_100315.rom", 0x00000, 0x40000, CRC(b98fc9f4) SHA1(30bd29707ae98cdc7e9ee9fa2b46cec956fc78a6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey2_ent.001", 0x000000, 0x80000, CRC(e5051ebb) SHA1(1cb9d4eda3a752966fa706f3cf43bc6448a29d1f) )
	ROM_LOAD( "crazymonkey2_ent.002", 0x100000, 0x80000, CRC(55d2165b) SHA1(aa85f8d67e659626de97a78d349e9a7a47127eeb) )
	ROM_LOAD( "crazymonkey2_ent.003", 0x200000, 0x80000, CRC(981494a9) SHA1(4a6d28112529aed027d807d383f7077878886909) )
	ROM_LOAD( "crazymonkey2_ent.004", 0x300000, 0x80000, CRC(84c3c8a8) SHA1(e84d3bddadc32c4fad06c9d9a41b5cfd17a182d1) )
	ROM_LOAD( "crazymonkey2_ent.005", 0x080000, 0x80000, CRC(018d881c) SHA1(8816c68c4da3f396c08e62c3a9ff283524b6c6d9) )
	ROM_LOAD( "crazymonkey2_ent.006", 0x180000, 0x80000, CRC(f225d0a6) SHA1(bf0e8e49598293315ac7d8e0789d77266f433c25) )
	ROM_LOAD( "crazymonkey2_ent.007", 0x280000, 0x80000, CRC(89135627) SHA1(c2a245aaeebcb50453f127f79d6cf8eb2e757fe8) )
	ROM_LOAD( "crazymonkey2_ent.008", 0x380000, 0x80000, CRC(2b86f707) SHA1(0f5eac22b041a54ab40685f6a52869fad19cda60) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(84b6f391) SHA1(8ffc43ed73f614268c3d089b6f304e095ac21fe9) )
ROM_END

ROM_START( crzmon2_4 ) // 100618
	ROM_REGION( 0x40000, "maincpu", 0 ) // z80 code, banked
	ROM_LOAD( "cm2_100618.rom", 0x00000, 0x40000, CRC(3519e933) SHA1(1dad2fa2084e4c25f5d75cba3b631c7f3a1bdce6) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "crazymonkey2.001", 0x000000, 0x80000, CRC(8c8edfa7) SHA1(a68d4ebc370a09d588b906e5c254bccac4f53001) )
	ROM_LOAD( "crazymonkey2.002", 0x100000, 0x80000, CRC(6379769f) SHA1(3b3ffc3ce4436168db41f7e643b58dca94f250da) )
	ROM_LOAD( "crazymonkey2.003", 0x200000, 0x80000, CRC(b12f8080) SHA1(218fd38c3c2b2886084a1c694ce181f497c54085) )
	ROM_LOAD( "crazymonkey2.004", 0x300000, 0x80000, CRC(1da8ed1a) SHA1(9681e631eaeb26d242daba4d1c362302de6ca1bd) )
	ROM_LOAD( "crazymonkey2.005", 0x080000, 0x80000, CRC(82bfb2d3) SHA1(ca4c4a8b105fbcb3e120fc2d89866f3e66624e96) )
	ROM_LOAD( "crazymonkey2.006", 0x180000, 0x80000, CRC(e3dfdf6a) SHA1(67608c09d8c92f3278ddc11c27ddba8f3146c2c6) )
	ROM_LOAD( "crazymonkey2.007", 0x280000, 0x80000, CRC(329f4c3d) SHA1(27e1ed25b7e11c604abcf435f87b165201918def) )
	ROM_LOAD( "crazymonkey2.008", 0x380000, 0x80000, CRC(f7c8e613) SHA1(6dec3f6f27773cd0af83ca914d8481c8f17f1d3f) )

	ROM_REGION( 0x8000, "m48t35", 0 ) // factory initialized defaults
	ROM_LOAD( "m48t35", 0x0000, 0x8000, CRC(9849f5ec) SHA1(9d9bd0a85fe0214e254cb2f5817696b39cffe4a4) )
ROM_END


/*

Note:

   Only the first set of a given revision is listed in Igrosoft's official hashes list.

Most games had a revision in early 2007 to meet the standards of the "Government gambling control"
   law of The Russian Federation No 244-03 of Dec 29, 2006

   From Igrosoft's web site about version types (IE: some version have "M" in them):

   * Two software versions are shown, one of them corresponds to Russian legislation,
     the other one (with the letter m) is for the countries without such restrictions.

*/



GAME( 2002, goldfish,    mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gold Fish (020903, prototype)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish,       mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (021120)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_2,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (021121)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_3,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (021124)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_4,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (021219)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_5,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (021227)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_6,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (030124)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_7,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (030511)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_8,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (030522)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_9,     mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (031026)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_10,    mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (031117)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_11,    mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (031124)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_12,    mfish_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (040308)",           MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2002, mfish_13,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Multi Fish (040316)",           MACHINE_SUPPORTS_SAVE ) /* World */

GAME( 2002, windjamr,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Windjammer (021216)", MACHINE_SUPPORTS_SAVE ) /* World */

GAME( 2003, czmon,       czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (030217 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_2,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (030225 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_3,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (030227 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_4,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (030404 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_5,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (030421 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_6,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (031016 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_7,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (031110 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_8,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (050120 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_9,     czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (070315 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, czmon_12,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (090711 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2003, czmon_13,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (100311 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, czmon_15,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (100311 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2003, czmon_16,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Crazy Monkey (100312 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, czmon_17,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (100324 Entertainment X)", MACHINE_SUPPORTS_SAVE ) /* Entertainment X */
GAME( 2003, czmon_18,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (100331 Entertainment X)", MACHINE_SUPPORTS_SAVE ) /* Entertainment X */
GAME( 2003, czmon_19,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (101208 Entertainment N)", MACHINE_SUPPORTS_SAVE ) /* Entertainment N */
GAME( 2003, czmon_20,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (110111 Entertainment A)", MACHINE_SUPPORTS_SAVE ) /* Entertainment A */
GAME( 2003, czmon_21,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (110124 Entertainment A)", MACHINE_SUPPORTS_SAVE ) /* Entertainment A */
GAME( 2003, czmon_22,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (110204 Entertainment A)", MACHINE_SUPPORTS_SAVE ) /* Entertainment A */
GAME( 2003, czmon_23,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (110311 Entertainment B)", MACHINE_SUPPORTS_SAVE ) /* Entertainment B */
GAME( 2003, czmon_24,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (140526 Entertainment B)", MACHINE_SUPPORTS_SAVE ) /* Entertainment B */
GAME( 2003, czmon_25,    czmon_13, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmonent,  ROT0, "Igrosoft", "Crazy Monkey (110411 Entertainment C)", MACHINE_SUPPORTS_SAVE ) /* Entertainment C */

GAME( 2003, fcockt,      fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (030505 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_2,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (030512 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_3,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (030623 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_4,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (031028 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_5,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (031111 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_6,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (040216 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_7,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (050118 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_8,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (060111 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, fcockt_9,    fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (070305 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, fcockt_10,   fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (070517 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, fcockt_11,   fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (070822 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, fcockt_12,   fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail (070911 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, fcockt_14,   fcockt_8, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_fcocktent,  ROT0, "Igrosoft", "Fruit Cocktail (090708 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2003, lhaunt,      lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (030707 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, lhaunt_2,    lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (030804 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, lhaunt_3,    lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (031027 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, lhaunt_4,    lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (031111 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, lhaunt_5,    lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (040216 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, lhaunt_6,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (040825 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, lhaunt_7,    lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (070402 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, lhaunt_8,    lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Lucky Haunter (070604 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2003, lhaunt_10,   lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_lhauntent,  ROT0, "Igrosoft", "Lucky Haunter (090712 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2003, lhaunt_11,   lhaunt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_lhauntent,  ROT0, "Igrosoft", "Lucky Haunter (100331 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2003, rollfr,      rollfr_4, rollfr,          rollfr,          igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Roll Fruit (030821)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, rollfr_2,    rollfr_4, rollfr,          rollfr,          igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Roll Fruit (040318)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, rollfr_3,    rollfr_4, rollfr,          rollfr,          igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Roll Fruit (080327)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, rollfr_4,    0,        rollfr,          rollfr,          igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Roll Fruit (080331)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2003, rollfr_5,    rollfr_4, rollfr,          rollfr,          igrosoft_gamble_state, init_rollfruit,  ROT0, "Igrosoft", "Roll Fruit (100924)", MACHINE_NOT_WORKING|MACHINE_SUPPORTS_SAVE ) /* World */

GAME( 2004, garage,      garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (040122 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, garage_2,    garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (040123 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, garage_3,    garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (040216 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, garage_4,    garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (040219 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, garage_5,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (050311 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, garage_6,    garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (070213 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2004, garage_7,    garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Garage (070329 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2004, garage_9,    garage_5, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_garageent,  ROT0, "Igrosoft", "Garage (090715 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2004, rclimb,      rclimb_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Rock Climber (040815 World)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, rclimb_2,    rclimb_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Rock Climber (040823 World)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, rclimb_3,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Rock Climber (040827 World)", MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, rclimb_4,    rclimb_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Rock Climber (070322 Russia)", MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2004, rclimb_5,    rclimb_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Rock Climber (070621 Russia)", MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2004, rclimb_7,    rclimb_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_rclimbent,  ROT0, "Igrosoft", "Rock Climber (090716 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2004, sweetl,      0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Sweet Life (041220 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, sweetl_2,    sweetl,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Sweet Life (070412 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2004, sweetl_3,    sweetl,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_sweetlent,  ROT0, "Igrosoft", "Sweet Life (090720 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2004, resdnt,      resdnt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Resident (040415 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, resdnt_2,    resdnt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Resident (040513 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, resdnt_3,    resdnt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Resident (070222 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2004, resdnt_5,    resdnt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_resdntent,  ROT0, "Igrosoft", "Resident (090722 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2004, resdnt_6,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Resident (100311 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2004, resdnt_8,    resdnt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_resdntent,  ROT0, "Igrosoft", "Resident (100311 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2004, resdnt_9,    resdnt_6, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Resident (100316 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */

GAME( 2005, island,      0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Island (050713 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2005, island_2,    island,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Island (070409 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2005, island_3,    island,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_islandent,  ROT0, "Igrosoft", "Island (090806 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2005, pirate,      pirate_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Pirate (051229 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2005, pirate_2,    pirate_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Pirate (060210 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2005, pirate_3,    0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Pirate (060803 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2005, pirate_4,    pirate_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Pirate (070412 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2005, pirate_5,    pirate_3, igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_pirateent,  ROT0, "Igrosoft", "Pirate (090803 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2006, island2,     0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Island 2 (060529 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, island2_2,   island2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Island 2 (061214 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, island2_3,   island2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Island 2 (061218 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, island2_4,   island2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Island 2 (070205 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2006, island2_5,   island2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_island2l,   ROT0, "Igrosoft", "Island 2 (090528 Lottery)",       MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2006, island2_6,   island2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_island2ent, ROT0, "Igrosoft", "Island 2 (090724 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2006, pirate2,     0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Pirate 2 (061005 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, pirate2_2,   pirate2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Pirate 2 (070126 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2006, pirate2_3,   pirate2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_pirate2l,   ROT0, "Igrosoft", "Pirate 2 (090528 Lottery)",       MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2006, pirate2_4,   pirate2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_pirate2ent, ROT0, "Igrosoft", "Pirate 2 (090730 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2006, keks,        keks_6,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Keks (060328 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, keks_2,      keks_6,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Keks (060403 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, keks_3,      keks_6,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Keks (070119 Russia)",        MACHINE_SUPPORTS_SAVE )  /* Russia */
GAME( 2006, keks_4,      keks_6,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_keksl,      ROT0, "Igrosoft", "Keks (090604 Lottery)",       MACHINE_SUPPORTS_SAVE )  /* Lottery */
GAME( 2006, keks_5,      keks_6,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_keksent,    ROT0, "Igrosoft", "Keks (090727 Entertainment)", MACHINE_SUPPORTS_SAVE )  /* Entertainment */
GAME( 2006, keks_6,      0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Keks (110816 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2006, keks_7,      keks_6,   igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Keks (110816 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */

GAME( 2007, gnome,       gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (070906 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2007, gnome_2,     gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (071115 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2007, gnome_3,     gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (080303 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2007, gnome_4,     gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (090402 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2007, gnome_5,     gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (090406 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2007, gnome_7,     gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_gnomel,     ROT0, "Igrosoft", "Gnome (090708 Lottery)",       MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2007, gnome_8,     gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_gnomeent,   ROT0, "Igrosoft", "Gnome (090810 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2007, gnome_9,     0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (100326 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2007, gnome_10,    gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_gnomel,     ROT0, "Igrosoft", "Gnome (100326 Lottery)",       MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2007, gnome_11,    gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_gnomeent,   ROT0, "Igrosoft", "Gnome (100326 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2007, gnome_12,    gnome_9,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Gnome (100326 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */

GAME( 2007, sweetl2,     0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Sweet Life 2 (071217 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2007, sweetl2_2,   sweetl2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Sweet Life 2 (080320 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2007, sweetl2_3,   sweetl2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_sweetl2l,   ROT0, "Igrosoft", "Sweet Life 2 (090525 Lottery)",       MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2007, sweetl2_4,   sweetl2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_sweetl2ent, ROT0, "Igrosoft", "Sweet Life 2 (090812 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2008, fcockt2,     0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail 2 (080707 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2008, fcockt2_2,   fcockt2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail 2 (080904 Russia)",        MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2008, fcockt2_3,   fcockt2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail 2 (080909 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2008, fcockt2_4,   fcockt2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail 2 (081105 World)",         MACHINE_SUPPORTS_SAVE ) /* World */
GAME( 2008, fcockt2_5,   fcockt2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, empty_init,      ROT0, "Igrosoft", "Fruit Cocktail 2 (081106 Russia)",        MACHINE_SUPPORTS_SAVE ) /* Russia */
GAME( 2008, fcockt2_6,   fcockt2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_fcockt2l,   ROT0, "Igrosoft", "Fruit Cocktail 2 (090528 Lottery)",       MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2008, fcockt2_7,   fcockt2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_fcockt2ent, ROT0, "Igrosoft", "Fruit Cocktail 2 (090813 Entertainment)", MACHINE_SUPPORTS_SAVE ) /* Entertainment */

GAME( 2010, crzmon2,     0,        igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmon2,    ROT0, "Igrosoft", "Crazy Monkey 2 (100310 World)",          MACHINE_NOT_WORKING|MACHINE_SUPPORTS_SAVE ) /* World */ // xored and bitswapped palette and gfx roms
GAME( 2010, crzmon2_2,   crzmon2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmon2lot, ROT0, "Igrosoft", "Crazy Monkey 2 (100311 Lottery)",        MACHINE_NOT_WORKING|MACHINE_SUPPORTS_SAVE ) /* Lottery */
GAME( 2010, crzmon2_3,   crzmon2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmon2ent, ROT0, "Igrosoft", "Crazy Monkey 2 (100315 Entertainment)",  MACHINE_NOT_WORKING|MACHINE_SUPPORTS_SAVE ) /* Entertainment */
GAME( 2010, crzmon2_4,   crzmon2,  igrosoft_gamble, igrosoft_gamble, igrosoft_gamble_state, init_crzmon2,    ROT0, "Igrosoft", "Crazy Monkey 2 (100618 Russia)",         MACHINE_NOT_WORKING|MACHINE_SUPPORTS_SAVE ) /* Russia */
