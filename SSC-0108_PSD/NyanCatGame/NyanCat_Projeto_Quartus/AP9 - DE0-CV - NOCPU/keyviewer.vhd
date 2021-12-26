library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std;

entity keyviewer is
	port(
		clk: in std_logic;
		reset: in std_logic;
		key: in std_logic_vector(7 downto 0);
		videochar: out std_logic_vector(15 downto 0);
		videopos: out std_logic_vector(15 downto 0);
		videodraw: out std_logic;
		debug: out bit
	);
end keyviewer;

architecture behav of keyviewer is

type bool is (false, true);
type game_state is (playing, init, paused, over);
type char_type is (evilcat, dog, bird, player);

type Charac is record -- "Struct" dos personagens
	Show:bool;
	Char:char_type;
	Pos1:std_logic_vector(15 downto 0); -- Left Topper (0)
	Pos2:std_logic_vector(15 downto 0); -- Right Topper (1)
	Pos3:std_logic_vector(15 downto 0); -- Left Bottom (2)
	Pos4:std_logic_vector(15 downto 0); -- Right Bottom (3)
end record;

type CharacArray is array (9 downto 0) of Charac;
type Label_GameName is array (0 to 12) of std_logic_vector(7 downto 0);
type Label_GameOver is array (0 to 10) of std_logic_vector(7 downto 0);
type Label_GamePaused is array (0 to 6) of std_logic_vector(7 downto 0);
type Label_HowTo is array (0 to 17) of std_logic_vector(7 downto 0);

signal Fly:bool := false;
signal NyanCat:Charac := (true,player,x"0236",x"0237",x"025E",x"025F");
signal Enemies:CharacArray := (
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F"),
	(false,evilcat,x"0076",x"0077",x"009E",x"009F")
);
signal L_GameName:Label_GameName := (x"4E",x"59",x"41",x"4E",x"23",x"43",x"41",x"54",x"23",x"47",x"41",x"4D",x"45");
signal L_GameOver:Label_GameOver := (x"66", x"69", x"6D", x"23", x"64", x"65", x"23", x"6A", x"6F", x"67", x"6F");
signal L_GamePaused:Label_GamePaused := (x"70", x"61", x"75", x"73", x"61", x"64", x"6F");
signal L_HowTo:Label_HowTo := (x"7B", x"23", x"6A", x"6F", x"67", x"61", x"72", x"23", x"23", x"23", x"7C", x"23", x"70", x"61", x"75", x"73", x"61", x"72");
signal DrawPos:std_logic_vector(15 downto 0) := x"0000";
signal Points:std_logic_vector(31 downto 0) := x"00000000";
signal PointsCounter:std_logic_vector(15 downto 0) := x"0000";
signal NyanCounter:std_logic_vector(15 downto 0) := x"0000";
signal EnemiesCounter:std_logic_vector(15 downto 0) := x"0000";
signal EnemiesCounterVelocity:std_logic_vector(15 downto 0) := x"4E20";
signal EnemiesCounterMaxVelocity:std_logic_vector(15 downto 0) := x"1F40"; -- VELOCIDADE MAXIMA DOS INIMIGOS. PADRAO 10000.
signal RandomCounter:std_logic_vector(31 downto 0) := x"00000000";
signal AuxVector:std_logic_vector(15 downto 0);
signal BorderColor:std_logic_vector(3 downto 0):=x"B";
signal SpawnEnemy:bool:=true;
signal GameOver:bool:=false;
signal GamePaused:bool:=false;
signal GameState:game_state:=init;
signal Start:bool:=false;

