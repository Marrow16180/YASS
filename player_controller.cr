require "./ship"

class PlayerController
  def initialize(@window : SF::RenderWindow, @controlled : Ship? = nil)
  end

  def update(delta : SF::Time)
    if SF::Keyboard.key_pressed? SF::Keyboard::Up
      accelerate(delta)
    elsif SF::Keyboard.key_pressed? SF::Keyboard::Down
      decelerate(delta)
    end

    if SF::Keyboard.key_pressed? SF::Keyboard::Left
      turn_left(delta)
    elsif SF::Keyboard.key_pressed? SF::Keyboard::Right
      turn_right(delta)
    end
  end

  def accelerate(delta : SF::Time)
    @controlled.try do |c|
      c.accelerate(delta)
    end
  end

  def decelerate(delta : SF::Time)
    @controlled.try do |c|
      c.decelerate(delta)
    end
  end

  def turn_left(delta : SF::Time)
    @controlled.try do |c|
      c.turn_left(delta)
    end
  end

  def turn_right(delta : SF::Time)
    @controlled.try do |c|
      c.turn_right(delta)
    end
  end
end