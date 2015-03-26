# LED class

assert('LED', "class") do
  LED.class == Class
end

assert('LED', 'include?(EV3)') do
  LED.include?(EV3)
end

assert('LED', 'COLOR') do
  LED.const_defined?(:COLOR) &&
  LED::COLOR[:off]    == 0 &&
  LED::COLOR[:red]    == 1 &&
  LED::COLOR[:green]  == 2 &&
  LED::COLOR[:orange] == 3
end

assert('LED', "color=") do
  e = nil
  begin
    LED.color = :red
    LED.color = :green
    LED.color = :orange
    LED.color = :off
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
