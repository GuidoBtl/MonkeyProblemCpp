# Monkey Problem C++
*Solution in C++ to a concurrency problem (monkeys crossing a canyon in both directions)*

We want to know if African monkeys can be taught about deadlocks. She locates a deep canyon and fastens a rope across it, so the monkeys can cross hand-over-hand.

Passage along the rope follows these rules:
* Several monkeys can cross at the same time, provided that they are all going in the same direction.
* If eastward moving and westward moving monkeys ever get onto the rope at the same time, a deadlock will result (the monkeys will get stuck in the middle) because it is impossible for one monkey to climb over another one while suspended over the canyon.
* If a monkey wants to cross the canyon, he must check to see that no other monkey is currently crossing in the opposite direction.

Your solution should avoid starvation. When a monkey that wants to cross to the east arrives at the rope and finds monkeys crossing to the west, the monkey waits until the rope in empty, but no more westward moving monkeys are allowed to start until at least one monkey has crossed the other way.

For this exercise, you are to write a program to simulate activity for this canyon crossing problem:

* Simulate each monkey as a separate process.
* Altogether, a lot of monkeys, whatever you want, will cross the canyon, with a random number generator specifying whether they are eastward moving or westward moving.
* Use a random number generator, so the time between monkey arrivals is between 1 and 8 seconds.
* Each monkey takes 1 second to get on the rope. (That is, the minimum inter-monkey spacing is 1 second.)
* All monkeys travel at the same speed. Each traversal takes exactly 4 seconds, after the monkey is on the rope.

Enjoy it!

## Testing this solution

Since this is just an exercise, all the source code is in a single file to ease browsing and compilation.

* To compile on Linux or similar OS: **gcc monkey.cpp -o monkey -lstdc++** (or **g++ monkey.cpp -o monkey -lstdc++**)

* To run (e.g. with 20 monkeys): **./monkey 20**

## Run example

```
$ ./monkey 10

MONKEY PROBLEM

Symbols description:

TIM     = TIMe in seconds from the beginning
Q       = west and east Queue semaphores
D       = west and east Direction semaphores
ROPE    = the ROPE, of course
MESSAGE = monkey state MESSAGE
>       = eastward going monkey
<       = westward going monkey

TIM:   Q D  ROPE  D Q   MESSAGE

000:   | |  ----  | |<  Monkey  1 arrived to the queue.
000:   | |  ----  |<|   Monkey  1 is ready to cross.
000:   | |  ---- <| |   Monkey  1 is going to the rope.
001:  >| |  ----  | |   Monkey  2 arrived to the queue.
001:   |>|  ----  | |   Monkey  2 is ready to cross.
001:   | |  ---<  | |   Monkey  1 is on the rope crossing the canyon.
001:  >| |  ----  | |   Monkey  3 arrived to the queue.
001:  >| |  ----  | |   Monkey  4 arrived to the queue.
001:  >| |  ----  | |   Monkey  5 arrived to the queue.
001:  >| |  ----  | |   Monkey  6 arrived to the queue.
001:  >| |  ----  | |   Monkey  7 arrived to the queue.
005:   | | <----  | |   Monkey  1 finished crossing the canyon.
005:   | |> ----  | |   Monkey  2 is going to the rope.
006:   | |  >---  | |   Monkey  2 is on the rope crossing the canyon.
006:   |>|  ----  | |   Monkey  3 is ready to cross.
006:   | |> ----  | |   Monkey  3 is going to the rope.
007:   | |  >---  | |   Monkey  3 is on the rope crossing the canyon.
007:   |>|  ----  | |   Monkey  4 is ready to cross.
007:   | |> ----  | |   Monkey  4 is going to the rope.
008:   | |  >---  | |   Monkey  4 is on the rope crossing the canyon.
008:   |>|  ----  | |   Monkey  5 is ready to cross.
008:   | |> ----  | |   Monkey  5 is going to the rope.
009:   | |  >---  | |   Monkey  5 is on the rope crossing the canyon.
009:   |>|  ----  | |   Monkey  6 is ready to cross.
009:   | |> ----  | |   Monkey  6 is going to the rope.
009:   | |  ----  | |<  Monkey  8 arrived to the queue.
009:   | |  ----  |<|   Monkey  8 is ready to cross.
009:   | |  ----  | |<  Monkey  9 arrived to the queue.
009:   | |  ----  | |<  Monkey 10 arrived to the queue.
010:   | |  >---  | |   Monkey  6 is on the rope crossing the canyon.
010:   | |  ----> | |   Monkey  2 finished crossing the canyon.
010:   |>|  ----  | |   Monkey  7 is ready to cross.
011:   | |  ----> | |   Monkey  3 finished crossing the canyon.
012:   | |  ----> | |   Monkey  4 finished crossing the canyon.
013:   | |  ----> | |   Monkey  5 finished crossing the canyon.
014:   | |  ----> | |   Monkey  6 finished crossing the canyon.
014:   | |  ---- <| |   Monkey  8 is going to the rope.
015:   | |  ---<  | |   Monkey  8 is on the rope crossing the canyon.
015:   | |  ----  |<|   Monkey  9 is ready to cross.
019:   | | <----  | |   Monkey  8 finished crossing the canyon.
019:   | |> ----  | |   Monkey  7 is going to the rope.
020:   | |  >---  | |   Monkey  7 is on the rope crossing the canyon.
024:   | |  ----> | |   Monkey  7 finished crossing the canyon.
024:   | |  ---- <| |   Monkey  9 is going to the rope.
025:   | |  ---<  | |   Monkey  9 is on the rope crossing the canyon.
025:   | |  ----  |<|   Monkey 10 is ready to cross.
025:   | |  ---- <| |   Monkey 10 is going to the rope.
026:   | |  ---<  | |   Monkey 10 is on the rope crossing the canyon.
029:   | | <----  | |   Monkey  9 finished crossing the canyon.
030:   | | <----  | |   Monkey 10 finished crossing the canyon.
030:Parent process: last child terminated.
Parent process: goodbye.
```
