require "./crsfml"
require "./player_controller"

class Building
  include SF::Drawable
  include SF::DebugDrawable

  {% unless flag?(:release) %}

  @debug_vertices_rectangle : Array(SF::Vertex)?

  {% end %}
  
  def initialize(@texture : SF::Texture, position : SF::Vector2f)
    tex_x = @texture.size.x.to_f32
    tex_y = @texture.size.y.to_f32
    
    @transform = SF::Transform.new
    @transform.translate(position)

    @vertices = [
      SF::Vertex.new({0f32, 0f32}, SF::Color::White, {0f32, 0f32}),
      SF::Vertex.new({tex_x, 0f32}, SF::Color::White, {tex_x, 0f32}),
      SF::Vertex.new({tex_x, tex_y}, SF::Color::White, {tex_x, tex_y}),
      SF::Vertex.new({0f32, tex_y}, SF::Color::White, {0f32, tex_y}),
    ]
    
    {% unless flag?(:release) %}

    @debug_vertices_rectangle = [
      SF::Vertex.new({0f32, 0f32}, SF::Color::Red),
      SF::Vertex.new({tex_x, 0f32}, SF::Color::Red),
      SF::Vertex.new({tex_x, tex_y}, SF::Color::Red),
      SF::Vertex.new({0f32, tex_y}, SF::Color::Red),
      SF::Vertex.new({0f32, 0f32}, SF::Color::Red),
    ]

    {% end %}
  end

  def draw(target : SF::RenderTarget, states : SF::RenderStates)
    states.texture = @texture
    states.transform *= @transform

    target.draw(@vertices, SF::Quads, states)

    {% unless flag?(:release) %}

    debug_draw(target, states)

    {% end %}
  end

  def debug_draw(target : SF::RenderTarget, states : SF::RenderStates)
    states.texture = nil

    @debug_vertices_rectangle.try do |d|
      target.draw(d, SF::LinesStrip, states)
    end
  end
end