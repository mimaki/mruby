##
# EV3 Test

# EV3 module

assert('EV3', 'module') do
  EV3.class == Module
end

assert('EV3', 'Object.include?') do
  Object.include?(EV3)
end
