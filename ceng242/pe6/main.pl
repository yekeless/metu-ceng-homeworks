:- module(main, [is_vote_wasted/2, is_candidate_elected/2, candidate_count_from_city/3, all_parties/1, all_candidates_from_party/2, all_elected_from_party/2, election_rate/2, council_percentage/2, alternative_debate_setups/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE



is_vote_wasted(City,PoliticalParty) :- not(elected(City,PoliticalParty,_)).
is_candidate_elected(Name,PoliticalParty) :- candidate(Name, PoliticalParty, City, Row),elected(City, PoliticalParty, ElectedRepresentativeCount),Row=<ElectedRepresentativeCount.
candidate_count_from_city([],_,0).
candidate_count_from_city([H|T] ,GivenCity,Count) :- 
    candidate(H,_,GivenCity,_),
    candidate_count_from_city(T,GivenCity,Remaining),
    Count is (Remaining+1).
candidate_count_from_city([H|T] ,GivenCity,Count) :- 
    \+candidate(H,_,GivenCity,_),
    candidate_count_from_city(T,GivenCity,Count).
all_parties(PartyList) :-
    findall(Name,party(Name,_),List),
    PartyList=List.
all_candidates_from_party(Party,Clist) :-
    findall(C,candidate(C,Party,_,_),List),
    Clist=List.
all_elected_from_party(Party,Elist) :-
    findall(C,is_candidate_elected(C,Party),List),
    Elist=List.
election_rate(PoliticalParty,Percentage) :-
    all_candidates_from_party(PoliticalParty,Clist),
    all_elected_from_party(PoliticalParty,Elist),
    length(Clist,Total),
    length(Elist,Elected),
    Percentage is Elected/Total.
council_percentage(PoliticalParty,Percentage) :-
    all_elected_from_party(PoliticalParty,Elist),
    findall(C,is_candidate_elected(C,_),TotalElected),
    length(TotalElected,Total),
    length(Elist,Elected),
    Percentage is Elected/Total.
    
    