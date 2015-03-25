# Device class

assert('Device', "class") do
  EV3::Device.class == Class
end

assert('Device', 'include?(EV3)') do
  EV3::Device.include?(EV3)
end
