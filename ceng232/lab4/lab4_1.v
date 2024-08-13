`timescale 1ns / 1ps

module ROM (
input [2:0] addr,
output reg [7:0] dataOut);

	// write your ROM code here
always @(*)
  begin
    case (addr)
      3'b000: dataOut = 8'b00000000;
      3'b001: dataOut = 8'b01010101;
      3'b010: dataOut = 8'b10101010;
      3'b011: dataOut = 8'b00110011;
      3'b100: dataOut = 8'b11001100;
      3'b101: dataOut = 8'b00001111;
      3'b110: dataOut = 8'b11110000;
      3'b111: dataOut = 8'b11111111;
      default: dataOut = 8'b00000000; 
    endcase
  end



endmodule

module Difference_RAM (
input mode,
input [2:0] addr,
input [7:0] dataIn,
input [7:0] mask,
input CLK,
output reg [7:0] dataOut);

	// write your XOR_RAM code here
reg [7:0] memory [0:7];
reg [7:0] check=0;
initial begin    
    memory[0]=8'b0;
    memory[1]=8'b0;
    memory[2]=8'b0;
    memory[3]=8'b0;
    memory[4]=8'b0;
    memory[5]=8'b0;
    memory[6]=8'b0;
    memory[7]=8'b0;
    end
always @(posedge CLK)
  begin
    check=dataIn;  
    if (mode == 0) 
    begin
      if(dataIn >= mask) 
      	begin 
      	memory[addr] <= (dataIn - mask);
      	end
      else
      	begin
      	memory[addr] <= (mask - dataIn);
      	end	
    end
end

always @(check,mode)
  begin
    if (mode == 1) 
    begin
      dataOut = memory[addr]; 
    end
end

endmodule


module EncodedMemory (
input mode,
input [2:0] index,
input [7:0] number,
input CLK,
output [7:0] result);

	wire [7:0] mask;

	ROM R(index, mask);
	Difference_RAM DR(mode, index, number, mask, CLK, result);

endmodule


