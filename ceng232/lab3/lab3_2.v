`timescale 1ns / 1ps
module lab3_2(
			input[5:0] money,
			input CLK,
			input vm, //0:VM0, 1:VM1
			input [2:0] productID, //000:sandwich, 001:chocolate, 11x: dont care
			input sugar, //0: No sugar, 1: With Sugar
			output reg [5:0] moneyLeft,
			output reg [4:0] itemLeft,
			output reg productUnavailable,//1:show warning, 0:do not show warning
			output reg insufficientFund , //1:full, 0:not full
			output reg notExactFund , //1:full, 0:not full
			output reg invalidProduct, //1: empty, 0:not empty
			output reg sugarUnsuitable, //1: empty, 0:not empty
			output reg productReady	//1:door is open, 0:closed
	);

	// Internal State of the Module
	// (you can change this but you probably need this)
	reg [4:0] numOfSandwiches;
	reg [4:0] numOfChocolate;
	reg [4:0] numOfWaterVM1;
	reg [4:0] numOfWaterVM2;
	reg [4:0] numOfCoffee;
	reg [4:0] numOfTea;

	initial
	begin
		// You can initialize here
		numOfSandwiches=10;
		numOfChocolate=10;
		numOfWaterVM1=5;
		numOfWaterVM2=10;
		numOfCoffee=10;
		numOfTea=10;
		insufficientFund=0;
		notExactFund=0;
		invalidProduct=0;
		sugarUnsuitable=0;
		productReady=0;
		productUnavailable=0;
		
		
		
    	// ...
	end

	//Modify the lines below to implement your design
	always @(posedge CLK)
	begin

		// You can implement your code here,
		insufficientFund=0;
		notExactFund=0;
		invalidProduct=0;
		sugarUnsuitable=0;
		productReady=0;
		productUnavailable=0;
		moneyLeft=0;
		itemLeft=0;
		if (vm==0)
			begin
			case(productID) 
				 3'b000:begin
					if (money==20 && numOfSandwiches > 0 ) begin 
						itemLeft=numOfSandwiches-1;
						numOfSandwiches=numOfSandwiches -1;
						productReady=1;
						moneyLeft=0;
						end
					else if (numOfSandwiches==0) begin
						productUnavailable=1;
						moneyLeft=money;
						end
					else if (money!=20) begin
						notExactFund=1;
						moneyLeft=money;
						end
					end
				3'b001:begin
					if(money==10 && numOfChocolate >0) begin
						itemLeft=numOfChocolate-1;
						numOfChocolate=numOfChocolate-1;
						productReady=1;
						moneyLeft=0;
						end
					else if(numOfChocolate==0) begin
						productUnavailable=1;
						moneyLeft=money;
						end
					else if(money!=10) begin 
						notExactFund=1;
						moneyLeft=money;
						end
					end
				3'b010:begin
					if(money==5 && numOfWaterVM1>0) begin 
						itemLeft=numOfWaterVM1-1;
						numOfWaterVM1=numOfWaterVM1-1;
						productReady=1;
						moneyLeft=0;
						end
					else if(numOfWaterVM1==0) begin 
						productUnavailable=1;
						moneyLeft=money;
						end
					else if (money!=5) begin 
						notExactFund=1;
						moneyLeft=money;
						end
					end
				default:begin
					invalidProduct=1;
					moneyLeft=money;
					end
			endcase
			end
		else if(vm==1)
			begin
			case(productID)
				3'b010: begin
				
				
					if(numOfWaterVM2==0) begin
						productUnavailable=1;
						moneyLeft=money;
						end
					else if (sugar==1) begin
							sugarUnsuitable=1;
							moneyLeft=money;
							end
					else if (money<5) begin
						insufficientFund=1;
						moneyLeft=money;
						end
					else if(money>=5 && numOfWaterVM2>0) begin
						 
						itemLeft=numOfWaterVM2-1;
						numOfWaterVM2=numOfWaterVM2-1;
						productReady=1;
						moneyLeft=money-5;
						end
					end
				3'b011: begin
					if(money>=12 && numOfCoffee>0) begin 
						itemLeft=numOfCoffee-1;
						numOfCoffee=numOfCoffee-1;
						moneyLeft=money-12;
						productReady=1;
						end
					else if (numOfCoffee==0) begin
						productUnavailable=1;
						moneyLeft=money;
						end
					else if(money<12) begin
						insufficientFund=1;
						moneyLeft=money;
						end
					end
				3'b100: begin
					if(money>=8 && numOfTea>0) begin
						itemLeft=numOfTea-1;
						numOfTea=numOfTea-1;
						moneyLeft=money-8;
						productReady=1;
						end
					else if(numOfTea==0) begin
						productUnavailable=1;
						moneyLeft=money;
						end
					else if(money<8) begin
						insufficientFund=1;
						moneyLeft=money;
						end
					end
				default: begin
					 invalidProduct=1;
					 moneyLeft=money;
					 end
				endcase
			end
    	// ...

	end
endmodule

