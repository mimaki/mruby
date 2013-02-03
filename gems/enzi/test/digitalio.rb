assert('DigitalIO', 'class') do
  DigitalIO.class == Class
end

assert('DigitalIO', 'superclass') do
  DigitalIO.superclass == Object
end

assert('DigitalIO', 'include?(ENZI)') do
  DigitalIO.include?(ENZI)
end
