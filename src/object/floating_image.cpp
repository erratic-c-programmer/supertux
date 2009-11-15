//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <stdexcept>

#include "lisp/lisp.hpp"
#include "math/rect.hpp"
#include "object/floating_image.hpp"
#include "sprite/sprite.hpp"
#include "sprite/sprite_manager.hpp"
#include "supertux/main.hpp"
#include "supertux/resources.hpp"
#include "video/drawing_context.hpp"


FloatingImage::FloatingImage(const std::string& spritefile)
  : layer(LAYER_FOREGROUND1 + 1), visible(false), anchor(ANCHOR_MIDDLE), fading(0), fadetime(0)
{
  sprite.reset(sprite_manager->create(spritefile));
}

FloatingImage::~FloatingImage()
{
}

void
FloatingImage::update(float elapsed_time)
{
  if(fading > 0) {
    fading -= elapsed_time;
    if(fading <= 0) {
      fading = 0;
      visible = true;
    }
  } else if(fading < 0) {
    fading += elapsed_time;
    if(fading >= 0) {
      fading = 0;
      visible = false;
    }
  }

//  (void) elapsed_time;
}

void
FloatingImage::set_action(const std::string& action)
{
  sprite->set_action(action);
}

std::string
FloatingImage::get_action()
{
  return sprite->get_action();
}

void
FloatingImage::fade_in(float fadetime)
{
  this->fadetime = fadetime;
  fading = fadetime;
}

void
FloatingImage::fade_out(float fadetime)
{
  this->fadetime = fadetime;
  fading = -fadetime;
}


void
FloatingImage::draw(DrawingContext& context)
{
  context.push_transform();
  context.set_translation(Vector(0, 0));

  if(fading > 0) {
    context.set_alpha((fadetime-fading) / fadetime);
  } else if(fading < 0) {
    context.set_alpha(-fading / fadetime);
  } else if(!visible) {
    context.pop_transform();
    return;
  }

  Vector spos = pos + get_anchor_pos(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
      sprite->get_width(), sprite->get_height(), anchor);

  sprite->draw(context, spos, layer);

  context.pop_transform();
}
