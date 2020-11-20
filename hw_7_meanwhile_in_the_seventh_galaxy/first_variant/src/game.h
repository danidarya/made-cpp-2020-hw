#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H

#include "binary.h"
#include "xor_sum.h"

template<size_t... Heaps>
struct Game;

template<size_t high_bit, size_t Head, size_t... Tail>
struct heap_number;

template<size_t high_bit, size_t Head, size_t... Tail>
struct heap_number {
    static constexpr bool bit = GetBit<high_bit, Head>::value;
    static constexpr ssize_t number = (heap_number<high_bit, Head>::number * bit) + !bit * (1 + heap_number<high_bit, Tail...>::number);
    static constexpr ssize_t rock_number = (heap_number<high_bit, Head>::rock_number * bit) + ((!bit) *
                                                                                               heap_number<high_bit, Tail...>::rock_number);
};

template<size_t high_bit, size_t Head>
struct heap_number<high_bit, Head> {
    static constexpr bool bit = GetBit<high_bit, Head>::value;
    static constexpr ssize_t number = bit;
    static constexpr ssize_t rock_number = bit * Head;
};

template<bool first_player_wins, size_t high_bit, size_t... Heaps>
struct final_res {
};

template<size_t high_bit, size_t... Heaps>
struct final_res<false, high_bit, Heaps...> {
    static constexpr ssize_t first_move_heap_index = -1;
    static constexpr ssize_t first_move_rocks_count = -1;
    static constexpr ssize_t rock_number = 0;
};

template<size_t high_bit, size_t... Heaps>
struct final_res<true, high_bit, Heaps...> {
    static constexpr ssize_t first_move_heap_index = heap_number<high_bit, Heaps...>::number;
    static constexpr ssize_t rock_number = heap_number<high_bit, Heaps...>::rock_number;
    static constexpr ssize_t first_move_rocks_count = rock_number - (XorSum<Heaps...>::value ^ rock_number);
};


template<size_t... Heaps>
struct Game {
    static constexpr size_t xor_sum = XorSum<Heaps...>::value;
    static constexpr bool first_player_wins = xor_sum;
    static constexpr ssize_t high_bit = Log2<xor_sum>::value;
    static constexpr ssize_t first_move_heap_index = final_res<first_player_wins, high_bit, Heaps...>::first_move_heap_index;
    static constexpr ssize_t first_move_rocks_count = final_res<first_player_wins, high_bit, Heaps...>::first_move_rocks_count;
};


#endif/// MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H.
