--Project: Displaying on a matrix display
--Subject: Design of Computer Systems
--Author: Nikola Valesova, xvales02
--Date: 15. 11. 2015

--including libraries
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

--entity declaration
entity ledc8x8 is
port( 
	SMCLK, RESET: in std_logic;
	ROW, LED: out std_logic_vector(0 to 7)
); 
end ledc8x8;

--architecture spicification
architecture behv of ledc8x8 is
	signal ce, switch: std_logic;
	signal name, both, rowCounter: std_logic_vector(0 to 7);

begin
--duration counter (0.5 s estimated to be cirka 15,600 times the period of ce)
    duration_counter: process (SMCLK, RESET, ce, switch)
    variable counter: std_logic_vector(7 downto 0);
    variable duration: std_logic_vector(15 downto 0);
    begin
	if (RESET = '1') then
		counter := "00000000";			
		duration := "0000000000000000";
		switch <= '0';
	elsif ((SMCLK'event) and (SMCLK = '1')) then
		counter := counter + 1; 
		if counter = "11111111" then
			ce <= '1';
			duration := duration + 1;
		else
			ce <= '0';
		end if;  			
		if (switch = '0' and duration = "0011110011110000") then
			switch <= '1';
		end if;
	end if;       
    end process;	
	  
--row_counter
    row_counter: process (RESET, SMCLK, ce, rowCounter)
    begin
	if (RESET = '1') then
		rowCounter <= "10000000";
	elsif ((SMCLK'event) and (SMCLK = '1')) then
		if (ce = '1') then
			rowCounter <= rowCounter(7) & rowCounter(0 to 6);
		end if;
	end if;     
	ROW <= rowCounter;
    end process;
    	
--decoder for displaying name
   nameDecoder: process (rowCounter)
   begin
	case rowCounter is         
		when "10000000" => name <= "01101111";
		when "01000000" => name <= "00101111";
		when "00100000" => name <= "01001111";
		when "00010000" => name <= "01101111";
		when "00001000" => name <= "11111111";
		when "00000100" => name <= "11111111";
		when "00000010" => name <= "11111111";
		when "00000001" => name <= "11111111";	
		when others => name <= "11111111";
	end case;
   end process;
   
--decoder for displaying name and surname
   bothDecoder: process (rowCounter)
   begin
	case rowCounter is         
		when "10000000" => both <= "01101111";
		when "01000000" => both <= "00101111";
		when "00100000" => both <= "01001111";
		when "00010000" => both <= "01101111";
		when "00001000" => both <= "10111110";
		when "00000100" => both <= "11011101";
		when "00000010" => both <= "11101011";
		when "00000001" => both <= "11110111";	
		when others => both <= "11111111";
	end case;
   end process;   
  
--multiplexer
   multiplexer: process (SMCLK, RESET, switch, name, both)
   begin
	if (RESET = '1') then
		LED <= name;
	elsif ((SMCLK'event) and (SMCLK = '1')) then
		case switch is
			when '0' => LED <= name;
			when '1' => LED <= both;  
			when others => LED <= "11111111"; 
		end case;
	end if;
    end process;
	
end behv; 


