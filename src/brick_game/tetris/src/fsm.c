#include "../../../include/fsm.h"

#include <string.h>

#include "../../../include/brick_frontend.h"
#include "../include/tetris_backend.h"

void user_input(user_action_t *action, int user_input, bool hold) {
  *action = Nosig;

  if (user_input == NO_INPUT) {
    *action = Nosig;
  } else if (user_input == 'p' || user_input == 'P') {
    *action = Pause;
  } else if (user_input == KEY_DOWN) {
    *action = hold ? Hard_drop : Down;
  } else if (user_input == KEY_LEFT) {
    *action = Left;
  } else if (user_input == KEY_RIGHT) {
    *action = Right;
  } else if (user_input == ESCAPE || user_input == 'q' || user_input == 'Q') {
    *action = Terminate;
  } else if (user_input == SPACE) {
    *action = Action;
  } else if (user_input == ENTER_KEY) {
    *action = Start;
  }
}

void move_left(field_info_t *field) {
  if (!check_collide(field->figures_placed, field->curr.pos.x - 1,
                     field->curr.pos.y, field->curr.matrix))
    field->curr.pos.x--;
}

void move_right(field_info_t *field) {
  if (!check_collide(field->figures_placed, field->curr.pos.x + 1,
                     field->curr.pos.y, field->curr.matrix))
    field->curr.pos.x++;
}

void move_down(field_info_t *field, brick_state *state) {
  if (!check_collide(field->figures_placed, field->curr.pos.x,
                     field->curr.pos.y + 1, field->curr.matrix)) {
    field->curr.pos.y++;
  } else {
    *state = ATTACH;
  }
}

void hard_drop(field_info_t *figure, brick_state *state) {
  while (figure_dropping(figure)) {
    // *state = MOVING;
  }

  *state = ATTACH;
}

void rotate(field_info_t *figure) {
  if (!is_square(figure->curr.type)) {
    int result[SHAPE_H][SHAPE_W];

    transpose_matrix(figure->curr.matrix, result);

    if (!check_collide(figure->figures_placed, figure->curr.pos.x,
                       figure->curr.pos.y, result)) {
      memcpy(figure->curr.matrix, result, sizeof(result));
    }
  }
}

void on_start_state_tetris(user_action_t sig, brick_state *state) {
  switch (sig) {
    case Start:
      hint_msg();
      *state = SPAWN;
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      *state = START;
      start_msg();
      break;
  }
}

void spawn_figure(field_info_t *field, brick_state *state) {
  field->curr.type = field->next.type;

  init_figures(&field->curr, &field->next);

  if (check_collide(field->figures_placed, field->curr.pos.x, field->curr.pos.y,
                    field->curr.matrix)) {
    *state = GAMEOVER;
  } else {
    *state = MOVING;
  }
}

void on_spawn_state_tetris(field_info_t *figure, brick_state *state) {
  spawn_figure(figure, state);
}

void on_moving_state_tetris(brick_stats_t *stats, field_info_t *figure,
                            user_action_t sig, brick_state *state) {
  switch (sig) {
    case Left:
      move_left(figure);
      break;
    case Right:
      move_right(figure);
      break;
    case Down:
      move_down(figure, state);
      break;
    case Hard_drop:
      hard_drop(figure, state);
      break;
    case Action:
      rotate(figure);
      break;
    case Pause:
      *state = PAUSED;
      break;
    case Terminate:
      *state = EXIT;
      break;
    case Nosig:
      *state = SHIFTING;
      break;
    default:
      break;
  }

  render_game(stats, figure, state);
}

void on_shifting_state_tetris(field_info_t *figure, brick_state *state) {
  if (figure_dropping(figure)) {
    *state = MOVING;
  } else {
    *state = ATTACH;
  }
}

void on_attach_state_tetris(brick_stats_t *stats, field_info_t *figure,
                            brick_state *state) {
  lock_figure(figure);
  clear_lines(stats, figure, state);

  if (stats->won) {
    *state = WIN;
  } else {
    *state = SPAWN;
  }
}

void on_paused_state_tetris(user_action_t sig, brick_state *state) {
  switch (sig) {
    case Pause:
      if (*state == PAUSED) {
        *state = MOVING;
      }
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      pause_msg();
      break;
  }
}

void update_current_state(brick_stats_t *stats, field_info_t *field,
                          brick_state *state) {
  tetris_stats_init(stats, state);
  init_tetris_game(field);
}

void on_gameover_state_tetris(brick_stats_t *stats, field_info_t *field,
                              user_action_t sig, brick_state *state) {
  switch (sig) {
    case Start:
      update_current_state(stats, field, state);
      *state = START;
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      gameover_msg();
      start_msg();
      break;
  }
}

void on_win_state_tetris(brick_stats_t *stats, field_info_t *field,
                         user_action_t sig, brick_state *state) {
  switch (sig) {
    case Start:
      update_current_state(stats, field, state);
      *state = START;
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      winning_msg();
      start_msg();
      break;
  }
}

void on_exit_state_tetris(brick_stats_t *stats, field_info_t *game,
                          const brick_state *state) {
  render_game(stats, game, state);
  exit_msg();
}

void sigact_tetris(brick_stats_t *stats, field_info_t *filed, user_action_t sig,
                   brick_state *state) {
  switch (*state) {
    case START:
      on_start_state_tetris(sig, state);
      break;
    case SPAWN:
      on_spawn_state_tetris(filed, state);
      break;
    case MOVING:
      on_moving_state_tetris(stats, filed, sig, state);
      break;
    case SHIFTING:
      on_shifting_state_tetris(filed, state);
      break;
    case ATTACH:
      on_attach_state_tetris(stats, filed, state);
      break;
    case PAUSED:
      on_paused_state_tetris(sig, state);
      break;
    case GAMEOVER:
      on_gameover_state_tetris(stats, filed, sig, state);
      break;
    case WIN:
      on_win_state_tetris(stats, filed, sig, state);
      break;
    case EXIT:
      on_exit_state_tetris(stats, filed, state);
      break;
    default:
      render_game(stats, filed, state);
      break;
  }
}