#pragma once

#include "block_generator.h"
#include "buffer.h"
#include "input.h"
#include "grid.h"
#include "random.h"
#include "serialize.h"
#include "playfield.h"

/**
    Number of game steps to delay before a block is automatically placed.
    
    Movement resets the delay.
*/
static constexpr const size_t standardDelay = 1;

/**
    General state of the player.
*/
enum class PlayerState : unsigned {
    Alive,
    Dead
};

/**
    State of the game.
*/
template <
    PlayerState currentPlayerState,
    unsigned currentScore,
    size_t currentDelay,
    typename currentPosition,
    typename currentBlock,
    typename currentWorld,
    typename currentBlockGenerator>
struct State {
    static constexpr const PlayerState playerState = currentPlayerState;
    static constexpr const unsigned score = currentScore;
    
    static constexpr const size_t delay = currentDelay;
    using position = currentPosition;
    using block = currentBlock;
    using world = currentWorld;
    
    using random = currentBlockGenerator;
    using nextBlock = typename currentBlockGenerator::next::value;

    /**
        Is the block currently colliding with any pieces in the playfield?
    */
    static constexpr const bool is_collision =
        playfield_is_colliding<
            position,
            typename block::pieces,
            world>;
    
    template <typename newPos>
    using set_position = State<playerState, score, delay, newPos, block, world, random>;
    
    template <typename newWorld>
    using set_world = State<playerState, score, delay, position, block, newWorld, random>;
    
    template <typename newRandom>
    using set_random = State<playerState, score, delay, position, block, world, newRandom>;
    
    template <typename newBlock>
    using set_block = State<playerState, score, delay, position, newBlock, world, random>;
    
    template <unsigned newScore>
    using set_score = State<playerState, newScore, delay, position, block, world, random>;
    
    template <size_t newDelay>
    using set_delay = State<playerState, score, newDelay, position, block, world, random>;
    
    using reset_delay = set_delay<0>;
    using inc_delay = set_delay<delay + 1>;
    
    using set_game_over = State<PlayerState::Dead, score, delay, position, block, world, random>;
};

/**
    Create a new block at the top of the game world.
*/
template <typename s>
using place_initial_piece =
    typename s
        ::template set_position<
            Position<
                (s::world::width / 2) - (s::nextBlock::pieces::width / 2),
                0>>
        ::template set_block<typename s::nextBlock>
        ::template set_random<typename s::random::next>;

/**
    The initial state of a game.
*/
using initialState =
    place_initial_piece<
        State<
            PlayerState::Alive,
            0,
            0,
            Position<0, 0>,
            typename initialBlockGenerator::value,
            InitialWorld,
            initialBlockGenerator>>;

/**
    Place the current piece in the world and generate a new piece.
*/
template <typename s>
using place_piece =
    place_initial_piece<
        typename s::template set_world<
            buffer_draw_grid<
                typename s::position,
                typename s::block::pieces,
                typename s::world>>>;

/**
    Move the current piece at most `max` times down or until it collides with 
    something.
*/
template <size_t max, typename state>
struct Drop {
    using next = typename state::template set_position<typename state::position::template add<Position<0, 1>>>;
    
    struct NoCollision {
        using type = typename Drop<max - 1, next>::type;
    };
    
    using type = branch<next::is_collision, identity<state>, NoCollision>;
};

template <typename state>
struct Drop<0, state> {
    using type = state;
};

/**
    Hard drop the current piece but do not place it.
*/
template <typename state>
using hard_drop = typename Drop<static_cast<size_t>(-1), state>::type;

/**
    Attempt to move the block without checking for any collisions.
*/
template <Input input, typename state>
struct move_block {
    using type = typename state::inc_delay;
};

template <typename state>
struct move_block<Input::Left, state> {
    using type = typename state
        ::template set_position<typename state::position::template add<Position<-1, 0>>>
        ::reset_delay;
};

template <typename state>
struct move_block<Input::Right, state> {
    using type = typename state
        ::template set_position<typename state::position::template add<Position<1, 0>>>
        ::reset_delay;
};

template <typename state>
struct move_block<Input::RRot, state> {
    using type = typename state
        ::template set_block<typename state::block::rotateCw>
        ::reset_delay;
};

template <typename state>
struct move_block<Input::LRot, state> {
    using type = typename state
        ::template set_block<typename state::block::rotateCcw>
        ::reset_delay;
};

/**
    Move the block in response to player input.
    
    Happens before gravity is applied.
*/
template <Input input, typename state>
struct move {
    using next = typename move_block<input, state>::type;
    using type = std::conditional_t<next::is_collision, state, next>;
};

template <typename state>
struct move<Input::Hard, state> {
    using type = typename place_piece<hard_drop<state>>::reset_delay;
};

template <typename state>
struct move<Input::Soft, state> {
    using type = typename hard_drop<state>::reset_delay;
};

template <typename state>
struct move<Input::Down, state> {
    using type = typename Drop<4, state>::type::reset_delay;
};

/**
    Progress the game by one step by consuming one input.
*/
template <Input input, typename state>
struct step {
    /**
        Apply gravity to the current piece.
        
        If the delay is reached, place the current piece, otherwise keep it alive.
    */
    template <typename s>
    struct TryPlaceCollisionPiece {
         using type =
            std::conditional_t<(s::delay >= standardDelay),
                typename place_piece<s>::reset_delay,
                s>;
    };
    
