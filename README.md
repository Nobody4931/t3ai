# t3ai
some tic tac toe ai made because i hate losing \
works on all boards of any size and any amount
of markers in a row (at the cost of your cpu)

## dependencies
probably shouldve used static linking instead of just
including the headers but whatever i guess

* [spdlog](https://github.com/gabime/spdlog "spdlog")
* [json](https://github.com/nlohmann/json "json")
* [asio](https://github.com/chriskohlhoff/asio "asio")

## todo
* make it into a discord selfbot to cyberbully
nerds in different servers

## not so frequently asked questions

### how does it work?
it uses the minimax algorithm with alpha beta
pruning to search around 4 moves into the future
and apply a probably broken fitness function on
the resulting boards to choose a path to go down

### can i use this code for research?
no youre better off reading articles or looking
at those resources i put in a comment somewhere
the code goddamn sucks

### why c style naming conventions?
snake_case is the best case, change my mind

### your directory structure is garbage
i literally do not care go away
