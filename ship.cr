require "./crsfml"
require "./player_controller"

# TODO: move it somewhere else
module SF::DebugDrawable
  abstract def debug_draw(target : RenderTarget, states : RenderStates)
end

class Ship < SF::Transformable
  include SF::Drawable
  include SF::DebugDrawable

  getter velocity

  {% unless flag?(:release) %}

  @debug_vertices_rectangle : Array(SF::Vertex)?
  @debug_vertices_circle : Array(SF::Vertex)?

  {% end %}
  
  def initialize(@texture : SF::Texture, @linear_acc : Float32, @angular_acc : Float32)
    super()
    tex_x = @texture.size.x.to_f32
    tex_y = @texture.size.y.to_f32
    
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

    radius = Math.sqrt((tex_x / 2f32) * (tex_x / 2f32) + (tex_y / 2f32) * (tex_y / 2f32)) * 0.75f32
    center = SF::Vector2f.new(tex_x * 0.5f32, tex_y * 0.5f32)

    @debug_vertices_circle = Array(SF::Vertex).new(31)
    31.times do |n|
      angle = 12f32 * n / 180f32 * Math::PI
      dx = center.x + Math.cos(angle) * radius
      dy = center.y + Math.sin(angle) * radius
      @debug_vertices_circle.try do |d|
        d << SF::Vertex.new({dx, dy}, SF::Color::Green)
      end
    end

    {% end %}

    @velocity = SF::Vector2f.new(0f32, 0f32)
    
    center_origin(tex_x, tex_y)
  end

  def accelerate(delta : SF::Time)
    angle = self.rotation / 180f32 * Math::PI
    dx = Math.cos(angle) * @linear_acc * delta.as_seconds()
    dy = Math.sin(angle) * @linear_acc * delta.as_seconds()
    @velocity += {dx, dy}
  end

  def decelerate(delta : SF::Time)
    angle = self.rotation / 180f32 * Math::PI
    dx = Math.cos(angle) * @linear_acc * delta.as_seconds()
    dy = Math.sin(angle) * @linear_acc * delta.as_seconds()
    @velocity -= {dx, dy}
  end

  def turn_left(delta : SF::Time)
    self.rotate(-@angular_acc * delta.as_seconds())
    angle = Math.atan2(@velocity.y, @velocity.x) - @angular_acc * delta.as_seconds() / 180f32 * Math::PI
    length = Math.sqrt(@velocity.x * @velocity.x + @velocity.y * @velocity.y)
    x = Math.cos(angle) * length
    y = Math.sin(angle) * length
    @velocity = SF::Vector2f.new(x, y)
  end

  def turn_right(delta : SF::Time)
    self.rotate(@angular_acc * delta.as_seconds())
    angle = Math.atan2(@velocity.y, @velocity.x) + @angular_acc * delta.as_seconds() / 180f32 * Math::PI
    length = Math.sqrt(@velocity.x * @velocity.x + @velocity.y * @velocity.y)
    x = Math.cos(angle) * length
    y = Math.sin(angle) * length
    @velocity = SF::Vector2f.new(x, y)
  end

  def center_origin(width : Float32, height : Float32)
    self.origin = {width * 0.5f32, height * 0.5f32}
  end

  def draw(target : SF::RenderTarget, states : SF::RenderStates)
    states.texture = @texture
    states.transform *= transform

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

    @debug_vertices_circle.try do |d|
      target.draw(d, SF::LinesStrip, states)
    end
  end

  def update(delta : SF::Time)
    self.position += @velocity * delta.as_seconds()
  end
end