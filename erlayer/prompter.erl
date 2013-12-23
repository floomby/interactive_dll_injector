-export([prompt/0]).
-export([prompt/1]).

-define(DEFAULT_PROMPT, "~>").

prompt() ->
	prompt(?DEFAULT_PROMPT).

prompt(P) ->
	case io:fread(P, "~s") of
	{ok, ["quit"]} ->
		exit(quit);
	{ok, [X]} ->
		fw(X),
		prompt(P)
	end.

fw(Input) ->
    [ Nm , Md | Fn ] = string:tokens(Input, ":"),
    case lists:member(list_to_atom(Nm), registered()) of
        true ->
            whereis(list_to_atom(Nm)) ! Md ++ ":" ++ Fn;
        false ->
            io:format("framework node ~p does not exist~n", [Nm])
    end.