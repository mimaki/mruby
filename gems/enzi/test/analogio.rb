assert('AnalogIO', 'class') do
  AnalogIO.class == Class
end

assert('AnalogIO', 'superclass') do
  AnalogIO.superclass == Object
end

assert('AnalogIO', 'include?(ENZI)') do
  AnalogIO.include?(ENZI)
end

assert('AnalogIO', 'DEFAULT') do
  AnalogIO::DEFAULT == 0
end

assert('AnalogIO', 'INTERNAL') do
  AnalogIO::INTERNAL == 1
end

assert('AnalogIO', 'EXTERNAL') do
  AnalogIO::EXTERNAL == 2
end

assert('AnalogIO', 'REFERENCE') do
  AnalogIO::REFERENCE[:DEFAULT] == 0 &&
  AnalogIO::REFERENCE[:INTERNAL] == 1 &&
  AnalogIO::REFERENCE[:EXTERNAL] == 2
end

assert('AnalogIO#initialize', 'few args') do
  e1, e2 = nil, nil
  begin
    AnalogIO.new
  rescue => e1 # OK
  end
  begin
    AnalogIO.new(0)
  rescue => e2 # OK
  end
  e1 && e2
end

assert('AnalogIO#initialize', '(int, int)') do
  a, b = nil, nil
  a = AnalogIO.new(A0, INPUT)
  b = AnalogIO.new(A1, OUTPUT)
  a && b
end

assert('AnalogIO#initialize', '(int, int, int, int)') do
  a, b, c = nil, nil, nil
  a = AnalogIO.new(A0, INPUT, AnalogIO::DEFAULT, 8)
  b = AnalogIO.new(A1, INPUT, AnalogIO::INTERNAL, 16)
  c = AnalogIO.new(A2, INPUT, AnalogIO::EXTERNAL, 32)
  a && b && c
end

assert('AnalogIO#initialize', '(sym, sym)') do
  a, b = nil, nil
  a = AnalogIO.new(:A4, :INPUT)
  b = AnalogIO.new(:A5, :OUTPUT)
  a && b
end

assert('AnalogIO#initialize', '(sym, sym, sym, int)') do
  a, b, c = nil, nil, nil
  a = AnalogIO.new(:A0, :INPUT, :DEFAULT, 8)
  b = AnalogIO.new(:A1, :INPUT, :INTERNAL, 16)
  c = AnalogIO.new(:A2, :INPUT, :EXTERNAL, 32)
  a && b && c
end

assert('AnalogIO#initialize', 'illegal symbol') do
  e1, e2, e3 = nil, nil, nil
  begin
    AnalogIO.new(:A6, :INPUT, :DEFAULT, 8)
  rescue => e1 # OK
  end
  begin
    AnalogIO.new(:A1, :UNKNOWN, :INTERNAL, 16)
  rescue => e2 # OK
  end
  begin
    AnalogIO.new(:A2, :INPUT, :SPECIAL, 32)
  rescue => e3 # OK
  end
  e1 && e2 && e3
end
