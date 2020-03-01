% Alperen Bag
% compiling:  yes
% complete:  yes

:- [pokemon_data].

% Given pokemon is either evolved or not evolved according to its level, result of calculation is assigned to EvolvedPokemon.
% +PokemonLevel, +Pokemon, -EvolvedPokemon
find_pokemon_evolution(PokemonLevel, Pokemon, EvolvedPokemon):-
  (pokemon_evolution(Pokemon, Next, X), PokemonLevel>=X) -> find_pokemon_evolution(PokemonLevel, Next, EvolvedPokemon);
  EvolvedPokemon = Pokemon.

% Given pokemon and level, it finds pokemon's hp, attack and defense values.
% +PokemonLevel, ?Pokemon, -PokemonHp, -PokemonAttack,-PokemonDefense
pokemon_level_stats(PokemonLevel, Pokemon, PokemonHp, PokemonAttack, PokemonDefense):-
  pokemon_stats(Pokemon, _, X, Y, Z),PokemonHp is X+PokemonLevel*2,
  PokemonAttack is Y+PokemonLevel*1, PokemonDefense is Z+PokemonLevel*1.

% Calculates the multiplier between two pokemon types.
% ?AttackerType, +DefenderType, ?Multiplier
single_type_multiplier(AttackerType, DefenderType, Multiplier):-
  pokemon_types(Types), nth0(I, Types, DefenderType),
  type_chart_attack(AttackerType, Multipliers), nth0(I, Multipliers, Multiplier).

% Calculates the multiplier between a single attacker type and two defender types.
% ?AttackerType, +DefenderTypeList, ?Multiplier
type_multiplier(AttackerType, DefenderTypeList, Multiplier):-
  seperate(DefenderTypeOne, DefenderTypeTwo, DefenderTypeList),single_type_multiplier(AttackerType, DefenderTypeOne, MultiplierFirst),
  single_type_multiplier(AttackerType, DefenderTypeTwo, MultiplierSecond), Multiplier is MultiplierFirst*MultiplierSecond.

% Utility predicate. Given a list, it finds its first and second elements.
% -F, -S, +List
seperate(F, S, List):-
  nth0(0, List, F),nth0(1, List, S).

% Utility predicate. Given a list, it finds its first element.
% -F, +List
seperate(F, List):-
  nth0(0, List, F).

% Given two pokemons, it finds the multiplier according to their types. This predicate is for single type attacker and single type defender.
% ?AttackerPokemon, ?DefenderPokemon, ?Multiplier
pokemon_type_multiplier(AttackerPokemon, DefenderPokemon, Multiplier):-
  pokemon_stats(AttackerPokemon, AttackerPokemonTypeList, _, _, _),
  length(AttackerPokemonTypeList,X), X=1,
  seperate(AttackerPokemonType, AttackerPokemonTypeList),
  pokemon_stats(DefenderPokemon, DefenderPokemonTypeList, _, _, _),
  length(DefenderPokemonTypeList,Y), Y=1,
  seperate(DefenderPokemonType, DefenderPokemonTypeList),
  single_type_multiplier(AttackerPokemonType, DefenderPokemonType, Multiplier).

% Given two pokemons, it finds the multiplier according to their types. This predicate is for double type attacker and double type defender.
% ?AttackerPokemon, ?DefenderPokemon, ?Multiplier
pokemon_type_multiplier(AttackerPokemon, DefenderPokemon, Multiplier):-
  pokemon_stats(AttackerPokemon, AttackerPokemonTypes, _, _, _),
  length(AttackerPokemonTypes,X), X = 2,
  pokemon_stats(DefenderPokemon, DefenderPokemonTypes, _, _, _),
  length(DefenderPokemonTypes,Y),Y = 2,
  seperate(FirstAttackerType, SecondAttackerType, AttackerPokemonTypes),
  type_multiplier(FirstAttackerType, DefenderPokemonTypes, MultiplierFirst),
  type_multiplier(SecondAttackerType, DefenderPokemonTypes, MultiplierSecond),
  find_max(MultiplierFirst, MultiplierSecond, Multiplier).

