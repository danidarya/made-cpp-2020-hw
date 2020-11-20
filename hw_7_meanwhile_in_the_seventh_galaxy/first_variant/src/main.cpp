#include <iostream>
#include "game.h"

int main() {
    using game = Game<1, 1, 2, 1>;

    std::cout << (game::first_player_wins ? "First" : "Second") << " player wins." << std::endl;

    if (game::first_player_wins)
        std::cout << "To win, remove " << game::first_move_rocks_count << " from heap " << (game::first_move_heap_index) << std::endl;
    return 0;
}
