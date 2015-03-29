# Balancer class

assert('Balancer', "class") do
  Balancer.class == Class
end

assert('Balancer', 'include?(EV3)') do
  Balancer.include?(EV3)
end

assert('Balancer', 'new') do
  Balancer.new
end

assert('Balancer', 'reset') do
  Balancer.new.reset
end

assert('Balancer', "control") do
  pwm = Balancer.new.control(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0)
  pwm.class == Array &&
  pwm.size == 2
end

assert('Balancer', "control: parameter error") do
  bal = Balancer.new
  eary = []
  begin
    bal.control
  rescue => e
    eary << e
  end
  begin
    bal.control(1.0)
  rescue => e
    eary << e
  end
  begin
    bal.control(1.0, 2.0, 3.0, 4.0, 5.0, 6.0)
  rescue => e
    eary << e
  end
  begin
    bal.control(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0)
  rescue => e
    eary << e
  end
  eary.size == 4
end