% Given two pokemons, it finds the multiplier according to their types. This predicate is for single type attacker and double type defender.
% ?AttackerPokemon, ?DefenderPokemon, ?Multiplier
pokemon_type_multiplier(AttackerPokemon, DefenderPokemon, Multiplier):-
  pokemon_stats(AttackerPokemon, AttackerPokemonTypeList, _, _, _),
  length(AttackerPokemonTypeList,X),X = 1,
  seperate(AttackerPokemonType, AttackerPokemonTypeList),
  pokemon_stats(DefenderPokemon, DefenderPokemonTypes, _, _, _),
  length(DefenderPokemonTypes,Y),Y = 2,
  type_multiplier(AttackerPokemonType, DefenderPokemonTypes, Multiplier).

% Given two pokemons, it finds the multiplier according to their types. This predicate is for double type attacker and single type defender.
% ?AttackerPokemon, ?DefenderPokemon, ?Multiplier
pokemon_type_multiplier(AttackerPokemon, DefenderPokemon, Multiplier):-
  pokemon_stats(AttackerPokemon, AttackerPokemonTypes, _, _, _),
  length(AttackerPokemonTypes,X),X = 2,
  pokemon_stats(DefenderPokemon, DefenderPokemonTypeList, _, _, _),
  length(DefenderPokemonTypeList,Y),Y = 1,
  seperate(DefenderPokemonType, DefenderPokemonTypeList),
  seperate(FirstAttackerType, SecondAttackerType, AttackerPokemonTypes),
  single_type_multiplier(FirstAttackerType, DefenderPokemonType, MultiplierFirst),
  single_type_multiplier(SecondAttackerType, DefenderPokemonType, MultiplierSecond),
  find_max(MultiplierFirst, MultiplierSecond, Multiplier).

% Finds the maximum of F and S and assigns to Y.
% +F, +S, -Y
find_max(F, S, Y):-
  Y = S, S>F;
  Y = F.

% Calculates the damage dealt by a pokemon to another pokemon according to their levels.
% +AttackerPokemon, +AttackerPokemonLevel, +DefenderPokemon,+DefenderPokemonLevel, -Damage
pokemon_attack(AttackerPokemon, AttackerPokemonLevel, DefenderPokemon, DefenderPokemonLevel, Damage):-
  pokemon_level_stats(AttackerPokemonLevel, AttackerPokemon, _, AttackerPokemonAttack, _),
  pokemon_level_stats(DefenderPokemonLevel, DefenderPokemon, _, _, DefenderPokemonDefense),
  pokemon_type_multiplier(AttackerPokemon, DefenderPokemon, TypeMultiplier),
  Damage is ((0.5 * AttackerPokemonLevel * (AttackerPokemonAttack / DefenderPokemonDefense) * TypeMultiplier) + 1).

% Calculates the RemainingHP values of the two pokemon after a fight between them, also calculates the number of rounds.
% +Pokemon1, +Pokemon1Level, +Pokemon2, +Pokemon2Level,-Pokemon1Hp, -Pokemon2Hp, -Rounds
pokemon_fight(Pokemon1, Pokemon1Level, Pokemon2, Pokemon2Level, Pokemon1Hp, Pokemon2Hp, Rounds):-
  pokemon_level_stats(Pokemon1Level, Pokemon1, InitialPokemon1Hp, _, _),
  pokemon_level_stats(Pokemon2Level, Pokemon2, InitialPokemon2Hp, _, _),
  pokemon_attack(Pokemon1, Pokemon1Level, Pokemon2, Pokemon2Level, Damage1to2),
  pokemon_attack(Pokemon2, Pokemon2Level, Pokemon1, Pokemon1Level, Damage2to1),
  pokemon_fight2(InitialPokemon1Hp, InitialPokemon2Hp, Damage1to2, Damage2to1, 0, Pokemon1Hp, Pokemon2Hp, Rounds).

% Helper predicate for recursive.
pokemon_fight2(CurrentPokemon1Hp, CurrentPokemon2Hp, Damage1to2, Damage2to1, CurrentRound, Pokemon1Hp, Pokemon2Hp, Rounds):-
  CurrentPokemon1Hp > 0, CurrentPokemon2Hp > 0, decrement(CurrentPokemon1Hp, Damage2to1, NextPokemon1Hp),
  decrement(CurrentPokemon2Hp, Damage1to2, NextPokemon2Hp), increment(CurrentRound, NextRound),
  pokemon_fight2(NextPokemon1Hp, NextPokemon2Hp, Damage1to2, Damage2to1, NextRound, Pokemon1Hp, Pokemon2Hp, Rounds).