    template <typename s,
        typename gnext = typename s::template set_position<typename s::position::template add<Position<0, 1>>>>
    using apply_gravity =
        branch<gnext::is_collision,
            TryPlaceCollisionPiece<s>,
            identity<gnext>>;
    
    /**
        Update the score based on the number of rows removed.
    */
    template <size_t rowsRemoved, typename s>
    using update_score =
        typename s::template set_score<
            s::score +
                (rowsRemoved == 1
                    ?40
                :rowsRemoved == 2
                    ?100
                :rowsRemoved == 3
                    ?300
                :rowsRemoved == 4
                    ?1200
                    :0)>;
    
    /**
        Collapse all full rows and update score.
    */
    struct RemoveFullRow {
        template <typename p, typename c>
        using apply = identity<
            typename p::template set_world<
                grid_cons_row<
                    gen<p::world::width, empty_pixel>,
                    grid_remove_row<
                        c::value,
                        typename p::world>>>>;
    };
    
    template <typename s,
        typename fullRows = playfield_get_full_rows<typename s::world>>
    using update_full_rows =
        update_score<
            fullRows::size,
            fold<RemoveFullRow, s, fullRows>>;
    
    /**
        Check if the player has lost.
    */
    template <typename s>
    using check_game_over =
        std::conditional_t<
            playfield_is_colliding<
                Position<0, 0>,
                gen_grid<s::world::width, deathZoneHeight, o_cell>,
                typename s::world>,
            typename s::set_game_over,
            s>;

    using type =
        check_game_over<
        update_full_rows<
        apply_gravity<typename move<input, state>::type>>>;
};

/**
    For the case where we are dead, noop.
*/
template <
    Input input,
    unsigned score,
    size_t delay,
    typename... state>
struct step<input, State<PlayerState::Dead, score, delay, state...>> {
    using type = State<PlayerState::Dead, score, delay, state...>;
};

template <Input input, typename state>
using step_t = typename step<input, state>::type;

/*------------------------------------------------------------------------------
    Printer
*/
template <
    PlayerState playerState,
    unsigned score,
    size_t delay,
    typename position,
    typename block,
    typename world,
    typename blockGenerator>
struct ToString<
    State<playerState, score, delay, position, block, world, blockGenerator>>
{
    using self = State<playerState, score, delay, position, block, world, blockGenerator>;
    
    static constexpr const size_t uiSize = 10;
    
    // Draw outline
    using initial_buffer = buffer_draw_rect_outline<
        Position<0, 0>,
        Size<world::width + 2, world::height + 2>,
        Pixel<'+', default_gfx>,
        empty_buffer<world::width + 2 + uiSize, world::height + 2>>;
    
    // Draw next block
    using next_block = buffer_draw_grid<
        Position<world::width + 2 + 2, 2>,
        typename self::nextBlock::pieces,
        initial_buffer>;
    
    // Draw Score
    using score_buffer =
        buffer_draw_centered_text<
            Position<world::width + 2, 7>,
            Orientation::Horizontal,
            uiSize,
            std::conditional_t<playerState == PlayerState::Dead,
                decltype("GameOver"_string),
                String<>>,
            default_gfx,
        buffer_draw_centered_text<
            Position<world::width + 2, 8>,
            Orientation::Horizontal,
            uiSize,
            decltype("Score"_string),
            default_gfx,
        buffer_draw_centered_text<
            Position<world::width + 2, 10>,
            Orientation::Horizontal,
            uiSize,
            int_to_string<score>,
            default_gfx,
            next_block>>>;
    
    // Draw death area
    using death_buffer = buffer_draw_rect<
        Position<1, 1>,
        Size<world::width, deathZoneHeight>,
        Pixel<'-', default_gfx>,
        score_buffer>;
    
    // draw playfield
    using play_buffer = buffer_draw_grid<
        Position<1, 1>,
        world,
        death_buffer>;

    // Draw ghost
    using ghost_buffer = buffer_draw_grid<
        Position<1, 1>::add<typename hard_drop<self>::position>,
        typename hard_drop<self>::block::as_ghost_piece,
        play_buffer>;
    
    // Draw current block
    using buffer = buffer_draw_grid<
        Position<1, 1>::add<position>,
        typename block::pieces,
        ghost_buffer>;

    using type = to_string<buffer>;
};

/*------------------------------------------------------------------------------
    Serialize
*/
template <PlayerState x>
struct Serialize<SerializableValue<PlayerState, x>> {
    using type =
        serialize_enum<decltype("PlayerState"_string), PlayerState, x>;
};

template <
    PlayerState playerState,
    unsigned score,
    size_t delay,
    typename position,
    typename block,
    typename world,
    typename blockGenerator>
struct Serialize<State<playerState, score, delay, position, block, world, blockGenerator>> {
    using type =
        serialize_class<decltype("State"_string),
            SerializableValue<PlayerState, playerState>,
            SerializableValue<unsigned, score>,
            SerializableValue<size_t, delay>,
            position,
            block,
            world,
            blockGenerator>;
};
