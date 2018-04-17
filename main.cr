require "./crsfml"

require "./ship"
require "./player_controller"

require "./transform_bug"

TIME_PER_FRAME = SF.seconds(1f32 / 60f32)

window = SF::RenderWindow.new(SF::VideoMode.new(800, 640), "YASS")
clock = SF::Clock.new
delta = SF::Time::Zero

texture = SF::Texture.from_file("blueShip.png")

ship = Ship.new(texture, linear_acc: 200f32, angular_acc: 90f32)
ship.position = {400, 320}

player = PlayerController.new(window, ship)

while window.open?
  while (event = window.poll_event)
    case event
    when SF::Event::Closed
      window.close()
    end
  end

  delta += clock.restart()
  while delta >= TIME_PER_FRAME

    player.update(TIME_PER_FRAME)
    ship.update(TIME_PER_FRAME)

    delta -= TIME_PER_FRAME
  end

  window.clear()
  window.draw(ship)
  window.display()
end