%% fixme: removing all spaces is a poor fix
%% I think that I will do strings in quotes

-export([prompt/1]).
-export([prompt/2]).

-define(DEFAULT_PROMPT, "~>").

prompt(Pid) ->
	prompt(Pid, ?DEFAULT_PROMPT).

prompt(Pid, P) ->
	case io:fread(P, "~s") of
	{ok, ["quit"]} ->
		exit(quit);
	{ok, [X]} ->
        %fixme: get rid of all the spcases
		Pid ! X -- " ",
		prompt(Pid, P)
	end.