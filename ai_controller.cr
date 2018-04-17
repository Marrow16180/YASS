require "./ship"

class AIController
  def initialize(@controlled : Ship? = nil)
  end

  def update(delta : SF::Time)
    return # TODO: implement me
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