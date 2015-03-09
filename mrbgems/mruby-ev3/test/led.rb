# LED class

assert('LED', "class") do
  LED.class == Class
end

assert('LED', 'include?(EV3)') do
  LED.include?(EV3)
end

assert('LED', 'constants') do
  LED::OFF    == 0 &&
  LED::RED    == 1 &&
  LED::GREEN  == 2 &&
  LED::ORANGE == 3
end

assert('LED', "color=") do
  e = nil
  begin
    LED.color = LED::RED
    LED.color = LED::GREEN
    LED.color = LED::ORANGE
    LED.color = LED::OFF
  rescue => e
  end
  !e
end

assert('LED', "off") do
  e = nil
  begin
    LED.off
  rescue => e
  end
  !e
end
