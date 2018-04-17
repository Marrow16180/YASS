require "./crsfml"
require "./ship"

class Hud
  include SF::Drawable

  def initialize(font : SF::Font, @ship : Ship)
    @velocity_info = SF::Text.new("[#{@ship.velocity.x.round(2)},#{@ship.velocity.y.round(2)}]", font)
    @velocity_info.position = {10f32, 10f32}
  end

  def update(delta : SF::Time)
    @velocity_info.string = "[x: #{@ship.velocity.x.round(2)}, y: #{@ship.velocity.y.round(2)}]"
  end

  def draw(target : SF::RenderTarget, states : SF::RenderStates)
    states.transform.translate(target.map_pixel_to_coords(@velocity_info.position))
    target.draw(@velocity_info, states)
  end
end