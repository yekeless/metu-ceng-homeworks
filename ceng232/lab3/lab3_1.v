`timescale 1ns / 1ps

module bh(input B, input H, input clk, output reg Q);

    initial begin
        Q = 1;
    end

	// You can implement your code here
	always @(posedge clk) begin
		case({B,H})
			2'b00: Q<= ~Q;
			2'b01: Q<= 1;
			2'b10: Q<= 0;
			2'b11: Q<= Q;
		endcase
	end	
    // ...

endmodule

module ic1337(// Inputs
              input A0,
              input A1,
              input A2,
              input clk,
              // Outputs
              output Q0,
              output Q1,
              output Z);

	// You can implement your code here
	wire Qa,Qb,b1,b2,h1,h2;
	assign b1= (A0 ^ (~A1)) | A2;
	assign b2= A0 & (~A2);
	assign h1= A0 & (~A2);
	assign h2= (~(~A0 | A1)) & A2;
	bh bh1(.B(b1), .H(h1), .clk(clk), .Q(Qa));
	bh bh2(.B(b2), .H(h2), .clk(clk), .Q(Qb));
	assign Q0=Qa;
	assign Q1=Qb;
	assign Z=~(Qa ^ Qb);
	
    // ...

endmodule