% Helper predicate in order to finish the recursive.
pokemon_fight2(CurrentPokemon1Hp, CurrentPokemon2Hp, _, _, CurrentRound, Pokemon1Hp, Pokemon2Hp, Rounds):-
  (0 >= CurrentPokemon1Hp; 0 >= CurrentPokemon2Hp),
  Pokemon1Hp is CurrentPokemon1Hp, Pokemon2Hp is CurrentPokemon2Hp, Rounds is CurrentRound,!.

% Increments X by 1 and assigns to Y.
% +X, -Y
increment(X,Y):-
  Y is (X+1).

% Decrements X by A and assigns to Y.
% +X, +A, -Y
decrement(X,A,Y):-
  Y is (X-A).

% Finds the winner after a pokemon tournament between PokemonTrainer1 and PokemonTrainer2.
% +PokemonTrainer1, +PokemonTrainer2, -WinnerTrainerList
pokemon_tournament(PokemonTrainer1, PokemonTrainer2, WinnerTrainerList):-
  pokemon_trainer(PokemonTrainer1, PokemonTeam1, PokemonLevels1),
  pokemon_trainer(PokemonTrainer2, PokemonTeam2, PokemonLevels2),
  pokemon_tournament1(PokemonTeam1, PokemonLevels1, PokemonTeam2, PokemonLevels2, [], WinnerTrainerList, PokemonTrainer1, PokemonTrainer2).

% Helper predicate for recursive. Recursively it takes the first pokemon in PokemonTeam1 and PokemonTeam2,
% after fight it modifies the CurrentWinnerList according to the winner.
pokemon_tournament1(PokemonTeam1, PokemonLevels1, PokemonTeam2, PokemonLevels2, CurrentWinnerList, WinnerTrainerList, PokemonTrainer1, PokemonTrainer2):-
  length(PokemonTeam1, X),  X > 1 -> (
  first(PokemonTeam1, FirstPokemon1, RestPokemon1),first(PokemonTeam2, FirstPokemon2, RestPokemon2),
  first(PokemonLevels1, FirstPokemonLevel1, RestLevel1),first(PokemonLevels2, FirstPokemonLevel2, RestLevel2),
  find_pokemon_evolution(FirstPokemonLevel1, FirstPokemon1, FirstEvolvedPokemon1),
  find_pokemon_evolution(FirstPokemonLevel2, FirstPokemon2, FirstEvolvedPokemon2),
  pokemon_fight(FirstEvolvedPokemon1, FirstPokemonLevel1, FirstEvolvedPokemon2, FirstPokemonLevel2 ,FirstPokemon1Hp, FirstPokemon2Hp, _),
  (
  (FirstPokemon2Hp > FirstPokemon1Hp, append(CurrentWinnerList, [PokemonTrainer2], CurrentWinnerListNext),!);
  (append(CurrentWinnerList, [PokemonTrainer1], CurrentWinnerListNext),!)
  ),
  pokemon_tournament1(RestPokemon1, RestLevel1, RestPokemon2, RestLevel2, CurrentWinnerListNext, WinnerTrainerList, PokemonTrainer1, PokemonTrainer2)
  );
  first(PokemonTeam1, FirstPokemon1, RestPokemon1),first(PokemonTeam2, FirstPokemon2, RestPokemon2),
  first(PokemonLevels1, FirstPokemonLevel1, RestLevel1),first(PokemonLevels2, FirstPokemonLevel2, RestLevel2),
  pokemon_tournament2(FirstPokemon1, FirstPokemonLevel1, FirstPokemon2, FirstPokemonLevel2, CurrentWinnerList, WinnerTrainerList, PokemonTrainer1, PokemonTrainer2).

% Helper predicate in order to finish recursive.
pokemon_tournament2(FirstPokemon1, FirstPokemonLevel1, FirstPokemon2, FirstPokemonLevel2, CurrentWinnerList ,WinnerTrainerList, PokemonTrainer1, PokemonTrainer2):-
  find_pokemon_evolution(FirstPokemonLevel1, FirstPokemon1, FirstEvolvedPokemon1),
  find_pokemon_evolution(FirstPokemonLevel2, FirstPokemon2, FirstEvolvedPokemon2),
  pokemon_fight(FirstEvolvedPokemon1, FirstPokemonLevel1, FirstEvolvedPokemon2, FirstPokemonLevel2 ,FirstPokemon1Hp, FirstPokemon2Hp, _),
  (
  (FirstPokemon2Hp > FirstPokemon1Hp, append(CurrentWinnerList, [PokemonTrainer2], WinnerTrainerList),!);
  (append(CurrentWinnerList, [PokemonTrainer1], WinnerTrainerList),!)
  ).

