-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Zdenek Vasicek <vasicek AT fit.vutbr.cz>
-- Nikola Valesova, xvales02

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- komponenta CNT
	signal CNT_inc		: std_logic;
	signal CNT_dec		: std_logic;
	signal CNT_register	: std_logic_vector(7 downto 0);
	signal CNT_zero		: std_logic;
	signal CNT_make_one	: std_logic;

	-- komponenta TMP
	signal TMP_ld		: std_logic;
	signal TMP_DATA_WDATA	: std_logic_vector(7 downto 0); 

	-- komponenta PC
	signal PC_inc		: std_logic;
	signal PC_dec		: std_logic;
	signal PC_DATA_ADDR	: std_logic_vector(12 downto 0); 

	-- komponenta PTR
	signal PTR_inc		: std_logic;
	signal PTR_dec		: std_logic;
	signal PTR_DATA_ADDR	: std_logic_vector(12 downto 0);

	-- komponenta MX1
	signal MX1_sel	: std_logic_vector(0 downto 0);

	-- komponenta MX2
	signal DECREMENT_DATA_WDATA	: std_logic_vector(7 downto 0);
	signal INCREMENT_DATA_WDATA	: std_logic_vector(7 downto 0);
	signal DEC_DATA_RDATA		: std_logic_vector(7 downto 0);
	signal DATA_RDATA_zero		: std_logic;
	signal MX2_sel				: std_logic_vector(1 downto 0);

	-- FSM
	type FSM_state is (sidle, sfetch0, sfetch2, sdecode,
	sincrement_ptr,							-- >
	sdecrement_ptr,							-- <
	sincrement_value, sincrement_value2,	-- +
	sdecrement_value, sdecrement_value2,	-- -
	swhile_begin, swhile_begin2, swhile_begin3,	swhile_begin4,			-- [
	swhile_begin5, swhile_begin6, swhile_begin7, swhile_begin8,
	swhile_end,	swhile_end2, swhile_end3, swhile_end4, swhile_end5,		-- ]
	swhile_end6, swhile_end7, swhile_end8, swhile_end9,
	sput_char, sput_char2,			-- .
	sget_char, sget_char2,			-- ,
	sdata_to_tmp, sdata_to_tmp2,	-- $
	stmp_to_data,					-- !
	sreturn,						-- NULL
	sothers							-- others
);

	signal pstate	: FSM_state;
	signal nstate	: FSM_state;


begin

-- CNT
cnt: process (RESET, CLK, CNT_dec, CNT_inc, CNT_register)
begin
	if (RESET = '1') then
		CNT_register <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (CNT_dec = '1') then
			CNT_register <= CNT_register - 1;
		elsif (CNT_inc = '1') then
			CNT_register <= CNT_register + 1;
		end if;
		if (CNT_make_one = '1') then
			CNT_register <= "00000001";
		end if;
	end if;
end process;

-- TMP
tmp: process (RESET, CLK, DATA_RDATA, tmp_ld)
begin
	if (RESET = '1') then
		TMP_DATA_WDATA <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (TMP_ld = '1') then
			TMP_DATA_WDATA <= DATA_RDATA;
		end if;
	end if;
end process;
	
