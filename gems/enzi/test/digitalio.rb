assert('DigitalIO', 'class') do
  DigitalIO.class == Class
end

assert('DigitalIO', 'superclass') do
  DigitalIO.superclass == Object
end

assert('DigitalIO', 'include?(ENZI)') do
  DigitalIO.include?(ENZI)
end

assert('DigitalIO#initialize', 'few args') do
  e1, e2 = nil, nil
  begin
    DigitalIO.new
  rescue => e1 # OK
  end
  begin
    DigitalIO.new(0)
  rescue => e2 # OK
  end
  e1 && e2
end

assert('DigitalIO#initialize', '(int, int)') do
  a, b = nil, nil
  a = DigitalIO.new(D0, INPUT)
  b = DigitalIO.new(D1, OUTPUT)
  a && b
end

assert('DigitalIO#initialize', '(sym, sym)') do
  a, b = nil, nil
  a = DigitalIO.new(:D18, :INPUT)
  b = DigitalIO.new(:D19, :OUTPUT)
  a && b
end

assert('DigitalIO#initialize', 'illegal symbol') do
  e1, e2 = nil, nil
  begin
    DigitalIO.new(:D20, :INPUT)
  rescue => e1 # OK
  end
  begin
    DigitalIO.new(:D19, :UNKNOWN)
  rescue => e2 # OK
  end
  e1 && e2
end

assert('DigitalIO#read') do
  DigitalIO.new(D0, INPUT).read
end

assert('DigitalIO#write') do
  DigitalIO.new(D0, OUTPUT).write(255) == 255
end

assert('DigitalIO#high') do
  DigitalIO.new(D0, OUTPUT).high == HIGH
end

assert('DigitalIO#high?') do
  v = DigitalIO.new(D0, INPUT).high?
  v == true || v == false
end

assert('DigitalIO#low') do
  DigitalIO.new(D0, OUTPUT).low == LOW
end

assert('DigitalIO#low?') do
  v = DigitalIO.new(D0, INPUT).low?
  v == true || v == false
end
