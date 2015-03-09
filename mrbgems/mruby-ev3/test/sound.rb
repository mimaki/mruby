# Sound class

assert('Sound', "class") do
  Sound.class == Class
end

assert('Sound', 'include?(EV3)') do
  Sound.include?(EV3)
end

assert('Sound', 'tone') do
  Sound.tone(0,    0) == nil &&
  Sound.tone(0.0,  0) == nil &&
  Sound.tone(:c4,  0) == nil &&
  Sound.tone("c5", 0) == nil
end

assert('Sound', 'tone: parameter error') do
  e1 = e2 = nil
  begin
    Sound.tone
  rescue => e1
  end
  begin
    Sound.tone(1)
  rescue => e2
  end
  e1 && e2
end

assert('Sound', 'tone: Unknown tone') do
  e = nil
  begin
    Sound.tone(:unknown, 0)
  rescue => e
  end
  e.class == ArgumentError
end

assert('Sound', 'volume=') do
  e = nil
  begin
    Sound.volume = 0
    Sound.volume = 100
    Sound.volume = 101
  rescue => e
  end
  !e
end

assert('Sound', 'stop') do
  e = nil
  begin
    Sound.stop
  rescue => e
  end
  !e
end
