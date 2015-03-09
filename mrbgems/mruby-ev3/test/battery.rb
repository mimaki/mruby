# Battery class

assert('Battery', "class") do
  Battery.class == Class
end

assert('Battery', 'include?(EV3)') do
  Battery.include?(EV3)
end

assert('Battery', "mA") do
  Battery.mA.class == Fixnum
end

assert('Battery', "mV") do
  Battery.mV.class == Fixnum
end
