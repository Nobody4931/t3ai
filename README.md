# t3ai
some tic tac toe ai made because i hate losing \
works on all boards of any size and any amount
of markers in a row (at the cost of your cpu)

## dependencies
* [spdlog](https://github.com/gabime/spdlog "spdlog")
* [json](https://github.com/nlohmann/json "json")

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
