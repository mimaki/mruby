# Device class

assert('Device', "class") do
  Device.class == Class
end

assert('Device', 'include?(EV3)') do
  Device.include?(EV3)
end
