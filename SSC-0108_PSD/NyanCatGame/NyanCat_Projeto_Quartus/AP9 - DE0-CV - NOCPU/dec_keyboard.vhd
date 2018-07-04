library IEEE;
use  IEEE.STD_LOGIC_1164.all;
use  IEEE.STD_LOGIC_ARITH.all;
use  IEEE.STD_LOGIC_UNSIGNED.all;

ENTITY dec_keyboard IS

 PORT( hex_bus		: IN	STD_LOGIC_VECTOR(7 downto 0);
	   scan_rd 		: IN 	STD_LOGIC;
	   clk	 		: IN 	STD_LOGIC;		
	   bin_digit    : OUT 	STD_LOGIC_VECTOR(7 downto 0)
	 );
	 
END dec_keyboard;

-------------------------- -- ------------------------------------

ARCHITECTURE a OF dec_keyboard IS
 type bool is (false, true);
 signal ReturnChar	: bool := true;

 BEGIN

------------------------ -- ----------------------------------------
  PROCESS (hex_bus)
   BEGIN
		--bin_digit <= x"ff";
		--if scan_rd'event and scan_rd = '1' then
		--if scan_rd = '1' then
			if ReturnChar = true then
				case hex_bus is --bloco das teclas normais (n�o e' case sensitive)
				  when x"1c" => bin_digit <= x"41"; --A
				  when x"32" => bin_digit <= x"42"; --B
				  when x"21" => bin_digit <= x"43"; --C
				  when x"23" => bin_digit <= x"44"; --D
				  when x"24" => bin_digit <= x"45"; --E
				  when x"2b" => bin_digit <= x"46"; --F
				  when x"34" => bin_digit <= x"47"; --G
				  when x"33" => bin_digit <= x"48"; --H
				  when x"43" => bin_digit <= x"49"; --I
				  when x"3b" => bin_digit <= x"4a"; --J
				  when x"42" => bin_digit <= x"4b"; --K
				  when x"4b" => bin_digit <= x"4c"; --L
				  when x"3a" => bin_digit <= x"4d"; --M
				  when x"31" => bin_digit <= x"4e"; --N
				  when x"44" => bin_digit <= x"4f"; --O
				  when x"4d" => bin_digit <= x"50"; --P
				  when x"15" => bin_digit <= x"51"; --Q
				  when x"2d" => bin_digit <= x"52"; --R
				  when x"1b" => bin_digit <= x"53"; --S
				  when x"2c" => bin_digit <= x"54"; --T
				  when x"3c" => bin_digit <= x"55"; --U
				  when x"2a" => bin_digit <= x"56"; --V
				  when x"1d" => bin_digit <= x"57"; --W
				  when x"22" => bin_digit <= x"58"; --X
				  when x"35" => bin_digit <= x"59"; --Y
				  when x"1a" => bin_digit <= x"5a"; --Z

				  when x"29" => bin_digit <= x"20"; --space
				  when x"66" => bin_digit <= x"08"; --backspace
				  when x"5a" => bin_digit <= x"0d"; --enter

				  when x"12" => bin_digit <= x"0e"; --shift esquerdo
				  when x"59" => bin_digit <= x"0f"; --shift direito
				  when x"14" => bin_digit <= x"fd"; --ctrl  Codigo Inventado
				  when x"11" => bin_digit <= x"fe"; --alt  Codigo Inventado
				  when x"0d" => bin_digit <= x"09"; --tab
				  when x"76" => bin_digit <= x"1b"; --ESC
				  
				  when x"41" => bin_digit <= x"3c"; -- <
				  when x"49" => bin_digit <= x"3e"; -- >
				  when x"61" => bin_digit <= x"5c"; -- \
				  when x"4a" => bin_digit <= x"3f"; -- ?
				  when x"51" => bin_digit <= x"2f"; -- /
				  when x"4c" => bin_digit <= x"3b"; -- ;
				  when x"52" => bin_digit <= x"7e"; -- ~ 
				  when x"5d" => bin_digit <= x"5d"; -- ] 
				  when x"54" => bin_digit <= x"60"; -- ` 
				  when x"5b" => bin_digit <= x"5b"; -- [
					
				  when x"69" => bin_digit <= x"e1"; --num1  Codigo Inventado
				  when x"72" => bin_digit <= x"e2"; --num2  Codigo Inventado
				  when x"7a" => bin_digit <= x"e3"; --num3  Codigo Inventado
				  when x"6b" => bin_digit <= x"e4"; --num4  Codigo Inventado
				  when x"73" => bin_digit <= x"e5"; --num5  Codigo Inventado
				  when x"74" => bin_digit <= x"e6"; --num6  Codigo Inventado
				  when x"6c" => bin_digit <= x"e7"; --num7  Codigo Inventado
				  when x"75" => bin_digit <= x"e8"; --num8  Codigo Inventado
				  when x"7d" => bin_digit <= x"e9"; --num9  Codigo Inventado
				 
					
				  when x"16" => bin_digit <= x"31"; --1
				  when x"1e" => bin_digit <= x"32"; --2
				  when x"26" => bin_digit <= x"33"; --3
				  when x"25" => bin_digit <= x"34"; --4
				  when x"2e" => bin_digit <= x"35"; --5
				  when x"36" => bin_digit <= x"36"; --6
				  when x"3d" => bin_digit <= x"37"; --7
				  when x"3e" => bin_digit <= x"38"; --8
				  when x"46" => bin_digit <= x"39"; --9
				  when x"45" => bin_digit <= x"30"; --0
				  
				  when x"05" => bin_digit <= x"f1"; --F1  Codigo Inventado
				  when x"06" => bin_digit <= x"f2"; --F2  Codigo Inventado
				  when x"04" => bin_digit <= x"f3"; --F3  Codigo Inventado
				  when x"0C" => bin_digit <= x"f4"; --F4  Codigo Inventado
				  when x"03" => bin_digit <= x"f5"; --F5  Codigo Inventado
				  when x"0B" => bin_digit <= x"f6"; --F6  Codigo Inventado
				  when x"83" => bin_digit <= x"f7"; --F7  Codigo Inventado
				  when x"0A" => bin_digit <= x"f8"; --F8  Codigo Inventado
				  when x"01" => bin_digit <= x"f9"; --F9	Codigo Inventado
				  when x"09" => bin_digit <= x"fa"; --F10  Codigo Inventado
				  when x"78" => bin_digit <= x"fb"; --F11  Codigo Inventado
				  when x"07" => bin_digit <= x"fc"; --F12  Codigo Inventado
				  
				  
				  when x"F0" => bin_digit <= x"ff"; ReturnChar <= false; -- BREAK CODE
				  
				  
					
				  when others => bin_digit <= x"ff";	-- Se nao for nenhum desses, responde FF
				end case;
			else
				ReturnChar <= true;
			end if;
		--end if;
END PROCESS;

--process (scan_rd)
--begin
--	if scan_rd'event and scan_rd = '1' then
		--ReturnChar <= true;
	--else
--		ReturnChar <= false;
--	end if;
--end process;

END a;
