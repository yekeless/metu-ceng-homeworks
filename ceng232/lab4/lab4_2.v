`timescale 1ns / 1ps
module SelectionOfAvatar(
	input [1:0] mode,
	input [5:0] userID,
	input [1:0] candidate, // 00:Air 01:Fire, 10:Earth, 11: Water
	input CLK,
	output reg [1:0] ballotBoxId,
	output reg [5:0] numberOfRegisteredVoters,
	output reg [5:0] numberOfVotesWinner, // number of votes of winner
	output reg [1:0] WinnerId,
	output reg AlreadyRegistered,
	output reg AlreadyVoted,
	output reg NotRegistered,
	output reg VotingHasNotStarted,
	output reg RegistrationHasEnded
	);

		 reg [7:0] counter;
		 reg [15:0] box00;
		 reg [15:0] box01;
		 reg [15:0] box10;
		 reg [15:0] box11;
		 reg [15:0] voted_box00;
		 reg [15:0] voted_box01;
		 reg [15:0] voted_box10;
		 reg [15:0] voted_box11;
		 reg [6:0] aday00;
		 reg [6:0] aday01;
		 reg [6:0] aday10;
		 reg [6:0] aday11;
		 reg [3:0] index;
	
	initial begin
		// ...
		 numberOfRegisteredVoters=0;
		 numberOfVotesWinner=0;
		 counter=0;
		 box00=0;
		 box01=0;
		 box10=0;
		 box11=0;
		 voted_box00=0;
		 voted_box01=0;
		 voted_box10=0;
		 voted_box11=0;
		 aday00=0;
		 aday01=0;
		 aday10=0;
		 aday11=0;
		 WinnerId=0;
	end

	always @(posedge CLK)
	begin
		// ...
		AlreadyRegistered=0;
		AlreadyVoted=0;
		NotRegistered=0;
		VotingHasNotStarted=0;
		RegistrationHasEnded=0;
		ballotBoxId=userID[5:4];
		index=userID[3:0];
		if (counter < 100) begin
			if(mode==0) begin
				if(ballotBoxId==0) begin
					if(box00[index]==0) begin
						box00[index]=1;
						numberOfRegisteredVoters=numberOfRegisteredVoters+1;
						end
					else begin
						AlreadyRegistered=1;
						end
					end
				else if(ballotBoxId==1) begin
					if(box01[index]==0) begin
						box01[index]=1;
						numberOfRegisteredVoters=numberOfRegisteredVoters+1;
						end
					else begin
						AlreadyRegistered=1;
						end
					end	
				else if(ballotBoxId==2) begin
					if(box10[index]==0) begin
						box10[index]=1;
						numberOfRegisteredVoters=numberOfRegisteredVoters+1;
						end
					else begin
						AlreadyRegistered=1;
						end
					end
				else if(ballotBoxId==3) begin
					if(box11[index]==0) begin
						box11[index]=1;
						numberOfRegisteredVoters=numberOfRegisteredVoters+1;
						end
					else begin
						AlreadyRegistered=1;
						end
					end
				end	
			else if(mode==1) begin
				VotingHasNotStarted=1;
				end
			counter=counter+1;	
			end
		else if(100<=counter && counter<200) begin
			if(mode==0) begin
				RegistrationHasEnded=1;
				end
			else if(mode==1) begin
				if(ballotBoxId==0) begin
					if(box00[index]==0) begin
						NotRegistered=1;
						end
					else if(box00[index]==1 && voted_box00[index]==0) begin
						voted_box00[index]=1;
						if (candidate==0) begin
							aday00=aday00+1;
							end
						else if (candidate==1) begin
							aday01=aday01+1;
							end
						else if (candidate==2) begin
							aday10=aday10+1;
							end
						else if (candidate==3) begin
							aday11=aday11+1;
							end			
						end
					else if(voted_box00[index]==1) begin
						AlreadyVoted=1;
						end		
					end
				else if(ballotBoxId==1) begin
					if(box01[index]==0) begin
						NotRegistered=1;
						end
					else if(box01[index]==1 && voted_box01[index]==0) begin
						voted_box01[index]=1;
						if (candidate==0) begin
							aday00=aday00+1;
							end
						else if (candidate==1) begin
							aday01=aday01+1;
							end
						else if (candidate==2) begin
							aday10=aday10+1;
							end
						else if (candidate==3) begin
							aday11=aday11+1;
							end			
						end
					else if(voted_box01[index]==1) begin
						AlreadyVoted=1;
						end		
					end
				else if(ballotBoxId==2) begin
					if(box10[index]==0) begin
						NotRegistered=1;
						end
					else if(box10[index]==1 && voted_box10[index]==0) begin
						voted_box10[index]=1;
						if (candidate==0) begin
							aday00=aday00+1;
							end
						else if (candidate==1) begin
							aday01=aday01+1;
							end
						else if (candidate==2) begin
							aday10=aday10+1;
							end
						else if (candidate==3) begin
							aday11=aday11+1;
							end			
						end
					else if(voted_box10[index]==1) begin
						AlreadyVoted=1;
						end		
					end
				else if(ballotBoxId==3) begin
					if(box11[index]==0) begin
						NotRegistered=1;
						end
					else if(box11[index]==1 && voted_box11[index]==0) begin
						voted_box11[index]=1;
						if (candidate==0) begin
							aday00=aday00+1;
							end
						else if (candidate==1) begin
							aday01=aday01+1;
							end
						else if (candidate==2) begin
							aday10=aday10+1;
							end
						else if (candidate==3) begin
							aday11=aday11+1;
							end			
						end
					else if(voted_box11[index]==1) begin
						AlreadyVoted=1;
						end		
					end			
				end
				counter=counter+1;	
			end
			else if(counter>=200) begin
				if(aday00 > aday01 && aday00 > aday10 && aday00 > aday11) begin
		 			WinnerId=0;
		 			numberOfVotesWinner=aday00;
		 			end
		 		else if(aday01 > aday00 && aday01 > aday10 && aday01 > aday11) begin
		 			WinnerId=1;
		 			numberOfVotesWinner=aday01;
		 			end
		 		else if(aday10 > aday00 && aday10 > aday01 && aday10 > aday11) begin
		 			WinnerId=2;
		 			numberOfVotesWinner=aday10;
		 			end
		 		else if(aday11 > aday00 && aday11 > aday01 && aday11 > aday10) begin
		 			WinnerId=3;
		 			numberOfVotesWinner=aday11;
		 			end	
				end								 		
	
	end

endmodule
