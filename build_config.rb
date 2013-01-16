# target settings
PEPPER_VERSION = "pepper_23"
NACL_SDK = File.join(Dir.pwd, "../nacl_sdk/#{PEPPER_VERSION}")
platform =
  case RUBY_PLATFORM
  when /linux/i then 'linux'
  when /darwin/i then 'mac'
  else raise "#{RUBY_PLATFORM} is not supported yet"
  end
nacltool = "#{NACL_SDK}/toolchain/#{platform}_x86_newlib/bin/x86_64-nacl-"

# make host
MRuby::Build.new do |conf|
  conf.cc = ENV['CC'] || 'gcc'
  conf.ld = ENV['LD'] || 'gcc'
  conf.ar = ENV['AR'] || 'ar'
  # conf.bins = %w(mrbc mruby mirb)
  # conf.cxx = conf.cc
  # conf.objcc = conf.cc
  # conf.asm = conf.cc
  # conf.yacc = 'bison'
  # conf.gperf = 'gperf'
  # conf.cat = 'cat'
  # conf.git = 'git'

  conf.cflags << (ENV['CFLAGS'] || %w(-g -O3 -Wall -Werror-implicit-function-declaration))
  conf.ldflags << (ENV['LDFLAGS'] || %w(-lm))
  # conf.cxxflags = []
  # conf.objccflags = []
  # conf.asmflags = []

  # conf.gem 'doc/mrbgems/ruby_extension_example'
  # conf.gem 'doc/mrbgems/c_extension_example'
  # conf.gem 'doc/mrbgems/c_and_ruby_extension_example'
  # conf.gem :git => 'git@github.com:masuidrive/mrbgems-example.git', :branch => 'master'
end

# make targets
[32, 64].each do |bits|
  MRuby::CrossBuild.new("nacl#{bits}") do |conf|
    #platform =
    #  case RUBY_PLATFORM
    #  when /linux/i then 'linux'
    #  when /darwin/i then 'mac'
    #  else raise "#{RUBY_PLATFORM} is not supported yet"
    #  end
    #nacltool = "#{NACL_SDK}/toolchain/#{platform}_x86_newlib/bin/x86_64-nacl-"

    conf.cc = nacltool + "gcc"
    conf.ld = nacltool + "g++"
    conf.ar = nacltool + "ar"
    conf.bins = []

    conf.cflags << (ENV['CFLAGS'] || %W(-DNACL_ENABLED -m#{bits} -g -O3 -Wall -Werror-implicit-function-declaration))
    conf.ldflags << (ENV['LDFLAGS'] || %W(-m#{bits} -lm))

    # conf.gem 'doc/mrbgems/ruby_extension_example'
    # conf.gem 'doc/mrbgems/c_extension_example'
    # conf.gem 'doc/mrbgems/c_and_ruby_extension_example'
    conf.gem 'ext/enzi'
  end
end
