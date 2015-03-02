# Button class

assert('Button', "class") do
  Button.class == Class
end

assert('Button', 'include?(EV3)') do
  Button.include?(EV3)
end

assert('Button', 'new') do
  bl = Button.new(:left)
  br = Button.new(:right)
  bu = Button.new(:up)
  bd = Button.new(:down)
  be = Button.new(:enter)
  bb = Button.new(:back)
  bl.instance_variable_get('@key') == 0 &&
  br.instance_variable_get('@key') == 1 &&
  bu.instance_variable_get('@key') == 2 &&
  bd.instance_variable_get('@key') == 3 &&
  be.instance_variable_get('@key') == 4 &&
  bb.instance_variable_get('@key') == 5
end

assert('Button', '[]') do
  bl = Button[:left]
  br = Button[:right]
  bu = Button[:up]
  bd = Button[:down]
  be = Button[:enter]
  bb = Button[:back]
  bl.instance_variable_get('@key') == 0 &&
  br.instance_variable_get('@key') == 1 &&
  bu.instance_variable_get('@key') == 2 &&
  bd.instance_variable_get('@key') == 3 &&
  be.instance_variable_get('@key') == 4 &&
  bb.instance_variable_get('@key') == 5
end

assert('Button', 'pressed?') do
  Button[:left].pressed? == false
end