begin
	
	process(clk, reset)
	begin
		if reset = '1' then
			videodraw <= '0';
			NyanCat <= (true,player,x"0236",x"0237",x"025E",x"025F");
			DrawPos <= x"0000";
			Points <= x"00000000";
			EnemiesCounterVelocity <= x"4E20";
			NyanCounter <= x"0000";
			BorderColor <= x"B";
			SpawnEnemy <= true;
			GameOver <= false;
			EnemiesCounter <= x"0000";
			RandomCounter <= x"00000000";
			GameState <= init;
			for i in 0 to 9 loop
				Enemies(i) <= (false,evilcat,x"0076",x"0077",x"009E",x"009F");
			end loop;
		elsif clk'event and clk = '1' then
		
			RandomCounter <= RandomCounter + RandomCounter + RandomCounter + NyanCat.Pos1;

			videochar(7 downto 0) <= x"23";
			videochar(11 downto 8) <= x"0";

			if DrawPos = NyanCat.Pos1 then
				videochar(7 downto 0) <= x"3A";
				videochar(11 downto 8) <= x"E";
			elsif DrawPos = NyanCat.Pos2 then
				videochar(7 downto 0) <= x"3B";
				videochar(11 downto 8) <= x"E";
			elsif DrawPos = NyanCat.Pos3 then
				videochar(7 downto 0) <= x"3C";
				videochar(11 downto 8) <= x"E";
			elsif DrawPos = NyanCat.Pos4 then
				videochar(7 downto 0) <= x"3D";
				videochar(11 downto 8) <= x"E";
			end if;
			
			for i in 0 to 9 loop
				if Enemies(i).Show = true then
					if DrawPos = Enemies(i).Pos1 then
						videochar(11 downto 8) <= x"7" + char_type'pos(Enemies(i).Char);
						videochar(7 downto 0) <= x"24" + char_type'pos(Enemies(i).Char)*4;
						exit;
					elsif DrawPos = Enemies(i).Pos2 then
						videochar(11 downto 8) <= x"7" + char_type'pos(Enemies(i).Char);
						videochar(7 downto 0) <= x"25" + char_type'pos(Enemies(i).Char)*4;
						exit;
					elsif DrawPos = Enemies(i).Pos3 then
						videochar(11 downto 8) <= x"7" + char_type'pos(Enemies(i).Char);
						videochar(7 downto 0) <= x"26" + char_type'pos(Enemies(i).Char)*4;
						exit;
					elsif DrawPos = Enemies(i).Pos4 then
						videochar(11 downto 8) <= x"7" + char_type'pos(Enemies(i).Char);
						videochar(7 downto 0) <= x"27" + char_type'pos(Enemies(i).Char)*4;
						exit;
					end if;
				end if;
			end loop;
			
			if DrawPos < 80 or DrawPos >= 1160 or conv_integer(DrawPos) mod 40 = 39 or conv_integer(DrawPos) mod 40 = 0 then
				videochar(7 downto 0) <= x"23";
				videochar(11 downto 8) <= BorderColor;
				for i in 0 to 12 loop
					if conv_integer(DrawPos) = 41+i then
						videochar(7 downto 0) <= L_GameName(i);
					end if;
				end loop;
				for i in 0 to 7 loop
					if conv_integer(DrawPos) = 71+(7-i) then
						videochar(7 downto 0) <= conv_std_logic_vector(48 + ((conv_integer(Points)/(10**i)) mod 10), 8);
					end if;
				end loop;
				if GameState = over then
					for i in 0 to 10 loop
						if conv_integer(DrawPos) = 1161+i then
							videochar(7 downto 0) <= L_GameOver(i);
						end if;
					end loop;
				elsif GameState = paused then
					for i in 0 to 6 loop
						if conv_integer(DrawPos) = 1161+i then
							videochar(7 downto 0) <= L_GamePaused(i);
						end if;
					end loop;
				end if;
				if GameState /= playing then
					for i in 0 to 17 loop
						if conv_integer(DrawPos) = 1181+i then
							videochar(7 downto 0) <= L_HowTo(i);
						end if;
					end loop;
				end if;
			end if;

			videopos <= DrawPos;
			videochar(15 downto 12) <= x"0";
			videodraw <= '1';
			if DrawPos > x"04AF" then
				DrawPos <= x"0000";
			else
				DrawPos <= DrawPos + 1;
			end if;

			case GameState is
				when playing =>
					if GamePaused = true then
						GameState <= paused;
					end if;
					if (PointsCounter > 5000) then -- TEMPO PARA SOMAR PONTUACAO. PADRAO 5000.
						Points <= Points + 1;
						if ((Points > 2500) and ((conv_integer(Points) mod 100) = 0)) then
							BorderColor <= BorderColor+1;
						end if;
						if (EnemiesCounterVelocity > EnemiesCounterMaxVelocity) then
							EnemiesCounterVelocity <= EnemiesCounterVelocity - 3;
						end if;
						PointsCounter <= x"0000";
					else
						PointsCounter <= PointsCounter + 1;
					end if;

					for i in 0 to 9 loop
						if Enemies(i).Show = true then
							if (Enemies(i).Pos1 = NyanCat.Pos1 or Enemies(i).Pos1 = NyanCat.Pos2 or Enemies(i).Pos1 = NyanCat.Pos3 or Enemies(i).Pos1 = NyanCat.Pos4) then
								GameState <= over;
								GameOver <= true;
								exit;
							elsif (Enemies(i).Pos2 = NyanCat.Pos1 or Enemies(i).Pos2 = NyanCat.Pos2 or Enemies(i).Pos2 = NyanCat.Pos3 or Enemies(i).Pos2 = NyanCat.Pos4) then
								GameState <= over;
								GameOver <= true;
								exit;
							elsif (Enemies(i).Pos3 = NyanCat.Pos1 or Enemies(i).Pos3 = NyanCat.Pos2 or Enemies(i).Pos3 = NyanCat.Pos3 or Enemies(i).Pos3 = NyanCat.Pos4) then
								GameState <= over;
								GameOver <= true;
								exit;
							elsif (Enemies(i).Pos4 = NyanCat.Pos1 or Enemies(i).Pos4 = NyanCat.Pos2 or Enemies(i).Pos4 = NyanCat.Pos3 or Enemies(i).Pos4 = NyanCat.Pos4) then
								GameState <= over;
								GameOver <= true;
								exit;
							end if;
						end if;
					end loop;

					if GameState = playing and (NyanCounter > 7500) then -- VELOCIDADE DO NYAN. PADRAO 7500.
						if Fly = true then
							NyanCat.Pos1 <= NyanCat.Pos1 - 40;
							NyanCat.Pos2 <= NyanCat.Pos2 - 40;
							NyanCat.Pos3 <= NyanCat.Pos3 - 40;
							NyanCat.Pos4 <= NyanCat.Pos4 - 40;
							if NyanCat.Pos1 < 120 then
								GameState <= over;
								GameOver <= true;
							end if;
						else
							NyanCat.Pos1 <= NyanCat.Pos1 + 40;
							NyanCat.Pos2 <= NyanCat.Pos2 + 40;
							NyanCat.Pos3 <= NyanCat.Pos3 + 40;
							NyanCat.Pos4 <= NyanCat.Pos4 + 40;
							if NyanCat.Pos3 > 1119 then
								GameState <= over;
								GameOver <= true;
							end if;
						end if;
						NyanCounter <= x"0000";
					else
						NyanCounter <= NyanCounter + 1;
					end if;
					
					if GameState = playing and (EnemiesCounter > EnemiesCounterVelocity) then
						for i in 0 to 9 loop
							if Enemies(i).Show = true then
								if ((conv_integer(Enemies(i).Pos1) mod 40) < 1) then
									Enemies(i) <= (false,evilcat,x"0076",x"0077",x"009E",x"009F");
								else
									if Enemies(i).Char = bird then
										if ((conv_integer(Enemies(i).Pos1) mod 2) = 0) and ((conv_integer(Enemies(i).Pos1) / 40) > (conv_integer(NyanCat.Pos1) / 40)) then
											Enemies(i).Pos1 <= Enemies(i).Pos1 - 41;
											Enemies(i).Pos2 <= Enemies(i).Pos2 - 41;
											Enemies(i).Pos3 <= Enemies(i).Pos3 - 41;
											Enemies(i).Pos4 <= Enemies(i).Pos4 - 41;
										elsif ((conv_integer(Enemies(i).Pos1) mod 2) = 0) and ((conv_integer(Enemies(i).Pos1) / 40) < (conv_integer(NyanCat.Pos1) / 40)) then
											Enemies(i).Pos1 <= Enemies(i).Pos1 + 39;
											Enemies(i).Pos2 <= Enemies(i).Pos2 + 39;
											Enemies(i).Pos3 <= Enemies(i).Pos3 + 39;
											Enemies(i).Pos4 <= Enemies(i).Pos4 + 39;
										else
											Enemies(i).Pos1 <= Enemies(i).Pos1 - 1;
											Enemies(i).Pos2 <= Enemies(i).Pos2 - 1;
											Enemies(i).Pos3 <= Enemies(i).Pos3 - 1;
											Enemies(i).Pos4 <= Enemies(i).Pos4 - 1;
										end if;
									else
										Enemies(i).Pos1 <= Enemies(i).Pos1 - 1;
										Enemies(i).Pos2 <= Enemies(i).Pos2 - 1;
										Enemies(i).Pos3 <= Enemies(i).Pos3 - 1;
										Enemies(i).Pos4 <= Enemies(i).Pos4 - 1;
									end if;
								end if;
							end if;
						end loop;
						if SpawnEnemy = true and ( (conv_integer(RandomCounter) mod 5) = 0) then -- Probabilidade de nascer inimigo.
							for i in 0 to 9 loop
								if Enemies(i).Show = false then
									AuxVector <= conv_std_logic_vector((conv_integer(RandomCounter) mod 26)*40, AuxVector'length); -- Posição Y aleatória.
									case conv_integer(RandomCounter) mod 9 is -- Probabilidade de cair cada tipo de inimigo.
										when 1=>
											Enemies(i).Char <= bird;
										when 2=>
											Enemies(i).Char <= dog;
										when 3=>
											Enemies(i).Char <= dog;
										when 4=>
											Enemies(i).Char <= dog;
										when others=>
											Enemies(i).Char <= evilcat;
									end case;
									Enemies(i).Pos1 <= Enemies(i).Pos1 + AuxVector;
									Enemies(i).Pos2 <= Enemies(i).Pos2 + AuxVector;
									Enemies(i).Pos3 <= Enemies(i).Pos3 + AuxVector;
									Enemies(i).Pos4 <= Enemies(i).Pos4 + AuxVector;
									Enemies(i).Show <= true;
									SpawnEnemy <= false;
									exit;
								end if;
							end loop;
						elsif SpawnEnemy = false then -- Flag para evitar que inimigos colidam entre si.
							SpawnEnemy <= true;
						end if;
						EnemiesCounter <= x"0000";
					else
						EnemiesCounter <= EnemiesCounter + 1;
					end if;
				when init =>
					if Fly = true then
						GameState <= playing;
					end if;
				when paused =>
					if GamePaused = false and (Fly = true or Start = true) then --and (Start = true or Fly = true) then
						GameState <= playing;
					end if;
				when over =>
					BorderColor <= x"B";
					if GameOver = false and Fly = true then
						Points <= x"00000000";
						EnemiesCounterVelocity <= x"4E20";
						NyanCounter <= x"0000";
						EnemiesCounter <= x"0000";
						SpawnEnemy <= true;
						NyanCat <= (true,player,x"0236",x"0237",x"025E",x"025F");
						for i in 0 to 9 loop
							Enemies(i) <= (false,evilcat,x"0076",x"0077",x"009E",x"009F");
						end loop;
						GameState <= playing;
					end if;
					if Start = true then
						GameOver <= false;
					end if;
			end case;

		end if;
	end process;

	process(reset, key)
	begin
		if reset = '1' then
			Fly <= false;
			Start <= false;
			GamePaused <= false;
		else
			if (key = x"20") or (key = x"57") or (key = x"77") then --or (key = x"38") then --or (key = x"77") then -- Space, W ou UpArrow/Num8
				Fly <= true;
				GamePaused <= false;
				Start <= false;
			elsif (key = x"0D") then -- 8D or 0D
				if GamePaused = false and GameState = playing then
					GamePaused <= true;
					Start <= false;
				elsif Start = true and GameState = paused then
					GamePaused <= false;
					Start <= false;
				end if;
			else
				Fly <= false;
				Start <= true;
			end if;
		end if;
	end process;

end architecture;