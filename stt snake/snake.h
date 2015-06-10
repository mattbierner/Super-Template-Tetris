#pragma once

#include "direction.h"
#include "cell.h"
#include "input.h"
#include "grid.h"
#include "printer.h"
#include "random.h"
#include "serialize.h"
#include "world.h"

/**
    Seed for the random number generator.
*/
template <unsigned max>
using InitialRandom = prandom::PseudoRandomGenerator<max,
    prandom::Lfsr<
        prandom::bitset<true, false, true, false, true, true, false, false, true, true, true, false, false, false, false, true>,
        prandom::indicies<0, 10, 12, 13, 15>>>;

/**
    General state of the player
*/
enum class PlayerState : unsigned
{
    Alive,
    Dead
};

/**
    State of a snake game.
*/
template <
    PlayerState currentPlayerState,
    typename currentPosition,
    Direction currentDirection,
    typename currentWorld,
    typename currentRandom>
struct State
{
    static const PlayerState PlayerState = currentPlayerState;

    using position = currentPosition;
    
    using world = currentWorld;
    
    static const Direction direction = currentDirection;
    
    using random = currentRandom;
    
    template <typename newWorld>
    using set_world = State<PlayerState, position, direction, newWorld, random>;
    
    template <typename newRandom>
    using set_random = State<PlayerState, position, direction, world, newRandom>;
};

/**
    Generate a new, randomly placed food entry.
*/
template <typename state, typename = void>
struct put_food {
    using type = typename put_food<
        typename state::template set_random<typename state::random::next::next>>::type;
};

template <typename state>
struct put_food<state,
    typename std::enable_if<
        is_empty<
            Position<state::random::value, state::random::next::value>,
            typename state::world>
        ::value>::type>
{
    static const size_t targetX = state::random::value;
    static const size_t targetY = state::random::next::value;
    
    using targetPosition = Position<targetX, targetY>;
    
    using type = typename state
        ::template set_world<
            put_grid<
                targetPosition,
                FoodCell,
                typename state::world>>
        ::template set_random<
            typename state::random::next::next>;
};

template <typename state>
using put_food_t = typename put_food<state>::type;


/**
    The initial state of a snake game.
*/
using InitialState =
    put_food_t<
        State<
            PlayerState::Alive,
            Position<worldSize / 2, worldSize / 2>,
            Direction::Right,
            InitialWorld<
                Position<worldSize / 2, worldSize / 2>,
                Direction::Right>,
            InitialRandom<worldSize>>>;

/**
    Progress the snake game by one step by consuming one input.
*/
template <Input input, typename state>
struct step {
    static const Direction direction = get_new_direction<state::direction, input>::value;

    using nextPosition = get_next_position<direction, typename state::position>;
    
    static const unsigned currentWeight = get_weight<typename state::position, typename state::world>::value;
    
    /**
        Case where the snake consumes some food.
        
        This is garunteed not to be a collision.
        
        Skip the decay step and add one to the snake's head.
    */
    struct consume
    {
        using newWorld = grow_snake<currentWeight + 1, direction, nextPosition, typename state::world>;
        
        using type = put_food_t<State<
            PlayerState::Alive,
            nextPosition,
            direction,
            newWorld,
            typename state::random>>;
    };
    
    /**
        Regular gameplay, may be other a collision or
    */
    struct regular
    {
        /**
            Decay the entire grid before performing the next move.
        */
        using decayedWorld = fmap_t<typename state::world, decay>;
        
        /**
            The snake collided with something.
        */
        struct die
        {
            /**
                Mark the collision on the map.
            */
            using newWorld = mark_collision<
                typename std::conditional<is_in_bounds<nextPosition, decayedWorld>::value,
                    nextPosition,
                    typename state::position>::type,
                typename state::world>;
        
            using type = State<
                PlayerState::Dead,
                nextPosition,
                direction,
                newWorld,
                typename state::random>;
        };
        
        /**
            No collision, continue on.
        */
        struct live
        {
            /**
                Regrow the snake by one to advance it.
            */
            using newWorld = grow_snake<
                currentWeight,
                direction,
                nextPosition,
                decayedWorld>;
        
            using type = State<
                PlayerState::Alive,
                nextPosition,
                direction,
                newWorld,
                typename state::random>;
        };
    
        using type = branch_t<is_empty<nextPosition, decayedWorld>::value,
            live,
            die>;
    };
    
    using type = branch_t<
        is_food<nextPosition, typename state::world>::value,
        consume,
        regular>;
};

/**
    For the case where we are dead, noop.
*/
template <
    Input input,
    typename position,
    Direction direction,
    typename world,
    typename random>
struct step<input, State<PlayerState::Dead, position, direction, world, random>> {
    using type = State<PlayerState::Dead, position, direction, world, random>;
};

template <Input input, typename state>
using step_t = typename step<input, state>::type;


/*------------------------------------------------------------------------------
    Printer
*/
template <
    PlayerState playerState,
    typename position,
    Direction direction,
    typename world,
    typename random>
struct Printer<State<playerState, position, direction, world, random>>
{
    static void Print(std::ostream& output)
    {
        output << "--" << (playerState == PlayerState::Dead ? " You Are Dead " : "--------------") << "--" << "\n";
        Printer<world>::Print(output);
    }
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <PlayerState state>
struct Serialize<SerializableValue<PlayerState, state>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "PlayerState::";
        switch (state)
        {
        case PlayerState::Alive: output << "Alive"; break;
        case PlayerState::Dead: output << "Dead"; break;
        }
        return output;
    }
};

template <
    PlayerState playerState,
    typename position,
    Direction direction,
    typename world,
    typename random>
struct Serialize<State<playerState, position, direction, world, random>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "State<";
        Join<',',
            SerializableValue<PlayerState, playerState>,
            position,
            SerializableValue<Direction, direction>,
            world,
            random>::Write(output);
        return output << ">";
    }
};