-- PC
pc_cntr: process (RESET, CLK, PC_dec, PC_inc, PC_DATA_ADDR)
begin
	if (RESET = '1') then
		PC_DATA_ADDR <= (others => '0');
	elsif (CLK'event) and (CLK = '1') then
		if (PC_dec = '1') then
			PC_DATA_ADDR <= PC_DATA_ADDR - 1;
		elsif (PC_inc = '1') then
			PC_DATA_ADDR <= PC_DATA_ADDR + 1;
		end if;
	end if;
end process;

-- PTR
ptr: process (RESET, CLK, PTR_dec, PTR_inc, PTR_DATA_ADDR)
begin
	if (RESET = '1') then
		PTR_DATA_ADDR <= "1000000000000";
	elsif (CLK'event) and (CLK = '1') then
		if (PTR_dec = '1') then
			if (PTR_DATA_ADDR = "1000000000000") then
				PTR_DATA_ADDR <= "1111111111111";
			else
				PTR_DATA_ADDR <= PTR_DATA_ADDR - 1;
			end if;
		elsif (PTR_inc = '1') then
			if (PTR_DATA_ADDR = "1111111111111") then
				PTR_DATA_ADDR <= "1000000000000";
			else
				PTR_DATA_ADDR <= PTR_DATA_ADDR + 1;
			end if;
		end if;
	end if; 
end process;

-- MX1
mx1: process (MX1_sel, CLK, PTR_DATA_ADDR, PC_DATA_ADDR)
begin
	if (MX1_sel = "0") then
		DATA_ADDR <= PTR_DATA_ADDR;
	else
		DATA_ADDR <= PC_DATA_ADDR;
	end if;
end process;

-- MX2
mx2: process (MX2_sel, IN_DATA, TMP_DATA_WDATA, DATA_RDATA)
begin
	if (MX2_sel = "00") then
		DATA_WDATA <= DATA_RDATA + 1;
	elsif (MX2_sel = "01") then
		DATA_WDATA <= DATA_RDATA - 1;
	elsif (MX2_sel = "10") then
		DATA_WDATA <= TMP_DATA_WDATA;
	else 
		DATA_WDATA <= IN_DATA;
	end if;
end process;

-- OTHER SIGNALS
signals: process (CNT_register, DATA_RDATA)
begin
	if (CNT_register = "00000000") then
		CNT_zero <= '1';
	else
		CNT_zero <= '0';
	end if;
		
	if (DATA_RDATA = "00000000") then
		DATA_RDATA_zero <= '1';
	else
		DATA_RDATA_zero <= '0';
	end if;
end process;

DEC_DATA_RDATA <= DATA_RDATA - 1;

-- FSM
FSM_pstate: process (RESET, CLK, EN)
begin
	if (RESET = '1') then
		pstate <= sidle;
	elsif (CLK'event) and (CLK = '1') then
		if (EN = '1') then
			pstate <= nstate;
		end if;
	end if;
end process;

FSM_nstate: process (IN_VLD, OUT_BUSY, EN, DATA_RDATA_zero, DEC_DATA_RDATA, DATA_RDATA, CNT_register, pstate)
begin
-- INIT
	PC_inc <= '0';
	PC_dec <= '0';
	PTR_inc <= '0';
	PTR_dec <= '0';
	CNT_inc <= '0';
	CNT_dec <= '0';	
	TMP_ld <= '0';
	OUT_WE <= '0';
	DATA_EN <= '0';
	DATA_RDWR <= '0';
	CNT_make_one <= '0';
	OUT_DATA <= DATA_RDATA;
	nstate <= sidle;
	MX2_sel <= "11";
	MX1_sel <= "1";
	IN_REQ <= '0';
	
	if (EN = '1') then
		case pstate is
	-- IDLE
		when sidle =>
			nstate <= sfetch0;
			
	-- INSTRUCTION FETCH
		when sfetch0 =>
			DATA_RDWR <= '1';
			DATA_EN <= '1';
			MX1_sel <= "1";		
			nstate <= sfetch2;
			
		when sfetch2 =>				
			nstate <= sdecode;
			
	-- INSTRUCTION DECODE
		when sdecode =>
			DATA_EN <= '0';
			case DATA_RDATA is
			when X"3E" =>	-- >
				nstate <= sincrement_ptr;
			when X"3C" =>	-- <
				nstate <= sdecrement_ptr;
			when X"2B" =>	-- +
				nstate <= sincrement_value;
			when X"2D" =>	-- -
				nstate <= sdecrement_value;
			when X"5B" =>	-- [
				nstate <= swhile_begin;
			when X"5D" =>	-- ]
				nstate <= swhile_end;
			when X"2E" =>	-- .
				nstate <= sput_char;
			when X"2C" =>	-- ,
				nstate <= sget_char;
			when X"24" =>	-- $
				nstate <= sdata_to_tmp;
			when X"21" =>	-- !
				nstate <= stmp_to_data;
			when X"00" =>	-- NULL
				nstate <= sreturn;
			when others =>	-- others
				nstate <= sothers;
			end case;
			
	-- INCREMENT PTR
		when sincrement_ptr =>
			PTR_inc <= '1';
			PTR_dec <= '0';
			PC_inc <= '1';
			PC_dec <= '0';
			nstate <= sfetch0;
			
	-- DECREMENT PTR
		when sdecrement_ptr =>
			PTR_inc <= '0';
			PTR_dec <= '1';
			PC_inc <= '1';
			PC_dec <= '0';
			nstate <= sfetch0;
			
	-- INCREMENT VALUE
		when sincrement_value =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= sincrement_value2;
			
		when sincrement_value2 =>
			MX1_sel <= "0";
			MX2_sel <= "00";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			PC_inc <= '1';
			PC_dec <= '0';
			nstate <= sfetch0;
			
	-- DECREMENT VALUE
		when sdecrement_value =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= sdecrement_value2;
			
		when sdecrement_value2 =>
			MX1_sel <= "0";
			MX2_sel <= "01";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			PC_inc <= '1';
			PC_dec <= '0';
			nstate <= sfetch0;
	
	-- PUT CHAR
		when sput_char =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= sput_char2;
			
		when sput_char2 =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			if (OUT_BUSY = '1') then
				nstate <= sput_char2;
			else
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				PC_inc <= '1';
				PC_dec <= '0';
				nstate <= sfetch0;
			end if;
			
	-- GET CHAR
		when sget_char =>
			MX1_sel <= "0";
			IN_REQ <= '1';
			nstate <= sget_char2;
			
		when sget_char2 =>
			if (IN_VLD = '0') then
				nstate <= sget_char2;
			else
				MX1_sel <= "0";
				MX2_sel <= "11";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				PC_inc <= '1';
				PC_dec <= '0';
				IN_REQ <= '0';
				nstate <= sfetch0;
			end if;

	-- WHILE BEGIN
		when swhile_begin =>
			MX1_sel <= "0";
			PC_dec <= '0';
			PC_inc <= '1';
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= swhile_begin2;

		when swhile_begin2 =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= swhile_begin3;
			
		when swhile_begin3 =>
			if (DATA_RDATA_zero = '1') then
				CNT_make_one <= '1';
				nstate <= swhile_begin4;
			else
				nstate <= sfetch0;
			end if;
			
		when swhile_begin4 =>
			nstate <= swhile_begin5;

		when swhile_begin5 =>
			if (CNT_zero = '0') then				
				MX1_sel <= "1";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= swhile_begin6;
			else
				nstate <= sfetch0;
			end if;

		when swhile_begin6 =>
			MX1_sel <= "1";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= swhile_begin7;

		when swhile_begin7 =>
			if (DATA_RDATA = X"5B") then
				CNT_dec <= '0';
				CNT_inc <= '1';
				nstate <= swhile_begin8;
			elsif (DATA_RDATA = X"5D") then
				CNT_dec <= '1';
				CNT_inc <= '0';
				nstate <= swhile_begin8;
			else
				nstate <= swhile_begin8;
			end if;  

		when swhile_begin8 =>
			PC_dec <= '0';
			PC_inc <= '1';
			nstate <= swhile_begin4;

	-- WHILE END
		when swhile_end =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= swhile_end2;

		when swhile_end2 =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= swhile_end3;

		when swhile_end3 =>
			if (DATA_RDATA_zero = '1') then
				PC_inc <= '1';
				PC_dec <= '0';
				nstate <= sfetch0;
			else
				PC_inc <= '0';
				PC_dec <= '1';
				CNT_make_one <= '1';
				nstate <= swhile_end4;
			end if;

		when swhile_end4 =>
			nstate <= swhile_end5;
			
		when swhile_end5 =>
			if (CNT_zero = '0') then
				MX1_sel <= "1";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= swhile_end6;
			else
				nstate <= sfetch0;
			end if;

		when swhile_end6 =>
			MX1_sel <= "1";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= swhile_end7;

		when swhile_end7 =>
			if (DATA_RDATA = X"5D") then
				CNT_inc <= '1';
				CNT_dec <= '0';
				nstate <= swhile_end8;
			elsif (DATA_RDATA = X"5B") then
				CNT_inc <= '0';
				CNT_dec <= '1';
				nstate <= swhile_end8;
			else
				nstate <= swhile_end8;
			end if;
			
		when swhile_end8 =>
			nstate <= swhile_end9;
			
		when swhile_end9 =>
			if (CNT_zero = '1') then
				PC_inc <= '1';
				PC_dec <= '0';
			else
				PC_inc <= '0';
				PC_dec <= '1';
			end if;
			nstate <= swhile_end4;

	-- DATA TO TMP
		when sdata_to_tmp =>
			MX1_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			nstate <= sdata_to_tmp2;
			
		when sdata_to_tmp2 =>
			MX1_sel <= "0";
			DATA_EN <= '0';
			TMP_ld <= '1';
			PC_inc <= '1';
			PC_dec <= '0';
			nstate <= sfetch0;
			
	-- TMP TO DATA
		when stmp_to_data =>
			MX1_sel <= "0";
			MX2_sel <= "10";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			PC_inc <= '1';
			PC_dec <= '0';
			nstate <= sfetch0;
			
	-- RETURN 
		when sreturn =>
			nstate <= sreturn;
	
	-- OTHERS
		when sothers =>
			nstate <= sfetch0;				
			PC_inc <= '1';
			PC_dec <= '0';

		when others =>
			nstate <= sfetch0;
		end case;
	
	else
		nstate <= pstate;
	end if;
	
end process;

end behavioral;