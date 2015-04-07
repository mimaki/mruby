MRuby::Gem::Specification.new('mruby-ev3rt') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'LEGO MINDSTORMS EV3 libraries for ev3rt'

  spec.add_dependency('mruby-rtos-toppers')
  spec.add_dependency('mruby-tiny-io')
end
