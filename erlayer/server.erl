-module(server).

-export([start/0]).
-export([worker/2]).
-export([listener/1]).
-export([loop/1]).


-include("prompter.erl").

%% TODO parse cmd line opts for port nums

-define(PORT, 1234).

start() ->
	spawn_link(?MODULE, listener, [?PORT]),
    prompt().

listener(Port) ->
	case gen_tcp:listen(Port, [binary, {reuseaddr, true}, {active, true}, {packet, 0}]) of
	{ok, Sock} ->
		spawn(?MODULE, worker, [self(), Sock]),
		loop(Sock);
	Other ->
		io:format("cant listen to socket ~p~n", [Other])
	end.

loop(Sock) ->
	receive
	next_worker ->
		spawn(?MODULE, worker, [self(), Sock])
	end,
	loop(Sock).
   
%this function needs to register the framework node   
worker(Server, Sock) ->
	case gen_tcp:accept(Sock) of
	{ok, Socket} ->
		Server ! next_worker,
		fw_register(Socket);
	{error, Reason} ->
		Server ! next_worker,
		io:format("cant accept socket ~p~n", [Reason])
	end.

% fixme: this function has a serious (but saved by erlang runtime errors) problem
fw_register(Socket) ->
    receive
        {tcp, Socket, Data} ->
            Name = binary_to_list(Data),
            Registered = registered(),
            case list_to_atom(Name) of
            Registered -> 
                io:format("error: framework node ~p already registered ~n", [Name]),
                case gen_tcp:send(Socket, "error: " ++ Name ++ " already registered") of
                    {error, Reason} -> exit(Reason);
                    ok -> ok
                end,
                gen_tcp:close(Socket);
            _ ->
                register(list_to_atom(Name), self()),
                io:format("framework node ~p registered ~n", [Name]),
                case gen_tcp:send(Socket, "registered as " ++ Name) of
                    {error, Reason} -> exit(Reason);
                    ok -> ok
                end
            end;
        {tcp_closed, Socket} ->
            io:format("closed~n", []),
            exit(closed)
	end,
    work_loop(Socket).

work_loop(Socket) ->
	inet:setopts(Socket, [{active, once}]),
	receive
        {tcp, Socket, Data} ->
            io:format("~p~n", [Data]);
        {tcp_closed, Socket} ->
            io:format("closed~n", []),
            exit(closed);
        Msg ->
            case gen_tcp:send(Socket, Msg) of
                {error, Reason} -> exit(Reason);
                ok -> ok
            end
	end,
	work_loop(Socket).