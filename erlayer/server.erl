-module(server).

-export([start/0]).

-export([dispatch_loop/1]).
%-export([target_loop/1]).
-export([dis_loop/1]).
-export([dis_worker/2]).
-export([cmd_loop_i/1]).

-include("prompter.erl").

%% TODO parse cmd line opts for port nums
%% TODO the way processes are linked is bad

-define(DISPATCH_PORT, 1234).
-define(TARGET_PORT, 1235).

start() ->
	spawn(?MODULE, dispatch_loop, [?DISPATCH_PORT]).
%	spawn(?MODULE, target_loop, [?TARGET_PORT]).

dispatch_loop(Port) ->
	case gen_tcp:listen(Port, [binary, {reuseaddr, true}, {active, true}, {packet, 0}]) of
	{ok, Sock} ->
		spawn_link(?MODULE, dis_worker, [self(), Sock]),
		dis_loop(Sock);
	Other ->
		io:format("cant listen to socket ~p~n", [Other])
	end.

dis_loop(Sock) ->
	receive
	next_worker ->
		spawn(?MODULE, dis_worker, [self(), Sock])
	end,
	dis_loop(Sock).    
dis_worker(Server, Sock) ->
	case gen_tcp:accept(Sock) of
	{ok, Socket} ->
		Server ! next_worker,
		cmd_loop(Socket);
	{error, Reason} ->
		Server ! next_worker,
		io:format("cant accept socket ~p~n", [Reason])
	end.

cmd_loop(Socket) ->
	spawn_link(?MODULE, prompt, [self()]),
	cmd_loop_i(Socket).

cmd_loop_i(Socket) ->
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
	cmd_loop_i(Socket).

%template code for the injected dll to conncet back
    
%target_loop(Port) ->
%	case gen_tcp:listen(Port, [binary, {reuseaddr, true},
%				{packet, 0}, {active, false}]) of
%	{ok, Sock} ->
%		spawn(?MODULE, tar_worker, [self(), Sock]),
%		tar_loop(Sock);
%	Other ->
%		io:format("cant listen to socket ~p~n", [Other])
%	end.

%tar_loop(Sock) ->
%	receive
%	next_worker ->
%		spawn_link(?MODULE, tar_worker, [self(), Sock])
%	end,
%	dis_loop(Sock).

%tar_worker(Server, Sock) ->
%	case gen_tcp:accept(Sock) of
%	{ok, Socket} ->
%		Server ! next_worker,
%		hello(Socket);
%	{error, Reason} ->
%		Server ! next_worker,
%		io:format("cant accept socket ~p~n", [Reason])
%	end.
