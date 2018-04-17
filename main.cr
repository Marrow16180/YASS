require "./crsfml"
require "./ai_controller"
require "./building"
require "./city"
require "./hud"
require "./player_controller"
require "./ship"

TIME_PER_FRAME = SF.seconds(1f32 / 60f32)

window = SF::RenderWindow.new(SF::VideoMode.new(1024, 768), "YASS")
window.vertical_sync_enabled = true

clock = SF::Clock.new
delta = SF::Time::Zero

blueShipTexture = SF::Texture.from_file("blueShip.png")
greenShipTexture = SF::Texture.from_file("greenShip.png")
buildingTexture = SF::Texture.from_file("building.png")
font = SF::Font.from_file("calibri.ttf")

playerShip = Ship.new(blueShipTexture, linear_acc: 200f32, angular_acc: 90f32)
playerShip.position = {512, 384}
player = PlayerController.new(window, playerShip)

aiShip = Ship.new(greenShipTexture, linear_acc: 200f32, angular_acc: 90f32)
aiShip.position = {512, 0}
ai = AIController.new(aiShip)

city = City.new(buildingTexture)
hud = Hud.new(font, playerShip)

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
    playerShip.update(TIME_PER_FRAME)

    ai.update(TIME_PER_FRAME)
    aiShip.update(TIME_PER_FRAME)

    hud.update(TIME_PER_FRAME)

    delta -= TIME_PER_FRAME
  end

  window.clear()
  window.draw(city)
  window.draw(playerShip)
  window.draw(aiShip)
  window.draw(hud)
  window.display()
end