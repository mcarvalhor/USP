-- VendingMachineDetailed.vhd

library IEEE;
use IEEE.std_logic_1164.all;




entity VendingMachineDetailed is -- Top-Level: Vending Machine	
	port (
		InUmReal, InDoisReais, Timeout, Clock:	in bit;
		OutDinheiro, OutCoxinha, OutPepsi:		out bit
	);
end entity;

entity FlipFlopD is
	port (
		D, Set, Rst, Clk:	in bit;
		Q:						out bit
	);
end entity;



architecture Main of VendingMachineDetailed is
component FlipFlopD is
	port (
		D, Set, Rst, Clk:	in bit;
		Q:						out bit
	);
end component;
signal Q0:bit;
signal Q1:bit;
signal Q2:bit;
signal D0:bit;
signal D1:bit;
signal D2:bit;
signal R2,R1:bit;
begin
	R2<=InDoisReais;
	R1<=InUmReal;
	OutCoxinha <= (Q2 and not(Q1) and Q0) and Timeout;
	OutPepsi <= (Q1 or (Q2 and not(Q1) and not(Q0))) and Timeout;
	OutDinheiro <= (not(Q2) and not(Q1) and Q0) and Timeout;
	D0 <= (not(Q2) and not(Q1) and not(Q0) and not(R2) and R1) or (not(Q2) and Q1 and not(Q0) and not(R2) and R1) or (not(Q2) and Q0 and not(R2) and not(R1)) or (not(Q2) and Q0 and R2 and not(R1)) or (Q2 and not(Q1) and R2 and not(R1)) or (Q2 and not(Q1) and Q0 and not(R2)) or (Q2 and not(Q1) and not(R2) and R1);
	D1 <= (not(Q2) and Q1 and not(R2) and not(R1)) or (not(Q2) and Q1 and not(Q0) and not(R2)) or (not(Q2) and not(Q1) and R2 and not(R1)) or (not(Q2) and not(Q1) and Q0 and not(R2) and R1);
	D2 <= (Q2 and not(Q1) and not(R2)) or (Q2 and not(Q1) and R2 and not(R1)) or (not(Q2) and Q1 and R2 and not(R1)) or (not(Q2) and Q1 and Q0 and not(R2) and R1);
	FFD0: FlipFlopD port map (
		D => D0 and not(Timeout),
		Set => '0',
		Rst => '0',
		Clk => Clock,
		Q => Q0
	);
	FFD1: FlipFlopD port map (
		D => D1 and not(Timeout),
		Set => '0',
		Rst => '0',
		Clk => Clock,
		Q => Q1
	);
	FFD2: FlipFlopD port map (
		D => D2 and not(Timeout),
		Set => '0',
		Rst => '0',
		Clk => Clock,
		Q => Q2
	);
end architecture;



architecture BuildFF of FlipFlopD is
begin
	process(Set,Rst,Clk)
	begin
		if Rst = '1' then
			Q<='0';
		elsif Set = '1' then
			Q<='1';
		elsif Clk = '1' and Clk'event then
			Q<=D;
		end if;
	end process;
end architecture;