% Finds the first element and the rest of the list.
first([First | Rest], First, Rest).

% Finds the best pokemon against EnemyPokemon at level LevelCap.
% Firstly it finds the all pokemons, then it calls construct_list.
% +EnemyPokemon, +LevelCap, -RemainingHP, -BestPokemon
best_pokemon(EnemyPokemon, LevelCap, RemainingHP, BestPokemon):-
  findall(X, pokemon_stats(X, _, _, _, _), AllPokemons),
  first(AllPokemons, FirstPokemon, RestPokemons),
  construct_list(FirstPokemon, EnemyPokemon, LevelCap, RestPokemons, [], RemainingHP, BestPokemon,AllPokemons).

% Helper predicate to finish recursion. It makes the calculation only for last element and assigns the results.
construct_list(Pokemon, Enemy, Level, [], List, Result, BestPokemon,AllPokemons):-
  pokemon_fight(Pokemon, Level, Enemy, Level, Pokemon1Hp, _, _),
  append(List,[Pokemon1Hp], NextList),
  max_list(NextList, Result),
  nth0(Index, NextList, Result),
  nth0(Index, AllPokemons, BestPokemon),!.

% Helper predicate for recursion. It calls pokemon_fight and appends the result to the list, then calls itself.
construct_list(Pokemon, Enemy, Level, RestPokemons, List, Result, BestPokemon,AllPokemons):-
  pokemon_fight(Pokemon, Level, Enemy, Level, Pokemon1Hp, _, _),
  append(List,[Pokemon1Hp], NextList),
  first(RestPokemons, FirstPokemon, NextRestPokemons),
  construct_list(FirstPokemon, Enemy, Level, NextRestPokemons, NextList, Result, BestPokemon,AllPokemons).

% Finds the best pokemon team against OpponentTrainer. It sets the team by choosing the best pokemon against each pokemon of the OpponentTrainer.
% +OpponentTrainer, -PokemonTeam
best_pokemon_team(OpponentTrainer, PokemonTeam):-
  pokemon_trainer(OpponentTrainer, OpponentPokemonTeam, OpponentPokemonLevels),
  first(OpponentPokemonTeam, FirstOpponentPokemon, RestOpponentPokemonTeam),
  first(OpponentPokemonLevels, FirstOpponentLevel, RestOpponentPokemonLevels),
  best_pokemon_team2(FirstOpponentPokemon, FirstOpponentLevel, RestOpponentPokemonTeam, RestOpponentPokemonLevels, [], PokemonTeam).

% Helper predicate to finish recursion. It makes the calculation only for last element and assigns the results.
best_pokemon_team2(OpponentPokemon, Level, [], [], List, PokemonTeam):-
  best_pokemon(OpponentPokemon, Level, _, BestPokemon),
  append(List, [BestPokemon], PokemonTeam),!.

% Helper predicate for recursion. It finds the best pokemon against OpponentPokemon, then calls itself recursively with new parameters.
best_pokemon_team2(OpponentPokemon, Level, RestOpponentPokemonTeam, RestOpponentPokemonLevels, List, PokemonTeam):-
  \+length(RestOpponentPokemonTeam) = 0,
  best_pokemon(OpponentPokemon, Level, _, BestPokemon),
  append(List, [BestPokemon], NextList),
  first(RestOpponentPokemonTeam, FirstOpponentPokemon, NextRestOpponentPokemonTeam),
  first(RestOpponentPokemonLevels, FirstOpponentLevel, NextRestOpponentPokemonLevels),
  best_pokemon_team2(FirstOpponentPokemon, FirstOpponentLevel, NextRestOpponentPokemonTeam, NextRestOpponentPokemonLevels, NextList, PokemonTeam).

% Given TypeList, it finds the pokemons that is compatable with the TypeList from InitialPokemonList.
% +TypeList, +InitialPokemonList, -PokemonList
pokemon_types(TypeList, InitialPokemonList, PokemonList):-
  findall(Pokemon, (member(Pokemon, InitialPokemonList), pokemon_types2(TypeList, Pokemon)), PokemonList).

% Helper predicate for recursion.
pokemon_types2([H|TypeListTail], Pokemon):-
  pokemon_stats(Pokemon, PokemonTypeList, _, _, _),
  ((member(H, PokemonTypeList),!); pokemon_types2(TypeListTail, Pokemon)).

