assert('AnalogIO', 'class') do
  AnalogIO.class == Class
end

assert('AnalogIO', 'superclass') do
  AnalogIO.superclass == Object
end

assert('AnalogIO', 'include?(ENZI)') do
  AnalogIO.include?(ENZI)
end

assert('AnalogIO#initialize', 'few args') do
  a, b = nil, nil
  begin
    a = AnalogIO.new
    b = AnalogIO.new(0)
  rescue => e
  	true
  end
  a || b
end

assert('AnalogIO#initialize', '(int, int)') do
  a, b = nil, nil
  begin
    a = AnalogIO.new(A0, INPUT)
    b = AnalogIO.new(A1, OUTPUT)
  rescue
  	false
  end
  a && b
end

assert('AnalogIO#initialize', '(int, int, int, int)') do
  a, b, c = nil, nil, nil
  begin
    a = AnalogIO.new(A0, INPUT, DEFAULT, 8)
    b = AnalogIO.new(A1, INPUT, INTERNAL, 16)
    c = AnalogIO.new(A2, INPUT, EXTERNAL, 32)
  rescue
  	false
  end
  a && b && c
end

assert('AnalogIO#initialize', '(sym, sym)') do
  a, b = nil, nil
  begin
    a = AnalogIO.new(:A4, :INPUT)
    b = AnalogIO.new(:A5, :OUTPUT)
  rescue
  	false
  end
  a && b
end

assert('AnalogIO#initialize', '(sym, sym, sym, int)') do
  a, b, c = nil, nil, nil
  begin
    a = AnalogIO.new(:A0, :INPUT, :DEFAULT, 8)
    b = AnalogIO.new(:A1, :INPUT, :INTERNAL, 16)
    c = AnalogIO.new(:A2, :INPUT, :EXTERNAL, 32)
  rescue
  	false
  end
  a && b && c
end
