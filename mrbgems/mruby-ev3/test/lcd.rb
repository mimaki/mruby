# LCD class

assert('LCD', "class") do
  LCD.class == Class
end

assert('LCD', 'include?(EV3)') do
  LCD.include?(EV3)
end

assert('LCD', 'WIDTH/HEIGHT') do
  LCD.const_defined?(:WIDTH) &&
  LCD.const_defined?(:HEIGHT)
end

assert('LCD', 'FONT') do
  LCD.const_defined?(:FONT) &&
  LCD::FONT[:small] &&
  LCD::FONT[:medium]
end

assert('LCD', 'COLOR') do
  LCD.const_defined?(:COLOR) &&
  LCD::COLOR[:white] &&
  LCD::COLOR[:black]
end


#
# LCD#cx, cy
#
assert('LCD', 'cx/cy') do
  lcd = LCD.new
  lcd.cx == 0 && lcd.cy == 0
end


#
# LCD#locate
#
assert('LCD', 'locate(x, y)') do
  lcd = LCD.new
  lcd.locate(1, 2)
  lcd.cx == 1 && lcd.cy == 2
end

assert('LCD', 'locate(x)') do
  lcd = LCD.new
  lcd.locate(1, 2)
  lcd.locate(10)
  lcd.cx == 10 && lcd.cy == 2
end

assert('LCD', 'puts+locate') do
  lcd = LCD.new
  lcd.locate(2, 4)
  lcd.puts "abcde\n12345567890"
  lcd.locate(1, 2)
  lcd.cx == 1 && lcd.cy == 2
end


#
# LCD.#new
#
assert('LCD', 'new') do
  l1 = LCD.new
  l1.instance_variable_get('@font')   == LCD::FONT[:small] &&
  l1.instance_variable_get('@left')   == 0 &&
  l1.instance_variable_get('@top')    == 0 &&
  l1.instance_variable_get('@width')  == LCD::WIDTH &&
  l1.instance_variable_get('@height') == LCD::HEIGHT &&
  l1.instance_variable_get('@color')  == LCD::COLOR[:black]
end

assert('LCD', 'new(font)') do
  l1 = LCD.new(:small)
  l2 = LCD.new(:medium)
  l1.instance_variable_get('@font') == LCD::FONT[:small]
  l2.instance_variable_get('@font') == LCD::FONT[:medium]
end

assert('LCD', 'new(font, x, y, w, h)') do
  l1 = LCD.new(:small, 1, 2, 3, 4)
  l1.instance_variable_get('@left')    == 1 &&
  l1.instance_variable_get('@top')     == 2 &&
  l1.instance_variable_get('@width')   == 3 &&
  l1.instance_variable_get('@height')  == 4
end

assert('LCD', 'new(font, x, y, w, h, col)') do
  l1 = LCD.new(:small, 1, 2, 3, 4, :white)
  l2 = LCD.new(:small, 1, 2, 3, 4, :black)
  l1.instance_variable_get('@color') == LCD::COLOR[:white] &&
  l2.instance_variable_get('@color') == LCD::COLOR[:black]
end

#
# LCD#print
#
assert('LCD', 'print') do
  lcd = LCD.new
  lcd.print
  lcd.cx == 0 && lcd.cy == 0
end

assert('LCD', 'print: single line') do
  lcd = LCD.new
  lcd.print '1234567890'
  lcd.cx == 10 && lcd.cy == 0
  true
end

assert('LCD', 'print: multiple line') do
  lcd = LCD.new
  lcd.print '123456789012345678901234567890'
  lcd.cx == 1 && lcd.cy == 1
end

assert('LCD', "print: multiple parameters") do
  lcd = LCD.new
  lcd.print 1, "234", [6, 9]
  lcd.cx == 10 && lcd.cy == 0
end

assert('LCD', 'print: line feed') do
  lcd = LCD.new
  lcd.print "1234567890\n12345678901234567890"
  lcd.cx == 20 && lcd.cy == 1
end

assert('LCD', 'print: window(SMALL_FONT)') do
  lcd = LCD.new(:small, 60, 16, 60, 16)
  lcd.print "12345678901234567890"
  lcd.cx == 0 && lcd.cy == 0
end

assert('LCD', 'print: window(MEDIUM_FONT') do
  lcd = LCD.new(:medium, 60, 20, 100, 32)
  lcd.print "12345678901234567890"
  lcd.cx == 0 && lcd.cy == 0
end

#
# LCD#puts
#
assert('LCD', 'puts') do
  lcd = LCD.new
  lcd.puts
  lcd.cx == 0 && lcd.cy == 1
end

assert('LCD', 'puts: single line') do
  lcd = LCD.new
  lcd.puts '1234567890'
  lcd.cx == 0 && lcd.cy == 1
  true
end

assert('LCD', 'puts: multiple line') do
  lcd = LCD.new
  lcd.puts '123456789012345678901234567890'
  lcd.cx == 0 && lcd.cy == 2
end

assert('LCD', "puts: multiple parameters") do
  lcd = LCD.new
  lcd.puts 1, "234", [6, 9]
  lcd.cx == 0 && lcd.cy == 3
end

assert('LCD', 'puts: line feed') do
  lcd = LCD.new
  lcd.puts "1234567890\n12345678901234567890"
  lcd.cx == 0 && lcd.cy == 2
end

assert('LCD', 'puts: window') do
  lcd = LCD.new(:small, 60, 16, 60, 16)
  lcd.puts "12345678901234567890"
  lcd.cx == 0 && lcd.cy == 1
end

assert('LCD', 'puts: window(MEDIUM_FONT') do
  lcd = LCD.new(:medium, 60, 20, 100, 32)
  lcd.puts "12345678901234567890"
  lcd.cx == 0 && lcd.cy == 1
end


# LCD#clear
assert('LCD', 'clear') do
  lcd = LCD.new
  lcd.puts "abcdefghijklmnopqrstuvwxyz"
  lcd.clear
  lcd.cx == 0 && lcd.cy == 0
end

assert('LCD', 'clear(col)') do
  lcd1 = LCD.new
  lcd1.puts "abcdefghijklmnopqrstuvwxyz"
  lcd1.clear(:white)
  lcd2 = LCD.new
  lcd2.puts "abcdefghijklmnopqrstuvwxyz"
  lcd2.clear(:black)
  lcd1.cx == 0 && lcd1.cy == 0 &&
  lcd2.cx == 0 && lcd2.cy == 0
end