% Helper function for generate_pokemon_team. It checks whether Pokemon is liked or not according to the LikedTypes and DislikedTypes.
% For single type Pokemon.
if_liked(Pokemon, LikedTypes, DislikedTypes):-
  pokemon_stats(Pokemon, Types, _, _, _),
  length(Types,X), X=2,
  seperate(FirstType, SecondType, Types),
  (member(FirstType, LikedTypes); member(SecondType, LikedTypes)),
  \+member(FirstType, DislikedTypes), \+member(SecondType, DislikedTypes).

% Helper function for generate_pokemon_team. It checks whether Pokemon is liked or not according to the LikedTypes and DislikedTypes.
% For double type Pokemon.
if_liked(Pokemon, LikedTypes, _):-
  pokemon_stats(Pokemon, Types, _, _, _),
  length(Types,X), X=1,
  seperate(Type, Types),
  member(Type, LikedTypes).

% Generates a team according to LikedTypes and Criterion.
% Firstly it finds all pokemons that is liked and not disliked, then it applies maplist to find health values of the pokemons as a list, then calls generate_team.
% +LikedTypes, +DislikedTypes, +Criterion, +Count, -PokemonTeam
generate_pokemon_team(LikedTypes, DislikedTypes, h, Count, PokemonTeam):-
  findall(X, if_liked(X, LikedTypes, DislikedTypes), Pokemons),
  maplist(find_health, Pokemons, PokemonHealths),
  generate_team(0, Count, Pokemons, PokemonHealths, [], PokemonTeam).

% Generates a team according to LikedTypes and Criterion.
% Firstly it finds all pokemons that is liked and not disliked, then it applies maplist to find attack values of the pokemons as a list, then calls generate_team.
% +LikedTypes, +DislikedTypes, +Criterion, +Count, -PokemonTeam
generate_pokemon_team(LikedTypes, DislikedTypes, a, Count, PokemonTeam):-
  findall(X, if_liked(X, LikedTypes, DislikedTypes), Pokemons),
  maplist(find_attack, Pokemons, PokemonAttacks),
  generate_team(0, Count, Pokemons, PokemonAttacks, [], PokemonTeam).

% Generates a team according to LikedTypes and Criterion.
% Firstly it finds all pokemons that is liked and not disliked, then it applies maplist to find defense values of the pokemons as a list, then calls generate_team.
% +LikedTypes, +DislikedTypes, +Criterion, +Count, -PokemonTeam
generate_pokemon_team(LikedTypes, DislikedTypes, d, Count, PokemonTeam):-
  findall(X, if_liked(X, LikedTypes, DislikedTypes), Pokemons),
  maplist(find_defense, Pokemons, PokemonDefenses),
  generate_team(0, Count, Pokemons, PokemonDefenses, [], PokemonTeam).

% Helper predicate. It calculates the max_value in ValueList, then finds the related pokemon.
% Eventually appends the pokemon to the CurrentTeam list and calls itself.
generate_team(CurrentCount, MaxCount, TeamList, ValueList, CurrentTeam, PokemonTeam):-
  \+CurrentCount = MaxCount,
  max_list(ValueList, MaxValue),
  nth0(MaxIndex, ValueList, MaxValue),
  nth0(MaxIndex, TeamList, BestPokemon),
  pokemon_stats(BestPokemon,_, H,A,D),
  append(CurrentTeam, [[BestPokemon, H, A, D]], NextCurrentTeam),
  nth0(MaxIndex, ValueList, _, NextValueList),
  nth0(MaxIndex, TeamList, _, NextTeamList),
  increment(CurrentCount, NextCount),
  generate_team(NextCount, MaxCount, NextTeamList, NextValueList, NextCurrentTeam, PokemonTeam).

% To finish recursion.
generate_team(CurrentCount, MaxCount, _, _, CurrentTeam, PokemonTeam):-
  CurrentCount = MaxCount,
  PokemonTeam = CurrentTeam.

% Finds the health value of a pokemon. This predicate is used by maplist predicate in generate_pokemon_team.
find_health(Pokemon, Health):-
  pokemon_stats(Pokemon, _, Health, _, _).

% Finds the attack value of a pokemon. This predicate is used by maplist predicate in generate_pokemon_team.
find_attack(Pokemon, Attack):-
  pokemon_stats(Pokemon, _, _, Attack, _).

% Finds the defense value of a pokemon. This predicate is used by maplist predicate in generate_pokemon_team.
find_defense(Pokemon, Defense):-
  pokemon_stats(Pokemon, _, _, _, Defense).
