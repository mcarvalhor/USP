-- VendingMachine.vhd

library ieee;
use ieee.std_logic_1164.all;




entity VendingMachine is -- Top-Level: Vending Machine
	port(
		Clock:										in bit;
		InUmReal, InDoisReais, Timeout:		in bit;
		OutDinheiro, OutCoxinha, OutPepsi:	out bit
	);
end entity;



architecture Main of VendingMachine is
type fsm_state is (S0,S1,S2,S3,S4,S5);
signal State : fsm_state;
begin
	process (Clock)
	begin
		if Clock = '1' and Clock'event then
				if Timeout = '1' then -- Timeout no nosso projeto simula o Reset e é sincrono. porque antes de resetar ele deve retornar a saída.
					case State is
						when S0 =>
							OutDinheiro <= '0';
							OutPepsi <= '0';
							OutCoxinha <= '0';
						when S1 =>
							OutDinheiro <= '1';
							OutPepsi <= '0';
							OutCoxinha <= '0';
						when S2 =>
							OutDinheiro <= '0';
							OutPepsi <= '1';
							OutCoxinha <= '0';
						when S3 =>
							OutDinheiro <= '0';
							OutPepsi <= '1';
							OutCoxinha <= '0';
						when S4 =>
							OutDinheiro <= '0';
							OutPepsi <= '1';
							OutCoxinha <= '0';
						when S5 =>
							OutDinheiro <= '0';
							OutPepsi <= '0';
							OutCoxinha <= '1';
					end case;
					State <= s0; -- Resetar o estado
				else
					case State is
						when S0=>
							OutDinheiro <= '0'; -- Resetar os valores
							OutPepsi <= '0';
							OutCoxinha <= '0';
							if InUmReal = '1' then
								State <= S1;
							elsif InDoisReais = '1' then
								State <= S2;
							end if;
						when S1=>
							if InUmReal = '1' then
								State <= S2;
							elsif InDoisReais = '1' then
								State <= S3;
							end if;
						when S2=>
							if InUmReal = '1' then
								State <= S3;
							elsif InDoisReais = '1' then
								State <= S4;
							end if;
						when S3=>
							if InUmReal = '1' then
								State <= S4;
							elsif InDoisReais = '1' then
								State <= S5;
							end if;
						when S4=>
							if InUmReal = '1' or InDoisReais = '1' then
								State <= S5;
							end if;
						when S5=>
							-- Não fazer nada.
					end case;
				end if;
		end if;
	end process;
end architecture;


